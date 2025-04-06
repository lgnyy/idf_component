#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "mongoose.h"   // To build, run: cc main.c mongoose.c

#include "../include/yos_httpd.h"


typedef struct yos_http_context
{
	struct mg_mgr mgr;  // Declare event manager
	struct mg_connection* listen_c;
	yos_httpd_uri_handler_t uri_handler;
	void* user_ctx;
	HANDLE thread;
	uint32_t is_running : 1;
}yos_http_context_t;


// HTTP server event handler function
static void ev_handler(struct mg_connection* c, int ev, void* ev_data) {
	if (ev == MG_EV_HTTP_MSG) {
		yos_http_context_t* ctx = (yos_http_context_t*)(c->mgr);
		if (ctx->uri_handler != NULL) {
			c->fn_data = ev_data; // c->fd = ev_data;
			ctx->uri_handler(c);
		}
	}
	else if (ev == MG_EV_WAKEUP) {	
		((yos_http_context_t*)(c->mgr))->is_running = 0;
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
	ctx->listen_c = mg_http_listen(&(ctx->mgr), url, ev_handler, NULL);  // Setup listener
	//mg_wakeup_init(&(ctx->mgr));

	ctx->uri_handler = NULL;
	ctx->user_ctx = NULL;
	ctx->thread = NULL;
	// ctx->thread = CreateThread(NULL, 0, _thread_func, ctx, 0, NULL);
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

int32_t yos_httpd_wait_bits(yos_httpd_handle_t server, uint32_t value, uint32_t ms) {
	if ((server == NULL) || (ms == 0)) {
		return -1;
	}
	yos_http_context_t* ctx = (yos_http_context_t*)server;
	time_t endt = time(NULL) + (ms / 1000);
	ctx->is_running = 1;
	do {
		mg_mgr_poll(&(ctx->mgr), 1000);
	} while (ctx->is_running && (endt > time(NULL)));
	return ctx->is_running ? -1 : 0;
}

int32_t yos_httpd_set_bits(yos_httpd_handle_t server, uint32_t value) {
	if (server == NULL) {
		return - 1;
	}
	yos_http_context_t* ctx = (yos_http_context_t*)server;
	ctx->is_running = value;
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
		struct mg_http_message* hm = (struct mg_http_message*)(c->fn_data);
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
		struct mg_http_message* hm = (struct mg_http_message*)(c->fn_data);
		if (out_len != NULL) {
			*out_len = (uint32_t)(hm->query.buf - hm->uri.buf + hm->query.len);
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

int32_t yos_httpd_resp_send(void* req, const char* buf, uint32_t buf_len)
{
	struct mg_connection* c = (struct mg_connection*)req;
	if (c != NULL) {
		mg_http_reply(c, 200, "", "%.*s", buf_len, buf);
	}
	return 0;
}
