
#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

void yos_sys_delay(uint32_t ms);
void yos_sys_restart(void);

#ifdef __cplusplus
}
#endif
