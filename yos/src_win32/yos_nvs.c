#include <stdio.h>
#include <string.h> // strcpy
#include <windows.h>
#include "yos_nvs.h"

static const char *TAG = "NVS";
/* 用于读取nvs的命名空间 */
static const char* namespace_array[] = { "wifi_info", "xmiot_info", "weather_info" };


static char _ini_file_name[MAX_PATH] = { 0 };

static int yos_nvs_read(void* arg, const char* key, char* value, size_t vsize) {
    if (!_ini_file_name[0]) {
        GetModuleFileNameA(NULL, _ini_file_name, sizeof(_ini_file_name));
        size_t tlen = strlen(_ini_file_name);
        if (tlen >= 4) {
            strcpy(_ini_file_name+ tlen-3, "ini");
        }
    }
    DWORD dwRet = GetPrivateProfileStringA((char*)arg, key, "", value, vsize, _ini_file_name);
    return (dwRet > 0)? 0 : -1;
}
static int yos_nvs_write(void* arg, const char* key, const char* value) {
    printf("nvs save key:%s, value:%s\n", key, value);
    WritePrivateProfileStringA((char*)arg, key, value, _ini_file_name);
    return 0;
}


yos_nvs_err_t yos_nvs_init(void)
{
    return 0;
}

yos_nvs_err_t yos_nvs_check(int namespace_type){
    return 0;
}

yos_nvs_err_t yos_nvs_load(int namespace_type, int (load_cb)(void* ctx, yos_nvs_read_cb_t read_cb, void* arg), void *ctx){
    return load_cb(ctx, yos_nvs_read, (void*)namespace_array[namespace_type]);
}

yos_nvs_err_t yos_nvs_save(int namespace_type, int (save_cb)(void* ctx, yos_nvs_write_cb_t write_cb, void* arg), void *ctx){
    return save_cb(ctx, yos_nvs_write, (void*)namespace_array[namespace_type]);
}
