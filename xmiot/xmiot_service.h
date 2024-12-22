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
 * @brief xmiot service context
 */
typedef struct _xmiot_service_context_t xmiot_service_context_t;

/**
 * @brief create context
 *
 * @param read_cb
 * @param arg
 * @return int  \c 0 on success.

 */
xmiot_service_context_t* xmiot_service_create(
	int (*read_cb)(void* arg, const char* key, char* value, size_t vsize), void* arg);

/**
 * @brief config
 *
 * @param ctx      a context
 *
 * @return int  \c 0 on success.

 */
int xmiot_service_destory(xmiot_service_context_t* ctx);

/**
 * @brief Get did of wifispeaker
 *
 * @param ctx      a context
 * @param write_cb     Write configuration callback
 * @param arg          Callback Arguments
 * @return int  \c 0 on success.

 */
int xmiot_service_get_speaker_did(xmiot_service_context_t* ctx,
	int (*write_cb)(void* arg, const char* key, const char* value), void* arg);

/**
 * @brief Send wifispeaker commands
 *
 * @param ctx      a context
 * @param cmd      a speaker command
 * @return int  \c 0 on success.

 */
int xmiot_service_send_speaker_cmd(xmiot_service_context_t* ctx, const char* cmd);

#ifdef __cplusplus
}
#endif
