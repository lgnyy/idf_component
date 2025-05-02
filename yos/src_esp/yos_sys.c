#include "esp_log.h"
//#include "esp_rom.h"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "yos_sys.h"

//static const char *TAG = "SYS";


void yos_sys_delay(uint32_t ms)
{
	//esp_rom_delay_us(ms * 1000);
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void yos_sys_restart(void)
{
    esp_restart();
}