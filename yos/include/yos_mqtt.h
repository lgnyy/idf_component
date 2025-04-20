/*
 * yos_mqtt.h
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** MQTT instance handle */
typedef void * yos_mqtt_handle_t;

typedef struct _yos_mqtt_topic_t {
    const char* filter;  /*!< Topic filter  to subscribe */
    int qos; /*!< Max QoS level of the subscription */
} yos_mqtt_topic_t;


typedef int32_t(*yos_mqtt_event_handler_t)(void* ev);

yos_mqtt_handle_t yos_mqtt_connect(const char* uri, const char* ca_cert, const char* client_id, const char* username, const char* password);
int32_t yos_mqtt_register_event_handler(yos_mqtt_handle_t client, yos_mqtt_event_handler_t handler);
int32_t yos_mqtt_disconnect(yos_mqtt_handle_t client);

int yos_mqtt_event_get_id(void* ev);
yos_mqtt_handle_t yos_mqtt_event_get_instance(void* ev);
bool yos_mqtt_event_is_connected(void* ev);
bool yos_mqtt_event_is_msg(void* ev);
char* yos_mqtt_event_get_data(void* ev, int* data_len);
char* yos_mqtt_event_get_topic(void* ev, int* topic_len);

int32_t yos_mqtt_subscribe_multiple(yos_mqtt_handle_t client, const yos_mqtt_topic_t* topic_list, int size);
int32_t yos_mqtt_unsubscribe(yos_mqtt_handle_t client, const char* topic);

int32_t yos_mqtt_publish(yos_mqtt_handle_t client, const char* topic, const char* data, int len, int qos, int retain);

#ifdef __cplusplus
}
#endif
