# ESP32自定义组件

## libfvad
语音活动检测，[参考](https://github.com/dpirch/libfvad)

## yos
跨平台代码，支持WIN32和ESP32
实现功能：http

## xmiot
小米物联网
- 小米账号登录
- 枚举设备
- 对小爱音箱发文本指令

## 示例代码

下面是一个调用 xmiot 实现小米 WiFi 音箱枚举设备，并发送命令的示例代码：

```c
#include "xmiot_account.h"
#include "xmiot_service.h"

static int _write_cb(void* arg, const char* key, const char* value){
    printf("key:%s, value:%s\n", key, value);
    return 0;
}
static int _read_cb(void* arg, const char* key, char* value, size_t vsize){
    // 根据_write_cb的实现
    printf("key:%s, TODO\n", key);
    return 0; 
}

void app_main(void) {

    // 设置小米账号登录信息
    const char *deviceid = "your_deviceid";
    const char *username = "your_username";
    const char *password = "your_password";
    void *json_result = NULL;

    // 执行登录
    int result = xmiot_account_login_auth(deviceid, username, password, _write_cb, NULL);

    if (result == 0) {
        printf("登录成功\n");

        // 创建服务上下文
        void *context = xmiot_service_context_create();

        // 加载配置
        result = xmiot_service_load_config(context, _read_cb, NULL);

        // 枚举设备，并获取did
        result = xmiot_service_get_speaker_did(context, _write_cb, NULL);
        
        if (result == 0) {
            // 发送命令打开客厅的灯
            const char *command = "打开客厅的灯";
            int cmd_result = xmiot_service_send_speaker_cmd(context, command);

            if (cmd_result == 0) {
                printf("命令发送成功\n");
            } else {
                printf("命令发送失败，错误码: %d\n", cmd_result);
            }
        } else {
            printf("未找到设备\n");
        }

        // 销毁服务上下文
        xmiot_service_context_destroy(context);
    } else {
        printf("登录失败，错误码: %d\n", result);
    }
}
```
