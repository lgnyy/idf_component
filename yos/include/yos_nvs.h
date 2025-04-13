/*
 * Using NVS to store configuration
 */

#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

enum yos_nvs_namespace_t
{
    YOS_NVS_WIFI_INFO_NAMESPACE,
    YOS_NVS_MIOT_INFO_NAMESPACE,
    YOS_NVS_XMIOT_INFO_NAMESPACE,
    YOS_NVS_WEATHER_INFO_NAMESPACE,
};

typedef int yos_nvs_err_t;

typedef int (*yos_nvs_read_cb_t)(void* arg, const char* key, char* value, size_t vsize);
typedef int (*yos_nvs_write_cb_t)(void* arg, const char* key, const char* value);

yos_nvs_err_t yos_nvs_init(void);

yos_nvs_err_t yos_nvs_check(int namespace_type);
yos_nvs_err_t yos_nvs_load(int namespace_type, int (load_cb)(void* ctx, yos_nvs_read_cb_t read_cb, void* arg), void *ctx);
yos_nvs_err_t yos_nvs_save(int namespace_type, int (save_cb)(void* ctx, yos_nvs_write_cb_t write_cb, void* arg), void *ctx);

typedef struct _yos_nvs_item {
    char* key;
    char* value;
    size_t vsize;
}yos_nvs_item_t;
yos_nvs_err_t yos_nvs_load_ex(int namespace_type, yos_nvs_item_t* items, int count);
yos_nvs_err_t yos_nvs_save_ex(int namespace_type, yos_nvs_item_t* items, int count);

#ifdef __cplusplus
}
#endif
