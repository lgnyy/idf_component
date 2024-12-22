/*
 * xmiot
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define XMIOT_SERVICE_ERR_INVALID_ARG -1
#define XMIOT_SERVICE_ERR_NO_MEM      -2
#define XMIOT_SERVICE_ERR_NO_CODE     -4


/**
 * @brief create context
 *
 * @param read_cb
 * @param arg
 * @return int  \c 0 on success.

 */
void* xmiot_service_context_create(void);

/**
 * @brief load config
 *
 * @param read_cb
 * @param arg
 * @return int  \c 0 on success.

 */
int xmiot_service_load_config(void* ctx,
	int (*read_cb)(void* arg, const char* key, char* value, size_t vsize), void* arg);

/**
 * @brief config
 *
 * @param ctx      a context
 *
 * @return int  \c 0 on success.

 */
int xmiot_service_context_destory(void* ctx);

/**
 * @brief Get did of wifispeaker
 *
 * @param ctx      a context
 * @param write_cb     Write configuration callback
 * @param arg          Callback Arguments
 * @return int  \c 0 on success.

 */
int xmiot_service_get_speaker_did(void* ctx,
	int (*write_cb)(void* arg, const char* key, const char* value), void* arg);

/**
 * @brief Send wifispeaker commands
 *
 * @param ctx      a context
 * @param cmd      a speaker command
 * @return int  \c 0 on success.

 */
int xmiot_service_send_speaker_cmd(void* ctx, const char* cmd);

#ifdef __cplusplus
}
#endif
