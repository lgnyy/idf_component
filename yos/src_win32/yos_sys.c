#include <windows.h>
#include "yos_sys.h"

//static const char *TAG = "SYS";


void yos_sys_delay(uint32_t ms)
{
    Sleep(ms);
}

void yos_sys_restart(void)
{
}