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
int32_t yos_httpd_register_uri_handler(yos_httpd_handle_t server, const char* uri, yos_httpd_uri_handler_t uri_handler, void* udata);


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
int32_t yos_httpd_unregister_uri_handler(yos_httpd_handle_t server, const char* uri);


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
 * @brief Retrieves the body of an HTTP request.
 *
 * This function extracts the body content of an HTTP request and returns it as a string.
 * The length of the body is also provided through the output parameter.
 *
 * @param req A pointer to the HTTP request object.
 * @param[out] out_len A pointer to a uint32_t variable where the length of the request body will be stored.
 * @return A pointer to the request body as a null-terminated string. 
 *         Returns NULL if the body is not available or an error occurs.
 */
char* yos_httpd_req_recv_body(void* req, uint32_t* out_len);


/**
 * @brief Frees the memory allocated for the HTTP request body.
 *
 * This function is used to release the memory associated with the HTTP request
 * body that was previously allocated. It is important to call this function
 * to avoid memory leaks after the request body is no longer needed.
 *
 * @param body A pointer to the memory holding the HTTP request body to be freed.
 *             This pointer must have been allocated dynamically.
 */
void yos_httpd_req_body_free(void *req, char* body);


/**
 * @brief Sets a header field and its value in the HTTP response.
 *
 * This function allows you to add or modify a header field in the HTTP response.
 * It is typically used to specify additional metadata or control information
 * for the HTTP response.
 *
 * @param r Pointer to the HTTP request object.
 * @param field The name of the header field to set (e.g., "Content-Type").
 * @param value The value to assign to the header field (e.g., "application/json").
 *
 * @return
 *  - 0 on success.
 *  - Negative value on failure.
 */
int32_t yos_httpd_resp_set_hdr(void *req, const char *field, const char *value);


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

/**
 * @brief Sends a file as an HTTP response.
 *
 * This function is used to send the contents of a specified file as the
 * response to an HTTP request.
 *
 * @param req A pointer to the HTTP request object.
 * @param fname The path to the file to be sent as the response.
 * 
 * @return 
 *      - 0 on success.
 *      - A negative value indicating an error code on failure.
 */
int32_t yos_httpd_resp_send_file(void* req, const char* fname);


#ifdef __cplusplus
}
#endif
