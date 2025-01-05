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


/**
 * @brief Loads the configuration for re-login.
 *
 * This function reads the device ID, username, and password hash from the provided
 * callback function and stores them in the context buffer.
 *
 * @param ctx Pointer to the context buffer where the configuration will be stored.
 *            The buffer should be large enough to hold the device ID (24 bytes),
 *            username (20 bytes), and password hash (36 bytes).
 * @param read_cb Callback function to read the configuration values. The callback
 *                should have the following signature:
 *                int read_cb(void* arg, const char* key, char* value, size_t vsize)
 *                - arg: User-defined argument passed to the callback.
 *                - key: The key of the configuration value to read.
 *                - value: Buffer to store the read value.
 *                - vsize: Size of the value buffer.
 * @param arg User-defined argument to pass to the callback function.
 *
 * @return Returns 0 on success, or an error code on failure.
 *         Possible error codes:
 *         - XMIOT_ACCOUNT_ERR_INVALID_ARG: If ctx or read_cb is NULL.
 */

int xmiot_account_relogin_load_config(void* ctx,
	int (*read_cb)(void* arg, const char* key, char* value, size_t vsize), void* arg);


/**
 * @brief Re-authenticates the account using the provided context and callback function.
 *
 * This function attempts to re-authenticate the account using the information provided
 * in the context. It uses a callback function to write key-value pairs during the
 * authentication process.
 *
 * @param ctx A pointer to the context containing authentication information.
 * @param write_cb A callback function used to write key-value pairs.
 * @param arg An argument to be passed to the callback function.
 *
 * @return Returns an error code indicating the result of the re-authentication process.
 *         - XMIOT_ACCOUNT_ERR_INVALID_ARG: If the context or callback function is invalid.
 *         - Other error codes as defined by the login_auth_internal function.
 */
int xmiot_account_relogin_auth(void* ctx,
	int (*write_cb)(void* arg, const char* key, const char* value), void* arg);

#ifdef __cplusplus
}
#endif
