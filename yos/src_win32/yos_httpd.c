#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "mongoose.h"   // To build, run: cc main.c mongoose.c

#include "../include/yos_httpd.h"


typedef struct yos_http_context
{
	struct mg_mgr mgr;  // Declare event manager
	struct mg_connection* listen_c;
	yos_httpd_uri_handler_t uri_handler; // TODO: multi-instance
	void* user_ctx;
	HANDLE thread;
	uint32_t is_running : 1;
}yos_http_context_t;

typedef struct yos_http_req_ext
{
	void* ev_data;
	char* headers;
}yos_http_req_ext_t;
#define _yos_httpd_get_req_ext(c) ((yos_http_req_ext_t*)(c+1))
#define _yos_httpd_get_req_message(c) ((struct mg_http_message*)(_yos_httpd_get_req_ext(c)->ev_data))

// HTTP server event handler function
static void ev_handler(struct mg_connection* c, int ev, void* ev_data) {
	if (ev == MG_EV_HTTP_MSG) {
		yos_http_context_t* ctx = (yos_http_context_t*)(c->mgr);
		if (ctx->uri_handler != NULL) {
			_yos_httpd_get_req_ext(c)->ev_data = ev_data;
			if (_yos_httpd_get_req_ext(c)->headers != NULL) {
				_yos_httpd_get_req_ext(c)->headers[0] = '\0';
			}
			ctx->uri_handler(c);
		}
	}
	else if (ev == MG_EV_WAKEUP) {
		((yos_http_context_t*)(c->mgr))->is_running = 0;
	}
	else if (ev == MG_EV_CLOSE) {
		if (_yos_httpd_get_req_ext(c)->headers != NULL) {
			free(_yos_httpd_get_req_ext(c)->headers);
			_yos_httpd_get_req_ext(c)->headers = NULL;
		}
	}
}

static DWORD WINAPI _thread_func(LPVOID lpThreadParameter){
	yos_http_context_t* ctx = (yos_http_context_t*)lpThreadParameter;
	ctx->is_running = 1;
	while(ctx->is_running) {          // Run an infinite event loop
		mg_mgr_poll(&(ctx->mgr), 6000);
	}
	return 0;
}

yos_httpd_handle_t yos_httpd_create(uint16_t server_port)
{
	char url[64];
	mg_snprintf(url, sizeof(url), "http://0.0.0.0:%d", server_port);

	yos_http_context_t* ctx = (yos_http_context_t*)malloc(sizeof(*ctx));
	if (ctx == NULL) {
		return NULL;
	}

	mg_mgr_init(&(ctx->mgr));  // Initialise event manager
	ctx->mgr.extraconnsize = sizeof(yos_http_req_ext_t);
	ctx->listen_c = mg_http_listen(&(ctx->mgr), url, ev_handler, NULL);  // Setup listener
	//mg_wakeup_init(&(ctx->mgr));

	ctx->uri_handler = NULL;
	ctx->user_ctx = NULL;
	//ctx->thread = NULL;
	ctx->thread = CreateThread(NULL, 0, _thread_func, ctx, 0, NULL);
	return ctx;
}

int32_t yos_httpd_destory(yos_httpd_handle_t server)
{
	yos_http_context_t* ctx = (yos_http_context_t*)server;
	if (ctx != NULL) {
		if (ctx->thread != NULL) {
#if 0
			mg_wakeup(&(ctx->mgr), ctx->listen_c->id, "close", 5);
			WaitForSingleObject(ctx->thread, 10000);
#else
			SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
			SOCKADDR_IN addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
			addr.sin_port = ctx->listen_c->loc.port;
			connect(s, (SOCKADDR*)&addr, sizeof(addr));

			ctx->is_running = 0;
			WaitForSingleObject(ctx->thread, 10000);
			closesocket(s);
#endif
		}

		mg_mgr_free(&(ctx->mgr));
		free(ctx);
	}

	return 0;
}

int32_t yos_register_uri_handler(yos_httpd_handle_t server, const char* uri, yos_httpd_uri_handler_t uri_handler, void* udata)
{
	yos_http_context_t* ctx = (yos_http_context_t*)server;
	ctx->uri_handler = uri_handler;
	ctx->user_ctx = udata;
	return 0;
}
int32_t yos_unregister_uri_handler(yos_httpd_handle_t server, const char* uri)
{
	return 0;
}


yos_httpd_handle_t yos_httpd_req_get_handle(void* req) {
	struct mg_connection* c = (struct mg_connection*)req;
	return (c != NULL) ? (yos_httpd_handle_t)(c->mgr) : NULL;
}

void* yos_httpd_req_get_udata(void* req) {
	struct mg_connection* c = (struct mg_connection*)req;
	if (c != NULL) {
		yos_http_context_t* ctx = (yos_http_context_t*)(c->mgr);
		return ctx->user_ctx;
	}
	return NULL;
}

const char* yos_httpd_req_get_method(void* req) {
	struct mg_connection* c = (struct mg_connection*)req;
	if (c != NULL) {
		struct mg_http_message* hm = _yos_httpd_get_req_message(c);
		char* method = alloca(hm->method.len + 1);
		memcpy(method, hm->method.buf, hm->method.len);
		method[hm->method.len] = '\0';
		return method;
	}
	return "";
}

const char* yos_httpd_req_get_uri(void* req, uint32_t* out_len)
{
	struct mg_connection* c = (struct mg_connection*)req;
	if (c != NULL) {
		struct mg_http_message* hm = _yos_httpd_get_req_message(c);
		if (out_len != NULL) {
			*out_len = (uint32_t)(hm->query.buf? (hm->query.buf - hm->uri.buf + hm->query.len) : hm->uri.len);
		}
		return hm->uri.buf;
	}
	else {
		if (out_len != NULL) {
			*out_len = 0;
		}
		return NULL;
	}
}

char* yos_httpd_req_recv_body(void* req, uint32_t* out_len)
{
	struct mg_connection* c = (struct mg_connection*)req;
	if (c != NULL) {
		struct mg_http_message* hm = _yos_httpd_get_req_message(c);
		if (out_len != NULL) {
			*out_len = hm->body.len;
		}
		return hm->body.buf;
	}
	else {
		if (out_len != NULL) {
			*out_len = 0;
		}
		return NULL;
	}
}
void yos_httpd_req_body_free(void* req, char* body)
{
}

int32_t yos_httpd_resp_set_hdr(void* req, const char* field, const char* value)
{
	struct mg_connection* c = (struct mg_connection*)req;
	char* headers = _yos_httpd_get_req_ext(c)->headers;
	size_t headers_len = (headers != NULL)? strlen(headers) : 0;
	char* new_headers = realloc(headers, headers_len + strlen(field) + strlen(value) + 64);
	if (new_headers == NULL) {
		return -1;
	}
	headers = new_headers + headers_len;
	strcpy(headers, field);
	strcat(headers, "= ");
	strcat(headers, value);
	strcat(headers, "\r\n");
	_yos_httpd_get_req_ext(c)->headers = new_headers;
	return 0; 
}

int32_t yos_httpd_resp_send(void* req, const char* buf, uint32_t buf_len)
{
	struct mg_connection* c = (struct mg_connection*)req;
	if (c != NULL) {
		const char* headers = _yos_httpd_get_req_ext(c)->headers;
		mg_http_reply(c, 200, headers, "%.*s", buf_len, buf);
	}
	return 0;
}

int32_t yos_httpd_resp_send_file(void* req, const char* fname)
{
	struct mg_connection* c = (struct mg_connection*)req;
	if (req != NULL) {
		struct mg_http_message* hm = _yos_httpd_get_req_message(c);
		struct mg_http_serve_opts opts = { .root_dir = "." };
		mg_http_serve_file(c, hm, fname, &opts);
	}
	return 0;
}
