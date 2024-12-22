/*
 * xmiot
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XMIOT_ACCOUNT_ERR_INVALID_ARG -1
#define XMIOT_ACCOUNT_ERR_NO_MEM      -2
#define XMIOT_ACCOUNT_ERR_NO_START    -3
#define XMIOT_ACCOUNT_ERR_NO_CODE     -4
#define XMIOT_ACCOUNT_ERR_NO_TOKEN    -5


/**
 * @brief Log in with username and password
 *
 * @param deviceid
 * @param username
 * @param password
 * @param json_result     Output cJSON object
 * @return int  \c 0 on success.

 */
int xmiot_account_login(const char* deviceid, const char* username, const char* password, void** json_result);

/**
 * @brief Login and Authorization
 *
 * @param deviceid
 * @param username
 * @param password
 * @param write_cb     Write configuration callback
 * @param arg          Callback Arguments
 * @return int  \c 0 on success.
 *
 * @note mbedtls_md.
 */
int xmiot_account_login_auth(const char* deviceid, const char* username, const char* password,
	int (*write_cb)(void* arg, const char* key, const char* value), void* arg);

#ifdef __cplusplus
}
#endif
