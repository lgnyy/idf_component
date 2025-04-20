
#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_event.h"
//#include "esp_tls.h"
#include "mqtt_client.h"

#include "../include/yos_mqtt.h"

static const char *TAG = "YOS_MQTT";


static void mqtt_event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
	ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, (int)event_id);
	if ((event_id == MQTT_EVENT_SUBSCRIBED) && (((esp_mqtt_event_handle_t)event_data)->error_handle)){
		ESP_LOGE(TAG, "Error event type: %d", (int)(((esp_mqtt_event_handle_t)event_data)->error_handle->error_type));
	}
	((yos_mqtt_event_handler_t)handler_args)(event_data);
}


yos_mqtt_handle_t yos_mqtt_connect(const char* uri, const char* ca_cert, const char* client_id, const char* username, const char* password)
{
	esp_mqtt_client_config_t mqtt_cfg = {
		.broker = {
			.address.uri = uri,
			.verification.use_global_ca_store = ca_cert == NULL,
			.verification.certificate = ca_cert,
			.verification.certificate_len = (ca_cert && (ca_cert[0]==0x30) && (ca_cert[1]==0x82))? (4+((uint8_t)(ca_cert[2]) << 8 | (uint8_t)(ca_cert[3]))) : 0,
			//.verification.skip_cert_common_name_check = true,
		},
		.credentials.client_id = client_id, 
		.credentials.username = username, 
		.credentials.authentication.password = password,
	};

	esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
	esp_err_t ret = esp_mqtt_client_start(client);
	ESP_LOGI(TAG, "esp_mqtt_client_start: %d", (int)ret); 
	return client;
}

int32_t yos_mqtt_register_event_handler(yos_mqtt_handle_t client, yos_mqtt_event_handler_t handler)
{
	return esp_mqtt_client_register_event((esp_mqtt_client_handle_t)client, ESP_EVENT_ANY_ID, mqtt_event_handler, handler);
}

int32_t yos_mqtt_disconnect(yos_mqtt_handle_t client)
{
	esp_mqtt_client_disconnect((esp_mqtt_client_handle_t)client);
	esp_err_t ret = esp_mqtt_client_stop((esp_mqtt_client_handle_t)client);
	ESP_LOGI(TAG, "esp_mqtt_client_stop: %d", (int)ret); 
	esp_mqtt_client_destroy((esp_mqtt_client_handle_t)client);
	return 0;
}

int yos_mqtt_event_get_id(void* ev)
{
	return ((esp_mqtt_event_handle_t)ev)->event_id;
}

yos_mqtt_handle_t yos_mqtt_event_get_instance(void* ev)
{
	return ((esp_mqtt_event_handle_t)ev)->client;
}

bool yos_mqtt_event_is_connected(void* ev)
{
	return ((esp_mqtt_event_handle_t)ev)->event_id == MQTT_EVENT_CONNECTED;
}

bool yos_mqtt_event_is_msg(void* ev)
{
	return ((esp_mqtt_event_handle_t)ev)->event_id == MQTT_EVENT_DATA;
}

char* yos_mqtt_event_get_data(void* ev, int* data_len)
{
	if (data_len != NULL) {
		*data_len = ((esp_mqtt_event_handle_t)ev)->data_len;
	}
	return ((esp_mqtt_event_handle_t)ev)->data;	
}

char* yos_mqtt_event_get_topic(void* ev, int* topic_len)
{
	if (topic_len != NULL) {
		*topic_len = ((esp_mqtt_event_handle_t)ev)->topic_len;
	}
	return ((esp_mqtt_event_handle_t)ev)->topic;	
}


int32_t yos_mqtt_subscribe_multiple(yos_mqtt_handle_t client, const yos_mqtt_topic_t* topic_list, int size)
{
	return esp_mqtt_client_subscribe_multiple((esp_mqtt_client_handle_t)client, (const esp_mqtt_topic_t*)topic_list, size);
}

int32_t yos_mqtt_unsubscribe(yos_mqtt_handle_t client, const char* topic)
{
	return esp_mqtt_client_unsubscribe((esp_mqtt_client_handle_t)client, topic);
}

int32_t yos_mqtt_publish(yos_mqtt_handle_t client, const char* topic, const char* data, int len, int qos, int retain)
{
	return esp_mqtt_client_publish((esp_mqtt_client_handle_t)client, topic, data, len, qos, retain);
}


