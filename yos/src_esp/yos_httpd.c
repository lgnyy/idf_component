
#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_event.h"
//#include "esp_tls.h"
#include "esp_http_server.h"

#include "../include/yos_httpd.h"

static const char *TAG = "YOS_HTTPD";


yos_httpd_handle_t yos_httpd_create(uint16_t server_port, const char* uri, yos_httpd_uri_handler_t uri_handler)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    config.server_port = server_port;
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to httpd_start, server_port: %d", server_port);
        return NULL;
    }

    if (uri_handler != NULL){
        const httpd_uri_t uri_get = {
            .uri = uri,
            .method = HTTP_ANY,
            .handler = (esp_err_t (*)(httpd_req_t *))uri_handler,
            .user_ctx = NULL
        };
        if (httpd_register_uri_handler(server, &uri_get) != ESP_OK){
            ESP_LOGE(TAG, "Failed to httpd_register_uri_handler");
        }
    }
 
    return server;
}

int32_t yos_httpd_destory(yos_httpd_handle_t server)
{
    return (int32_t)httpd_stop(server);
}

const char* yos_httpd_req_get_method(void* req)
{
    if (req == NULL){
        return "";
    }
    int method = ((httpd_req_t*)req)->method;
    return (method == HTTP_GET)? "GET" : "POST";
}

const char* yos_httpd_req_get_uri(void* req)
{
    return (req != NULL)? ((httpd_req_t*)req)->uri : NULL;
}

int32_t yos_httpd_resp_send(void *req, const char *buf, uint32_t buf_len)
{
    return (int32_t)httpd_resp_send((httpd_req_t*)req, buf, (ssize_t)buf_len);
}

