/* Declare USB audio control functions. */

#ifndef __ADCUSER_H__
/* Definitions */
#define __ADCUSER_H__

/* Shared declarations */
extern uint32_t ADC_IF_GetRequest(void);
extern uint32_t ADC_IF_SetRequest(void);
extern uint32_t ADC_EP_GetRequest(void);
extern uint32_t ADC_EP_SetRequest(void);

#endif
