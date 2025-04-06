
#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_event.h"
//#include "esp_tls.h"
#include "esp_http_server.h"

#include "../include/yos_httpd.h"

static const char *TAG = "YOS_HTTPD";


static void _user_ctx_free(void* arg)
{
    vEventGroupDelete((EventGroupHandle_t)arg);
}

yos_httpd_handle_t yos_httpd_create(uint16_t server_port)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    config.server_port = server_port;
    config.global_user_ctx = xEventGroupCreate();
    config.global_user_ctx_free_fn = _user_ctx_free;
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to httpd_start, server_port: %d", server_port);
        return NULL;
    }

    return server;
}

int32_t yos_httpd_destory(yos_httpd_handle_t server)
{
    if (server == NULL){
        return -1;
    }    
    //vEventGroupDelete((EventGroupHandle_t)httpd_get_global_user_ctx(server));
    return (int32_t)httpd_stop(server);
}

int32_t yos_register_uri_handler(yos_httpd_handle_t server, const char* uri, yos_httpd_uri_handler_t uri_handler, void* udata)
{
    if (server == NULL || uri == NULL || uri_handler == NULL){
        return -1;
    }
    const httpd_uri_t uri_get = {
        .uri = uri,
        .method = HTTP_ANY,
        .handler = (esp_err_t (*)(httpd_req_t *))uri_handler,
        .user_ctx = udata
    };
    return (int32_t)httpd_register_uri_handler(server, &uri_get);
}

int32_t yos_unregister_uri_handler(yos_httpd_handle_t server, const char* uri)
{
    if (server == NULL || uri == NULL){
        return -1;
    }
    return (int32_t)httpd_unregister_uri(server, uri);
}

int32_t yos_httpd_wait_bits(yos_httpd_handle_t server, uint32_t value, uint32_t ms)
{
    ESP_LOGI(TAG, "wait bits= %lx, ms= %d", value, (int)ms);
    EventGroupHandle_t event_group = (EventGroupHandle_t)httpd_get_global_user_ctx(server);

    EventBits_t bits = xEventGroupWaitBits(event_group, value, pdFALSE, pdFALSE, pdMS_TO_TICKS(ms));

    ESP_LOGI(TAG, "wait bits end");
    return (bits & BIT0)? 0 : -1;
}

int32_t yos_httpd_set_bits(yos_httpd_handle_t server, uint32_t value)
{
    ESP_LOGI(TAG, "set bits: %lx", value);
    EventGroupHandle_t event_group = (EventGroupHandle_t)httpd_get_global_user_ctx(server);
    return xEventGroupSetBits(event_group, value);
}

const char* yos_httpd_req_get_method(void* req)
{
    if (req == NULL){
        return "";
    }
    int method = ((httpd_req_t*)req)->method;
    return (method == HTTP_GET)? "GET" : "POST";
}

yos_httpd_handle_t yos_httpd_req_get_handle(void* req)
{
    if (req == NULL){
        return NULL;
    }
    return (yos_httpd_handle_t)((httpd_req_t*)req)->handle;
}

void* yos_httpd_req_get_udata(void* req)
{
    if (req == NULL){
        return NULL;
    }
    return ((httpd_req_t*)req)->user_ctx;
}


const char* yos_httpd_req_get_uri(void* req, uint32_t* out_len)
{
    if (req == NULL){
        return NULL;
    } 
    const char* uri = ((httpd_req_t*)req)->uri;
    if ((uri != NULL) && (out_len != NULL)){
        *out_len = strlen(uri);
    }
    ESP_LOGI(TAG, "uri: %s", uri);
    return uri;
}

int32_t yos_httpd_resp_send(void *req, const char *buf, uint32_t buf_len)
{
    return (int32_t)httpd_resp_send((httpd_req_t*)req, buf, (ssize_t)buf_len);
}

