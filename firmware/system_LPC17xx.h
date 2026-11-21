/* Declare processor clock functions. */

#ifndef __SYSTEM_LPC17xx_H
#define __SYSTEM_LPC17xx_H

#ifdef __cplusplus
/* Shared declarations */
extern "C" {
#endif

/* Includes */
#include <stdint.h>

extern uint32_t SystemFrequency;

extern void SystemInit(void);

extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif
