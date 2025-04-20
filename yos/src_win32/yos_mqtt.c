#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "mongoose.h"   // To build, run: cc main.c mongoose.c

#include "../include/yos_mqtt.h"


typedef struct yos_mqtt_context
{
	struct mg_mgr mgr;  // Declare event manager
	struct mg_connection* mqtt_c;
	yos_mqtt_event_handler_t event_handler; 
	void* user_ctx;
	HANDLE thread;
	uint32_t is_running : 1;
	uint32_t is_tls : 1;
}yos_mqtt_context_t;

typedef struct _yos_mqtt_event
{
	int event_id;
	yos_mqtt_context_t* client;
	const char* data;
	int data_len;
	const char* topic;
	int topic_len;
}yos_mqtt_event_t;

// MQTT client event handler function
static void fn(struct mg_connection* c, int ev, void* ev_data) {
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)(c->mgr);
	if (ev == MG_EV_OPEN) {
		MG_INFO(("%lu CREATED", c->id));
	}
	else if (ev == MG_EV_CONNECT) {
		if (ctx->is_tls) {
			struct mg_tls_opts opts = {NULL}; //  {.ca = mg_unpacked("/certs/ca.pem"), .name = mg_url_host(s_url) };
			mg_tls_init(c, &opts);
		}
	}
	else if (ev == MG_EV_CLOSE) {
		ctx->mqtt_c = NULL;
		MG_INFO(("%lu CLOSE", c->id));
	}
	else if (ev == MG_EV_ERROR) {
		yos_mqtt_event_t yevent = { .event_id = MG_EV_ERROR, .client= ctx, .data = (char*)ev_data };
		ctx->event_handler(&yevent);
	}
	else if (ev == MG_EV_MQTT_OPEN) {
		yos_mqtt_event_t yevent = { .event_id = MG_EV_MQTT_OPEN, .client = ctx };
		ctx->event_handler(&yevent);
		MG_INFO(("%lu CONNECTED", c->id));
	}
	else if (ev == MG_EV_MQTT_MSG) {
		struct mg_mqtt_message* mm = (struct mg_mqtt_message*)ev_data;
		yos_mqtt_event_t yevent = { .event_id = MG_EV_MQTT_MSG, .client = ctx, .data= mm->data.buf, .data_len = mm->data.len, .topic = mm->topic.buf, .topic_len = mm->topic.len };
		ctx->event_handler(&yevent);
		MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, (int)mm->data.len, mm->data.buf, (int)mm->topic.len, mm->topic.buf));
	}
	//else if (ev == MG_EV_MQTT_CMD) {
	//	struct mg_mqtt_message* mm = (struct mg_mqtt_message*)ev_data;
	//	//mm->cmd // MQTT_CMD_PINGREQ
	//}
	//else if (ev == MG_EV_CLOSE) {
	//	MG_INFO(("%lu CLOSED", c->id));
	//}
}

static void timer_fn(void* arg) {
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)arg;
	if (ctx->mqtt_c != NULL) {
		mg_mqtt_ping(ctx->mqtt_c);
	}
}

static DWORD WINAPI _thread_func(LPVOID lpThreadParameter){
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)lpThreadParameter;
	ctx->is_running = 1;
	while(ctx->is_running) {          // Run an infinite event loop
		mg_mgr_poll(&(ctx->mgr), 6000);
	}
	return 0;
}

yos_mqtt_handle_t yos_mqtt_create(yos_mqtt_event_handler_t handler, void* udata)
{
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)malloc(sizeof(*ctx));
	if (ctx == NULL) {
		return NULL;
	}

	mg_mgr_init(&(ctx->mgr));  // Initialise event manager
	mg_timer_add(&(ctx->mgr), 60000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, ctx);

	ctx->mqtt_c = NULL;
	ctx->event_handler = handler;
	ctx->user_ctx = udata;
	ctx->thread = NULL;
	return ctx;
}

int32_t yos_mqtt_destory(yos_mqtt_handle_t client)
{
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)client;
	if (ctx != NULL) {
		if (ctx->thread != NULL) {
			mg_wakeup(&(ctx->mgr), ctx->mqtt_c->id, "close", 5);
			WaitForSingleObject(ctx->thread, 10000);
		}

		mg_mgr_free(&(ctx->mgr));
		free(ctx);
	}

	return 0;
}


yos_mqtt_handle_t yos_mqtt_connect(const char* uri, const char* ca_cert, const char* client_id, const char* username, const char* password)
{
	yos_mqtt_handle_t client = yos_mqtt_create(NULL, NULL);

	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)client;
	if ((ctx == NULL) || (ctx->thread != NULL)) {
		return NULL;
	}

	struct mg_mqtt_opts opts = { .client_id = mg_str(client_id), .user = mg_str(username), .pass = mg_str(password), .clean = true, .version = 4 };
	ctx->is_tls = mg_url_is_ssl(uri);
	ctx->mqtt_c = mg_mqtt_connect(&(ctx->mgr), uri, &opts, fn, NULL);
#if 1
	ctx->thread = CreateThread(NULL, 0, _thread_func, client, 0, NULL);
#else // test
	while (1) {
		mg_mgr_poll(&(ctx->mgr), 6000);
	}
#endif
	return client;
}

int32_t yos_mqtt_register_event_handler(yos_mqtt_handle_t client, yos_mqtt_event_handler_t handler)
{
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)client;
	ctx->event_handler = handler;
	return 0;
}

int32_t yos_mqtt_disconnect(yos_mqtt_handle_t client)
{
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)client;
	if (ctx) {
		if (ctx->mqtt_c) {
			mg_mqtt_disconnect(ctx->mqtt_c, NULL);
		}

		if (ctx->thread) {
			ctx->is_running = 0;
			WaitForSingleObject(ctx->thread, 10000);
			ctx->thread = NULL;
		}

		yos_mqtt_destory(client);
	}
	return 0;
}

int yos_mqtt_event_get_id(void* ev)
{
	return ((yos_mqtt_event_t*)ev)->event_id;
}
yos_mqtt_handle_t yos_mqtt_event_get_instance(void* ev)
{
	return ((yos_mqtt_event_t*)ev)->client;
}
bool yos_mqtt_event_is_connected(void* ev)
{
	return ((yos_mqtt_event_t*)ev)->event_id == MG_EV_MQTT_OPEN;
}
bool yos_mqtt_event_is_msg(void* ev)
{
	return ((yos_mqtt_event_t*)ev)->event_id == MG_EV_MQTT_MSG;
}
char* yos_mqtt_event_get_data(void* ev, int* data_len)
{
	if (data_len != NULL) {
		*data_len = ((yos_mqtt_event_t*)ev)->data_len;
	}
	return (char*)((yos_mqtt_event_t*)ev)->data;
}
char* yos_mqtt_event_get_topic(void* ev, int* topic_len)
{
	if (topic_len != NULL) {
		*topic_len = ((yos_mqtt_event_t*)ev)->topic_len;
	}
	return (char*)((yos_mqtt_event_t*)ev)->topic;
}


int32_t yos_mqtt_subscribe_multiple(yos_mqtt_handle_t client, const yos_mqtt_topic_t* topic_list, int size)
{
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)client;
	for (int i = 0; i < size; i++) {
		struct mg_mqtt_opts sub_opts = { .topic = mg_str(topic_list[i].filter), .qos = topic_list[i].qos };
		mg_mqtt_sub(ctx->mqtt_c, &sub_opts);
	}
	return 0;
}

int32_t yos_mqtt_unsubscribe(yos_mqtt_handle_t client, const char* topic)
{
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)client;
	return -1;
}

int32_t yos_mqtt_publish(yos_mqtt_handle_t client, const char* topic, const char* data, int len, int qos, int retain)
{
	yos_mqtt_context_t* ctx = (yos_mqtt_context_t*)client;
	struct mg_mqtt_opts pub_opts = {.topic = mg_str(topic), .message = mg_str_n(data, len), .qos = qos, .retain = retain};
	mg_mqtt_pub(ctx->mqtt_c, &pub_opts);
	return 0;
}
