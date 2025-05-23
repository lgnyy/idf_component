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
static const char* _oauth_ca_cert = "\x30\x82\x05\xDE\x30\x82\x03\xC6\xA0\x03\x02\x01\x02\x02\x10\x01\xFD\x6D\x30\xFC\xA3\xCA\x51\xA8\x1B\xBC\x64\x0E\x35\x03\x2D\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x0C\x05\x00\x30\x81\x88\x31\x0B\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55\x53\x31\x13\x30\x11\x06\x03\x55\x04\x08\x13\x0A\x4E\x65\x77\x20\x4A\x65\x72\x73\x65\x79\x31\x14\x30\x12\x06\x03\x55\x04\x07\x13\x0B\x4A\x65\x72\x73\x65\x79\x20\x43\x69\x74\x79\x31\x1E\x30\x1C\x06\x03\x55\x04\x0A\x13\x15\x54\x68\x65\x20\x55\x53\x45\x52\x54\x52\x55\x53\x54\x20\x4E\x65\x74\x77\x6F\x72\x6B\x31\x2E\x30\x2C\x06\x03\x55\x04\x03\x13\x25\x55\x53\x45\x52\x54\x72\x75\x73\x74\x20\x52\x53\x41\x20\x43\x65\x72\x74\x69\x66\x69\x63\x61\x74\x69\x6F\x6E\x20\x41\x75\x74\x68\x6F\x72\x69\x74\x79\x30\x1E\x17\x0D\x31\x30\x30\x32\x30\x31\x30\x30\x30\x30\x30\x30\x5A\x17\x0D\x33\x38\x30\x31\x31\x38\x32\x33\x35\x39\x35\x39\x5A\x30\x81\x88\x31\x0B\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55\x53\x31\x13\x30\x11\x06\x03\x55\x04\x08\x13\x0A\x4E\x65\x77\x20\x4A\x65\x72\x73\x65\x79\x31\x14\x30\x12\x06\x03\x55\x04\x07\x13\x0B\x4A\x65\x72\x73\x65\x79\x20\x43\x69\x74\x79\x31\x1E\x30\x1C\x06\x03\x55\x04\x0A\x13\x15\x54\x68\x65\x20\x55\x53\x45\x52\x54\x52\x55\x53\x54\x20\x4E\x65\x74\x77\x6F\x72\x6B\x31\x2E\x30\x2C\x06\x03\x55\x04\x03\x13\x25\x55\x53\x45\x52\x54\x72\x75\x73\x74\x20\x52\x53\x41\x20\x43\x65\x72\x74\x69\x66\x69\x63\x61\x74\x69\x6F\x6E\x20\x41\x75\x74\x68\x6F\x72\x69\x74\x79\x30\x82\x02\x22\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x01\x05\x00\x03\x82\x02\x0F\x00\x30\x82\x02\x0A\x02\x82\x02\x01\x00\x80\x12\x65\x17\x36\x0E\xC3\xDB\x08\xB3\xD0\xAC\x57\x0D\x76\xED\xCD\x27\xD3\x4C\xAD\x50\x83\x61\xE2\xAA\x20\x4D\x09\x2D\x64\x09\xDC\xCE\x89\x9F\xCC\x3D\xA9\xEC\xF6\xCF\xC1\xDC\xF1\xD3\xB1\xD6\x7B\x37\x28\x11\x2B\x47\xDA\x39\xC6\xBC\x3A\x19\xB4\x5F\xA6\xBD\x7D\x9D\xA3\x63\x42\xB6\x76\xF2\xA9\x3B\x2B\x91\xF8\xE2\x6F\xD0\xEC\x16\x20\x90\x09\x3E\xE2\xE8\x74\xC9\x18\xB4\x91\xD4\x62\x64\xDB\x7F\xA3\x06\xF1\x88\x18\x6A\x90\x22\x3C\xBC\xFE\x13\xF0\x87\x14\x7B\xF6\xE4\x1F\x8E\xD4\xE4\x51\xC6\x11\x67\x46\x08\x51\xCB\x86\x14\x54\x3F\xBC\x33\xFE\x7E\x6C\x9C\xFF\x16\x9D\x18\xBD\x51\x8E\x35\xA6\xA7\x66\xC8\x72\x67\xDB\x21\x66\xB1\xD4\x9B\x78\x03\xC0\x50\x3A\xE8\xCC\xF0\xDC\xBC\x9E\x4C\xFE\xAF\x05\x96\x35\x1F\x57\x5A\xB7\xFF\xCE\xF9\x3D\xB7\x2C\xB6\xF6\x54\xDD\xC8\xE7\x12\x3A\x4D\xAE\x4C\x8A\xB7\x5C\x9A\xB4\xB7\x20\x3D\xCA\x7F\x22\x34\xAE\x7E\x3B\x68\x66\x01\x44\xE7\x01\x4E\x46\x53\x9B\x33\x60\xF7\x94\xBE\x53\x37\x90\x73\x43\xF3\x32\xC3\x53\xEF\xDB\xAA\xFE\x74\x4E\x69\xC7\x6B\x8C\x60\x93\xDE\xC4\xC7\x0C\xDF\xE1\x32\xAE\xCC\x93\x3B\x51\x78\x95\x67\x8B\xEE\x3D\x56\xFE\x0C\xD0\x69\x0F\x1B\x0F\xF3\x25\x26\x6B\x33\x6D\xF7\x6E\x47\xFA\x73\x43\xE5\x7E\x0E\xA5\x66\xB1\x29\x7C\x32\x84\x63\x55\x89\xC4\x0D\xC1\x93\x54\x30\x19\x13\xAC\xD3\x7D\x37\xA7\xEB\x5D\x3A\x6C\x35\x5C\xDB\x41\xD7\x12\xDA\xA9\x49\x0B\xDF\xD8\x80\x8A\x09\x93\x62\x8E\xB5\x66\xCF\x25\x88\xCD\x84\xB8\xB1\x3F\xA4\x39\x0F\xD9\x02\x9E\xEB\x12\x4C\x95\x7C\xF3\x6B\x05\xA9\x5E\x16\x83\xCC\xB8\x67\xE2\xE8\x13\x9D\xCC\x5B\x82\xD3\x4C\xB3\xED\x5B\xFF\xDE\xE5\x73\xAC\x23\x3B\x2D\x00\xBF\x35\x55\x74\x09\x49\xD8\x49\x58\x1A\x7F\x92\x36\xE6\x51\x92\x0E\xF3\x26\x7D\x1C\x4D\x17\xBC\xC9\xEC\x43\x26\xD0\xBF\x41\x5F\x40\xA9\x44\x44\xF4\x99\xE7\x57\x87\x9E\x50\x1F\x57\x54\xA8\x3E\xFD\x74\x63\x2F\xB1\x50\x65\x09\xE6\x58\x42\x2E\x43\x1A\x4C\xB4\xF0\x25\x47\x59\xFA\x04\x1E\x93\xD4\x26\x46\x4A\x50\x81\xB2\xDE\xBE\x78\xB7\xFC\x67\x15\xE1\xC9\x57\x84\x1E\x0F\x63\xD6\xE9\x62\xBA\xD6\x5F\x55\x2E\xEA\x5C\xC6\x28\x08\x04\x25\x39\xB8\x0E\x2B\xA9\xF2\x4C\x97\x1C\x07\x3F\x0D\x52\xF5\xED\xEF\x2F\x82\x0F\x02\x03\x01\x00\x01\xA3\x42\x30\x40\x30\x1D\x06\x03\x55\x1D\x0E\x04\x16\x04\x14\x53\x79\xBF\x5A\xAA\x2B\x4A\xCF\x54\x80\xE1\xD8\x9B\xC0\x9D\xF2\xB2\x03\x66\xCB\x30\x0E\x06\x03\x55\x1D\x0F\x01\x01\xFF\x04\x04\x03\x02\x01\x06\x30\x0F\x06\x03\x55\x1D\x13\x01\x01\xFF\x04\x05\x30\x03\x01\x01\xFF\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x0C\x05\x00\x03\x82\x02\x01\x00\x5C\xD4\x7C\x0D\xCF\xF7\x01\x7D\x41\x99\x65\x0C\x73\xC5\x52\x9F\xCB\xF8\xCF\x99\x06\x7F\x1B\xDA\x43\x15\x9F\x9E\x02\x55\x57\x96\x14\xF1\x52\x3C\x27\x87\x94\x28\xED\x1F\x3A\x01\x37\xA2\x76\xFC\x53\x50\xC0\x84\x9B\xC6\x6B\x4E\xBA\x8C\x21\x4F\xA2\x8E\x55\x62\x91\xF3\x69\x15\xD8\xBC\x88\xE3\xC4\xAA\x0B\xFD\xEF\xA8\xE9\x4B\x55\x2A\x06\x20\x6D\x55\x78\x29\x19\xEE\x5F\x30\x5C\x4B\x24\x11\x55\xFF\x24\x9A\x6E\x5E\x2A\x2B\xEE\x0B\x4D\x9F\x7F\xF7\x01\x38\x94\x14\x95\x43\x07\x09\xFB\x60\xA9\xEE\x1C\xAB\x12\x8C\xA0\x9A\x5E\xA7\x98\x6A\x59\x6D\x8B\x3F\x08\xFB\xC8\xD1\x45\xAF\x18\x15\x64\x90\x12\x0F\x73\x28\x2E\xC5\xE2\x24\x4E\xFC\x58\xEC\xF0\xF4\x45\xFE\x22\xB3\xEB\x2F\x8E\xD2\xD9\x45\x61\x05\xC1\x97\x6F\xA8\x76\x72\x8F\x8B\x8C\x36\xAF\xBF\x0D\x05\xCE\x71\x8D\xE6\xA6\x6F\x1F\x6C\xA6\x71\x62\xC5\xD8\xD0\x83\x72\x0C\xF1\x67\x11\x89\x0C\x9C\x13\x4C\x72\x34\xDF\xBC\xD5\x71\xDF\xAA\x71\xDD\xE1\xB9\x6C\x8C\x3C\x12\x5D\x65\xDA\xBD\x57\x12\xB6\x43\x6B\xFF\xE5\xDE\x4D\x66\x11\x51\xCF\x99\xAE\xEC\x17\xB6\xE8\x71\x91\x8C\xDE\x49\xFE\xDD\x35\x71\xA2\x15\x27\x94\x1C\xCF\x61\xE3\x26\xBB\x6F\xA3\x67\x25\x21\x5D\xE6\xDD\x1D\x0B\x2E\x68\x1B\x3B\x82\xAF\xEC\x83\x67\x85\xD4\x98\x51\x74\xB1\xB9\x99\x80\x89\xFF\x7F\x78\x19\x5C\x79\x4A\x60\x2E\x92\x40\xAE\x4C\x37\x2A\x2C\xC9\xC7\x62\xC8\x0E\x5D\xF7\x36\x5B\xCA\xE0\x25\x25\x01\xB4\xDD\x1A\x07\x9C\x77\x00\x3F\xD0\xDC\xD5\xEC\x3D\xD4\xFA\xBB\x3F\xCC\x85\xD6\x6F\x7F\xA9\x2D\xDF\xB9\x02\xF7\xF5\x97\x9A\xB5\x35\xDA\xC3\x67\xB0\x87\x4A\xA9\x28\x9E\x23\x8E\xFF\x5C\x27\x6B\xE1\xB0\x4F\xF3\x07\xEE\x00\x2E\xD4\x59\x87\xCB\x52\x41\x95\xEA\xF4\x47\xD7\xEE\x64\x41\x55\x7C\x8D\x59\x02\x95\xDD\x62\x9D\xC2\xB9\xEE\x5A\x28\x74\x84\xA5\x9B\xB7\x90\xC7\x0C\x07\xDF\xF5\x89\x36\x74\x32\xD6\x28\xC1\xB0\xB0\x0B\xE0\x9C\x4C\xC3\x1C\xD6\xFC\xE3\x69\xB5\x47\x46\x81\x2F\xA2\x82\xAB\xD3\x63\x44\x70\xC4\x8D\xFF\x2D\x33\xBA\xAD\x8F\x7B\xB5\x70\x88\xAE\x3E\x19\xCF\x40\x28\xD8\xFC\xC8\x90\xBB\x5D\x99\x22\xF5\x52\xE6\x58\xC5\x1F\x88\x31\x43\xEE\x88\x1D\xD7\xC6\x8E\x3C\x43\x6A\x1D\xA7\x18\xDE\x7D\x3D\x16\xF1\x62\xF9\xCA\x90\xA8\xFD";

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