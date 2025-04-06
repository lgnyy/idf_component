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
yos_httpd_handle_t yos_httpd_create(uint16_t server_port);

/**
 * @brief Destroy client instance
 *
 * @param [in] server           server handle
 *
 * @return 0 success/other failure
 */
int32_t yos_httpd_destory(yos_httpd_handle_t server);


/**
 * @brief Registers a URI handler with the HTTP server.
 *
 * This function allows you to associate a specific URI with a handler function
 * that will be invoked when the URI is accessed. You can also pass user-defined
 * data to the handler.
 *
 * @param server The handle to the HTTP server instance.
 * @param uri The URI string to register. This should be a null-terminated string.
 * @param uri_handler The callback function to handle requests to the specified URI.
 * @param udata A pointer to user-defined data that will be passed to the handler.
 *
 * @return 
 *      - 0 on success.
 *      - A negative value on failure.
 */
int32_t yos_register_uri_handler(yos_httpd_handle_t server, const char* uri, yos_httpd_uri_handler_t uri_handler, void* udata);


/**
 * @brief Unregisters a URI handler from the HTTP server.
 *
 * This function removes a previously registered URI handler from the HTTP server.
 * It stops the server from invoking the specified handler for the given URI.
 *
 * @param server The handle to the HTTP server instance.
 * @param uri The URI string for which the handler is to be unregistered.
 * 
 * @return 
 *      - 0 on success.
 *      - Negative value on failure.
 */
int32_t yos_unregister_uri_handler(yos_httpd_handle_t server, const char* uri);


/**
 * @brief Waits for specific bits to be set in the HTTP server's event flags.
 *
 * This function blocks the calling thread until the specified bits are set in the
 * event flags of the HTTP server or the timeout period elapses.
 *
 * @param server The handle to the HTTP server instance.
 * @param value The bitmask specifying the bits to wait for.
 * @param ms The timeout period in milliseconds. If set to 0, the function will not block.
 * 
 * @return 
 *     - A positive value indicating the bits that were set.
 *     - 0 if the timeout period elapsed without the specified bits being set.
 *     - A negative value if an error occurred.
 */
int32_t yos_httpd_wait_bits(yos_httpd_handle_t server, uint32_t value, uint32_t ms);


/**
 * @brief Sets specific bits in the HTTP server configuration.
 *
 * This function allows modifying the configuration of the HTTP server
 * by setting specific bits in the provided value.
 *
 * @param server The handle to the HTTP server instance.
 * @param value  The bitmask value to set in the server configuration.
 *
 * @return A 32-bit integer indicating the result of the operation.
 *         Typically, 0 indicates success, while a negative value
 *         indicates an error.
 */
int32_t yos_httpd_set_bits(yos_httpd_handle_t server, uint32_t value);


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
 * @brief Retrieves the HTTP server handle associated with a given request.
 *
 * This function extracts and returns the handle of the HTTP server
 * that is managing the specified request.
 *
 * @param req A pointer to the HTTP request object.
 * @return The handle of the HTTP server managing the request.
 */
yos_httpd_handle_t yos_httpd_req_get_handle(void* req);


/**
 * @brief Retrieves the user data associated with the given HTTP request.
 *
 * This function extracts and returns the user data pointer from the provided HTTP request object.
 *
 * @param req A pointer to the HTTP request object.
 * @return A void pointer to the user data associated with the request.
 *         The returned pointer is managed internally and should not be modified or freed.
 */
void* yos_httpd_req_get_udata(void* req);

/**
 * @brief Retrieves the URI of the given HTTP request.
 *
 * This function extracts and returns the URI from the provided HTTP request object.
 *
 * @param req A pointer to the HTTP request object.
 * @return A constant character pointer to the URI string.
 *         The returned string is managed internally and should not be modified or freed.
 */
const char* yos_httpd_req_get_uri(void* req, uint32_t* out_len);


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
