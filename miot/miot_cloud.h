/*
 * miot
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIOT_CLOUD_ERR_INVALID_ARG -1
#define MIOT_CLOUD_ERR_NO_MEM      -2
#define MIOT_CLOUD_ERR_OUT_BUFF    -3
#define MIOT_CLOUD_ERR_NO_CODE     -4
#define MIOT_CLOUD_ERR_NO_TOKEN    -5

/**
 * @brief Generate an authorization URL for user authentication.
 *
 * This function generates an authorization URL based on the provided parameters.
 * The URL is typically used in OAuth 2.0 or similar authentication flows to allow
 * users to grant permissions to the application.
 *
 * @param redirect_url The URL to which the user will be redirected after authorization.
 * @param state        An optional state parameter to prevent CSRF attacks.
 * @param scope        A space-separated list of permissions the application is requesting.
 * @param skip_confirm A flag indicating whether to skip the user confirmation step.
 * @param output       A buffer to store the generated authorization URL.
 * @param max_out_len  The maximum length of the output buffer.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_gen_auth_url(const char* redirect_url, const char* state, const char* scope, int skip_confirm, char* output, size_t max_out_len);

/**
 * @brief Retrieve an access token using an authorization code.
 *
 * @param redirect_url The application's callback URL.
 * @param code         The authorization code received after user authorization.
 * @param resp_json    A pointer to store the JSON response containing the access token.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_get_access_token(const char* redirect_url, const char* code, void** resp_json);

/**
 * @brief Refresh an access token using a refresh token.
 *
 * @param redirect_url   The application's callback URL.
 * @param refresh_token  The refresh token used to obtain a new access token.
 * @param resp_json      A pointer to store the JSON response containing the new access token.
 *
 * @return int           Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_refresh_access_token(const char* redirect_url, const char* refresh_token, void** resp_json);

/**
 * @brief Retrieve an access token using an authorization code with a custom write callback.
 *
 * @param redirect_url The application's callback URL.
 * @param code         The authorization code received after user authorization.
 * @param write_cb     A callback function to handle key-value pairs in the response.
 * @param arg          A user-defined argument passed to the callback function.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_get_access_token_w(const char* redirect_url, const char* code,
    int (*write_cb)(void* arg, const char* key, const char* value), void* arg);

/**
 * @brief Refresh an access token using a refresh token with a custom write callback.
 *
 * @param redirect_url   The application's callback URL.
 * @param refresh_token  The refresh token used to obtain a new access token.
 * @param write_cb       A callback function to handle key-value pairs in the response.
 * @param arg            A user-defined argument passed to the callback function.
 *
 * @return int           Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_refresh_access_token_w(const char* redirect_url, const char* refresh_token,
    int (*write_cb)(void* arg, const char* key, const char* value), void* arg);

/**
 * @brief Structure representing a property parameter for a device.
 *
 * This structure is used to define a property of a device, including its
 * device ID (DID), service ID (SIID), property ID (PIID), and value.
 */
typedef struct miot_cloud_param_did {
    const char* did;  /**< Device ID. */
    int siid;         /**< Service ID. */
    int piid;         /**< Property ID. */
    const char* value;/**< Property value. */
} miot_cloud_param_did_t;

/**
 * @brief Retrieve a single property of a device.
 *
 * @param access_token The access token for authentication.
 * @param did          The device ID.
 * @param siid         The service ID.
 * @param piid         The property ID.
 * @param resp_json    A pointer to store the JSON response containing the property value.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_get_prop(const char* access_token, const char* did, int siid, int piid, void** resp_json);

/**
 * @brief Retrieve multiple properties of devices.
 *
 * @param access_token The access token for authentication.
 * @param param_dids   An array of property parameters for the devices.
 * @param count        The number of properties to retrieve.
 * @param resp_json    A pointer to store the JSON response containing the property values.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_get_props(const char* access_token, const miot_cloud_param_did_t* param_dids, uint32_t count, void** resp_json);

/**
 * @brief Set a single property of a device.
 *
 * @param access_token The access token for authentication.
 * @param did          The device ID.
 * @param siid         The service ID.
 * @param piid         The property ID.
 * @param value        The value to set for the property.
 * @param resp_json    A pointer to store the JSON response.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_set_prop(const char* access_token, const char* did, int siid, int piid, const char* value, void** resp_json);

/**
 * @brief Set multiple properties of devices.
 *
 * @param access_token The access token for authentication.
 * @param param_dids   An array of property parameters for the devices.
 * @param count        The number of properties to set.
 * @param resp_json    A pointer to store the JSON response.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_set_props(const char* access_token, const miot_cloud_param_did_t* param_dids, uint32_t count, void** resp_json);

/**
 * @brief Perform an action on a device.
 *
 * @param access_token The access token for authentication.
 * @param did          The device ID.
 * @param siid         The service ID.
 * @param aiid         The action ID.
 * @param in_list      The input parameters for the action in JSON format.
 * @param resp_json    A pointer to store the JSON response.
 *
 * @return int         Returns 0 on success, or a negative error code on failure.
 */
int miot_cloud_action(const char* access_token, const char* did, int siid, int aiid, const char* in_list, void** resp_json);

#ifdef __cplusplus
}
#endif
