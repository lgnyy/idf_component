#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <Windows.h>
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")

#include "../include/yos_httpd.h"

static const char* TAG = "YOS_HTTPD";

typedef struct yos_http_context
{
	SOCKET listen_sock;
	SOCKET client_sock;
	yos_httpd_uri_handler_t uri_handler;
	HANDLE thread;
	char uri[512];
}yos_http_context_t;


static DWORD WINAPI _thread_func(LPVOID lpThreadParameter)
{
	yos_http_context_t* ctx = (yos_http_context_t*)lpThreadParameter;
	SOCKADDR_IN addr;
	char req_buf[0x800];
	while (1) {
		memset(&addr, 0, sizeof(addr));
		int addr_len = sizeof(addr);
		ctx->client_sock = accept(ctx->listen_sock, (SOCKADDR*)&addr, &addr_len);
		if (ctx->client_sock == INVALID_SOCKET) {
			break;
		}

		int rv = recv(ctx->client_sock, req_buf, sizeof(req_buf), 0);
		if (rv == SOCKET_ERROR) {
			break;
		}

		if (memcmp(req_buf, "GET ", 4) == 0) {
			char* ptr = strchr(req_buf + 4, ' ');
			if ((ptr != NULL) && (ctx->uri_handler != NULL)){
				memset(ctx->uri, 0, sizeof(ctx->uri));
				memcpy(ctx->uri, req_buf + 4, ptr - (req_buf + 4));
				ctx->uri_handler(ctx);
			}
		}
	}

	return 0;
}

yos_httpd_handle_t yos_httpd_create(uint16_t server_port, const char* uri, yos_httpd_uri_handler_t uri_handler)
{
    SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		WSADATA wsaData;
		WSAStartup(MAKEWORD(1, 1), &wsaData);

		s = socket(PF_INET, SOCK_STREAM, 0);
	}
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);

	if (bind(s, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		return NULL;
	}

	listen(s, 5);

	yos_http_context_t* ctx = (yos_http_context_t*)malloc(sizeof(*ctx));
	if (ctx != NULL) {
		ctx->listen_sock = s;
		ctx->client_sock = INVALID_SOCKET;
		ctx->uri_handler = uri_handler;

		ctx->thread = CreateThread(NULL, 0, _thread_func, ctx, 0, NULL);
	}
	else {
		closesocket(s);
	}
	return ctx;
}


int32_t yos_httpd_destory(yos_httpd_handle_t server)
{
	yos_http_context_t* ctx = (yos_http_context_t*)server;
	if (ctx != NULL) {
		closesocket(ctx->listen_sock);
		if (ctx->client_sock != INVALID_SOCKET) {
			closesocket(ctx->client_sock);
		}
		WaitForSingleObject(ctx->thread, 1000);
		free(ctx);
	}
	
    return 0;
}

const char* yos_httpd_req_get_uri(void* req)
{
	yos_http_context_t* ctx = (yos_http_context_t*)req;
	return (ctx != NULL)? ctx->uri : NULL;
}

int32_t yos_httpd_resp_send(void* req, const char* buf, uint32_t buf_len)
{
	yos_http_context_t* ctx = (yos_http_context_t*)req;
	if (req != NULL) {
		char ans_buf[0x800];
		int offset = wsprintfA(ans_buf, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n", buf_len);
		memcpy(ans_buf + offset, buf, buf_len);
		send(ctx->client_sock, ans_buf, offset + buf_len, 0);

		closesocket(ctx->client_sock);
		ctx->client_sock = INVALID_SOCKET;
	}
	return 0;
}
