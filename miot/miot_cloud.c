#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"
#include "yos_uri.h"
#include "yos_http.h"
#include "miot_cloud.h"

static const char *TAG = "MIOT_CLOUD";
#if(ESP_PLATFORM)
#include "esp_log.h"
#else
#define ESP_LOGI( tag, format, ... ) printf("I %s " format "\n", tag, ##__VA_ARGS__)
#endif

static const char* _oauth2_client_id = "2882303761520251711";
static const char* _oauth2_auth_url = "https://account.xiaomi.com/oauth2/authorize";
static const char* _oauth2_api_host = "ha.api.io.mi.com";
static const char* _oauth_get_token_url = "https://ha.api.io.mi.com/app/v2/ha/oauth/get_token";
static const char* _oauth_ca_cert = "\x30\x82\x03\x8E\x30\x82\x02\x76\xA0\x03\x02\x01\x02\x02\x10\x03\x3A\xF1\xE6\xA7\x11\xA9\xA0\xBB\x28\x64\xB1\x1D\x09\xFA\xE5\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x0B\x05\x00\x30\x61\x31\x0B\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55\x53\x31\x15\x30\x13\x06\x03\x55\x04\x0A\x13\x0C\x44\x69\x67\x69\x43\x65\x72\x74\x20\x49\x6E\x63\x31\x19\x30\x17\x06\x03\x55\x04\x0B\x13\x10\x77\x77\x77\x2E\x64\x69\x67\x69\x63\x65\x72\x74\x2E\x63\x6F\x6D\x31\x20\x30\x1E\x06\x03\x55\x04\x03\x13\x17\x44\x69\x67\x69\x43\x65\x72\x74\x20\x47\x6C\x6F\x62\x61\x6C\x20\x52\x6F\x6F\x74\x20\x47\x32\x30\x1E\x17\x0D\x31\x33\x30\x38\x30\x31\x31\x32\x30\x30\x30\x30\x5A\x17\x0D\x33\x38\x30\x31\x31\x35\x31\x32\x30\x30\x30\x30\x5A\x30\x61\x31\x0B\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55\x53\x31\x15\x30\x13\x06\x03\x55\x04\x0A\x13\x0C\x44\x69\x67\x69\x43\x65\x72\x74\x20\x49\x6E\x63\x31\x19\x30\x17\x06\x03\x55\x04\x0B\x13\x10\x77\x77\x77\x2E\x64\x69\x67\x69\x63\x65\x72\x74\x2E\x63\x6F\x6D\x31\x20\x30\x1E\x06\x03\x55\x04\x03\x13\x17\x44\x69\x67\x69\x43\x65\x72\x74\x20\x47\x6C\x6F\x62\x61\x6C\x20\x52\x6F\x6F\x74\x20\x47\x32\x30\x82\x01\x22\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x01\x05\x00\x03\x82\x01\x0F\x00\x30\x82\x01\x0A\x02\x82\x01\x01\x00\xBB\x37\xCD\x34\xDC\x7B\x6B\xC9\xB2\x68\x90\xAD\x4A\x75\xFF\x46\xBA\x21\x0A\x08\x8D\xF5\x19\x54\xC9\xFB\x88\xDB\xF3\xAE\xF2\x3A\x89\x91\x3C\x7A\xE6\xAB\x06\x1A\x6B\xCF\xAC\x2D\xE8\x5E\x09\x24\x44\xBA\x62\x9A\x7E\xD6\xA3\xA8\x7E\xE0\x54\x75\x20\x05\xAC\x50\xB7\x9C\x63\x1A\x6C\x30\xDC\xDA\x1F\x19\xB1\xD7\x1E\xDE\xFD\xD7\xE0\xCB\x94\x83\x37\xAE\xEC\x1F\x43\x4E\xDD\x7B\x2C\xD2\xBD\x2E\xA5\x2F\xE4\xA9\xB8\xAD\x3A\xD4\x99\xA4\xB6\x25\xE9\x9B\x6B\x00\x60\x92\x60\xFF\x4F\x21\x49\x18\xF7\x67\x90\xAB\x61\x06\x9C\x8F\xF2\xBA\xE9\xB4\xE9\x92\x32\x6B\xB5\xF3\x57\xE8\x5D\x1B\xCD\x8C\x1D\xAB\x95\x04\x95\x49\xF3\x35\x2D\x96\xE3\x49\x6D\xDD\x77\xE3\xFB\x49\x4B\xB4\xAC\x55\x07\xA9\x8F\x95\xB3\xB4\x23\xBB\x4C\x6D\x45\xF0\xF6\xA9\xB2\x95\x30\xB4\xFD\x4C\x55\x8C\x27\x4A\x57\x14\x7C\x82\x9D\xCD\x73\x92\xD3\x16\x4A\x06\x0C\x8C\x50\xD1\x8F\x1E\x09\xBE\x17\xA1\xE6\x21\xCA\xFD\x83\xE5\x10\xBC\x83\xA5\x0A\xC4\x67\x28\xF6\x73\x14\x14\x3D\x46\x76\xC3\x87\x14\x89\x21\x34\x4D\xAF\x0F\x45\x0C\xA6\x49\xA1\xBA\xBB\x9C\xC5\xB1\x33\x83\x29\x85\x02\x03\x01\x00\x01\xA3\x42\x30\x40\x30\x0F\x06\x03\x55\x1D\x13\x01\x01\xFF\x04\x05\x30\x03\x01\x01\xFF\x30\x0E\x06\x03\x55\x1D\x0F\x01\x01\xFF\x04\x04\x03\x02\x01\x86\x30\x1D\x06\x03\x55\x1D\x0E\x04\x16\x04\x14\x4E\x22\x54\x20\x18\x95\xE6\xE3\x6E\xE6\x0F\xFA\xFA\xB9\x12\xED\x06\x17\x8F\x39\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x0B\x05\x00\x03\x82\x01\x01\x00\x60\x67\x28\x94\x6F\x0E\x48\x63\xEB\x31\xDD\xEA\x67\x18\xD5\x89\x7D\x3C\xC5\x8B\x4A\x7F\xE9\xBE\xDB\x2B\x17\xDF\xB0\x5F\x73\x77\x2A\x32\x13\x39\x81\x67\x42\x84\x23\xF2\x45\x67\x35\xEC\x88\xBF\xF8\x8F\xB0\x61\x0C\x34\xA4\xAE\x20\x4C\x84\xC6\xDB\xF8\x35\xE1\x76\xD9\xDF\xA6\x42\xBB\xC7\x44\x08\x86\x7F\x36\x74\x24\x5A\xDA\x6C\x0D\x14\x59\x35\xBD\xF2\x49\xDD\xB6\x1F\xC9\xB3\x0D\x47\x2A\x3D\x99\x2F\xBB\x5C\xBB\xB5\xD4\x20\xE1\x99\x5F\x53\x46\x15\xDB\x68\x9B\xF0\xF3\x30\xD5\x3E\x31\xE2\x8D\x84\x9E\xE3\x8A\xDA\xDA\x96\x3E\x35\x13\xA5\x5F\xF0\xF9\x70\x50\x70\x47\x41\x11\x57\x19\x4E\xC0\x8F\xAE\x06\xC4\x95\x13\x17\x2F\x1B\x25\x9F\x75\xF2\xB1\x8E\x99\xA1\x6F\x13\xB1\x41\x71\xFE\x88\x2A\xC8\x4F\x10\x20\x55\xD7\xF3\x14\x45\xE5\xE0\x44\xF4\xEA\x87\x95\x32\x93\x0E\xFE\x53\x46\xFA\x2C\x9D\xFF\x8B\x22\xB9\x4B\xD9\x09\x45\xA4\xDE\xA4\xB8\x9A\x58\xDD\x1B\x7D\x52\x9F\x8E\x59\x43\x88\x81\xA4\x9E\x26\xD5\x6F\xAD\xDD\x0D\xC6\x37\x7D\xED\x03\x92\x1B\xE5\x77\x5F\x76\xEE\x3C\x8D\xC4\x5D\x56\x5B\xA2\xD9\x66\x6E\xB3\x35\x37\xE5\x32\xB6";

int miot_cloud_gen_auth_url(const char* redirect_url, const char* state, const char* scope, int skip_confirm, char* output, size_t max_out_len) {
	if ((redirect_url == NULL) || (state == NULL) || (output == NULL)) {
		return MIOT_CLOUD_ERR_INVALID_ARG;
	}

	strcpy(output, _oauth2_auth_url);
	strcat(output, "?");
	strcat(output, "redirect_uri=");
	yos_uri_encode(redirect_url, output + strlen(output));
	strcat(output, "&client_id=");
	strcat(output, _oauth2_client_id);
	strcat(output, "&response_type=code");
	strcat(output, "&state=");
	strcat(output, state);
	if (scope != NULL) {
		strcat(output, "&scope=");
		yos_uri_encode(scope, output + strlen(output));
	}
	strcat(output, "&skip_confirm=");
	strcat(output, skip_confirm?"True" : "False");
	return 0;
}

static int __get_token(const char* data_s, cJSON** resp_json) {
	int ret = 0;
	char* url = (char*)malloc(256 + strlen(data_s));
	if (url == NULL) {
		return MIOT_CLOUD_ERR_NO_MEM;
	}
	strcpy(url, _oauth_get_token_url);
	strcat(url, "?data=");
	yos_uri_encode(data_s, url+strlen(url));
	ESP_LOGI(TAG, "url=%s", url);

	const char* header = "Content-Type: application/x-www-form-urlencoded";
	uint8_t* resp = NULL;
	uint32_t resp_len = 0;
	ret = yos_http_static_request(url, _oauth_ca_cert, header, NULL, 0, &resp, &resp_len);
	if (ret != 0) {
		goto end;
	}
	if (resp == NULL) {
		ret = MIOT_CLOUD_ERR_NO_MEM;
		goto end;
	}
	ESP_LOGI(TAG, "resp=%s", (char*)resp);

	*resp_json = cJSON_Parse((char*)resp);
	if (*resp_json == NULL) {
		ret = MIOT_CLOUD_ERR_OUT_BUFF;
		goto end;
	}

	ret = (int)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(*resp_json, "code"));

end:
	if (url) free(url);
	if (resp) yos_http_static_free(resp);
	return ret;
}

int miot_cloud_get_access_token(const char* redirect_url, const char* code, void** resp_json) {
	//cJSON* data = cJSON_CreateObject();
	//cJSON_AddItemToObject(data, "client_id", cJSON_CreateString(_oauth2_client_id));
	//cJSON_AddItemToObject(data, "redirect_uri", cJSON_CreateString(redirect_url));
	//cJSON_AddItemToObject(data, "code", cJSON_CreateString(code));
	
	// Json standard does not support int64, https://zhuanlan.zhihu.com/p/519361799
	char* data = (char*)malloc(strlen(redirect_url) + strlen(code) + 256);
	if (data == NULL) {
		return MIOT_CLOUD_ERR_NO_MEM;
	}
	sprintf(data, "{\"client_id\":%s,\"redirect_uri\":\"%s\",\"code\":\"%s\"}", _oauth2_client_id, redirect_url, code);
	ESP_LOGI(TAG, "data=%s", data);
	int ret = __get_token(data, (cJSON**)resp_json);
	free(data);
	return ret;
}

int miot_cloud_refresh_access_token(const char* redirect_url, const char* refresh_token, void** resp_json) {
	char* data = (char*)malloc(strlen(redirect_url) + strlen(refresh_token) + 256);
	if (data == NULL) {
		return MIOT_CLOUD_ERR_NO_MEM;
	}
	sprintf(data, "{\"client_id\":%s,\"redirect_uri\":\"%s\",\"refresh_token\":\"%s\"}", _oauth2_client_id, redirect_url, refresh_token);
	ESP_LOGI(TAG, "data=%s", data);
	int ret = __get_token(data, (cJSON**)resp_json);
	return ret;
}

static int __save_access_token(const cJSON* resp_json,
	int (*write_cb)(void* arg, const char* key, const char* value), void* arg) {

	const cJSON* result = cJSON_GetObjectItemCaseSensitive(resp_json, "result");
	if (result == NULL) {
		return MIOT_CLOUD_ERR_INVALID_ARG;
	}
	const char* access_token = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(result, "access_token"));
	const char* refresh_token = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(result, "refresh_token"));
	int expires_in = (int)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(result, "expires_in"));
	char expires_ts[20];
	sprintf(expires_ts, "%lld", time(NULL) + expires_in);
	write_cb(arg, "access_token", access_token);
	write_cb(arg, "refresh_token", refresh_token);
	return write_cb(arg, "expires_ts", expires_ts);
}

int miot_cloud_get_access_token_w(const char* redirect_url, const char* code,
	int (*write_cb)(void* arg, const char* key, const char* value), void* arg) {

	cJSON* resp_json = NULL;
	int ret = miot_cloud_get_access_token(redirect_url, code, (void**)&resp_json);
	if (ret == 0) {
		ret = __save_access_token(resp_json, write_cb, arg);
	}
	if (resp_json != NULL) {
		cJSON_Delete(resp_json);
	}
	return ret;
}

int miot_cloud_refresh_access_token_w(const char* redirect_url, const char* refresh_token,
	int (*write_cb)(void* arg, const char* key, const char* value), void* arg) {

	cJSON* resp_json = NULL;
	int ret = miot_cloud_refresh_access_token(redirect_url, refresh_token, (void**)&resp_json);
	if (ret == 0) {
		ret = __save_access_token(resp_json, write_cb, arg);
	}
	if (resp_json != NULL) {
		cJSON_Delete(resp_json);
	}
	return ret;
}

static int __mihome_api_post(const char* access_token, const char* url_path, const char* data, cJSON** resp_json) {
	int ret;
	char url[256], header[512];
	sprintf(url, "https://%s%s", _oauth2_api_host, url_path);
	sprintf(header, "Host: %s\r\nX-Client-BizId: haapi\r\nContent-Type: application/json\r\nAuthorization: Bearer%s\r\nX-Client-AppId: %s", _oauth2_api_host, access_token, _oauth2_client_id);
	ESP_LOGI(TAG, "url=%s", url);
	ESP_LOGI(TAG, "header=%s", header);

	uint8_t* resp = NULL;
	uint32_t resp_len = 0;
	ret = yos_http_static_request(url, _oauth_ca_cert, header, (uint8_t*)data, strlen(data), &resp, &resp_len);
	if (ret != 0) {
		goto end;
	}
	if (resp == NULL) {
		ret = MIOT_CLOUD_ERR_NO_MEM;
		goto end;
	}
	ESP_LOGI(TAG, "resp=%s", (char*)resp);

	if (resp_json != NULL) {
		*resp_json = cJSON_Parse((char*)resp);
		if (*resp_json == NULL) {
			ret = MIOT_CLOUD_ERR_OUT_BUFF;
			goto end;
		}

		ret = (int)cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(*resp_json, "code"));
	}
end:
	if (resp) yos_http_static_free(resp);
	return ret;
}

int miot_cloud_api_post(const char* access_token, const char* url_path, const uint8_t* data, uint32_t data_len, uint8_t** resp, uint32_t* resp_len) {
	char url[256], header[512];
	sprintf(url, "https://%s%s", _oauth2_api_host, url_path);
	sprintf(header, "Host: %s\r\nX-Client-BizId: haapi\r\nContent-Type: application/json\r\nAuthorization: Bearer%s\r\nX-Client-AppId: %s", _oauth2_api_host, access_token, _oauth2_client_id);
	return yos_http_static_request(url, _oauth_ca_cert, header, data, data_len, resp, resp_len);
}

void miot_cloud_free(void* ptr) {
	yos_http_static_free(ptr);
}

int miot_cloud_get_prop(const char* access_token, const char* did, int siid, int piid, void** resp_json) {
	char data[256];
	sprintf(data, "{\"datasource\":1, \"params\":[{\"did\":\"%s\", \"siid\":%d, \"piid\":%d}]}", did, siid, piid);
	ESP_LOGI(TAG, "data=%s", data);

	int ret = __mihome_api_post(access_token, "/app/v2/miotspec/prop/get", data, (cJSON **)resp_json);
	return ret;
}

int miot_cloud_get_props(const char* access_token, const miot_cloud_param_did_t* param_dids, uint32_t count, void** resp_json) {
	char* data = malloc(256 + 128*count);
	if (data == NULL) {
		return MIOT_CLOUD_ERR_NO_MEM;
	}
	strcpy(data, "{\"datasource\":1, \"params\":[");
	uint32_t i;
	for (i = 0; i < count; i++) {
		sprintf(data + strlen(data), "%s{\"did\":\"%s\", \"siid\":%d, \"piid\":%d}", ((i == 0) ? "" : ","), param_dids->did, param_dids->siid, param_dids->piid); 
		param_dids++;
	}
	strcat(data, "]}");
	ESP_LOGI(TAG, "data=%s", data);

	int ret = __mihome_api_post(access_token, "/app/v2/miotspec/prop/get", data, (cJSON**)resp_json);
	return ret;
}

int miot_cloud_set_prop(const char* access_token, const char* did, int siid, int piid, const char* value, void** resp_json) {
	char data[256];
	sprintf(data, "{\"datasource\":1, \"params\":[{\"did\":\"%s\", \"siid\":%d, \"piid\":%d, \"value\":%s}]}", did, siid, piid, value);
	ESP_LOGI(TAG, "data=%s", data);

	int ret = __mihome_api_post(access_token, "/app/v2/miotspec/prop/set", data, (cJSON**)resp_json);
	return ret;
}

int miot_cloud_set_props(const char* access_token, const miot_cloud_param_did_t* param_dids, uint32_t count, void** resp_json) {
	char* data = malloc(256 + 128 * count);
	if (data == NULL) {
		return MIOT_CLOUD_ERR_NO_MEM;
	}
	strcpy(data, "{\"datasource\":1, \"params\":[");
	uint32_t i;
	for (i = 0; i < count; i++) {
		sprintf(data + strlen(data), "%s{\"did\":\"%s\", \"siid\":%d, \"piid\":%d, \"value\":%s}", ((i == 0) ? "" : ","), param_dids->did, param_dids->siid, param_dids->piid, param_dids->value);
		param_dids++;
	}
	strcat(data, "]}");
	ESP_LOGI(TAG, "data=%s", data);

	int ret = __mihome_api_post(access_token, "/app/v2/miotspec/prop/set", data, (cJSON**)resp_json);
	return ret;
}

int miot_cloud_action(const char* access_token, const char* did, int siid, int aiid, const char* in_list, void** resp_json) {
	char data[256];
	sprintf(data, "{\"params\":{\"did\":\"%s\", \"siid\":%d, \"aiid\":%d, \"in\":[%s]}}", did, siid, aiid, in_list);
	ESP_LOGI(TAG, "data=%s", data);

	int ret = __mihome_api_post(access_token, "/app/v2/miotspec/action", data, (cJSON**)resp_json);
	return ret;
}