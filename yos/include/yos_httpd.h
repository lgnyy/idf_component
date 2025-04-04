/*
 * yos_httpd.h
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** HTTPD instance handle */
typedef void * yos_httpd_handle_t;

typedef int32_t(*yos_httpd_uri_handler_t)(void* req);


/**
 * @brief Create HTTP server instance
 *
 * @param [in] server_port     server port
 * @param [in] uri             uri path
 * @param [in] uri_handler     uri handler
 *
 * @return NULL failure/other success
 */
yos_httpd_handle_t yos_httpd_create(uint16_t server_port, const char* uri,  yos_httpd_uri_handler_t uri_handler);


/**
 * @brief Destroy client instance
 *
 * @param [in] server           server handle
 *
 * @return 0 success/other failure
 */
int32_t yos_httpd_destory(yos_httpd_handle_t server);


/**
 * @brief Retrieves the HTTP method of the given HTTP request.
 *
 * This function extracts and returns the HTTP method (e.g., "GET", "POST") 
 * from the provided HTTP request object.
 *
 * @param req A pointer to the HTTP request object.
 * @return A constant character pointer to the HTTP method string.
 *         The returned string is managed internally and should not be modified or freed.
 */
const char* yos_httpd_req_get_method(void* req);


/**
 * @brief Retrieves the URI of the given HTTP request.
 *
 * This function extracts and returns the URI from the provided HTTP request object.
 *
 * @param req A pointer to the HTTP request object.
 * @return A constant character pointer to the URI string.
 *         The returned string is managed internally and should not be modified or freed.
 */
const char* yos_httpd_req_get_uri(void* req);


/**
 * @brief Send response to client
 *
 * @param [in] req            request handle
 * @param [in] buf            response buffer
 * @param [in] buf_len        response buffer length
 *
 * @return 0 success/other failure
 */
int32_t yos_httpd_resp_send(void *req, const char *buf, uint32_t buf_len);


#ifdef __cplusplus
}
#endif
