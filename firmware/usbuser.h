/* Declare USB event handlers. */

#ifndef __USBUSER_H__
/* Definitions */
#define __USBUSER_H__

/* Shared declarations */
extern void USB_Power_Event(uint32_t power);
extern void USB_Reset_Event(void);
extern void USB_Suspend_Event(void);
extern void USB_Resume_Event(void);
extern void USB_WakeUp_Event(void);
extern void USB_SOF_Event(void);
extern void USB_Error_Event(uint32_t error);

#define USB_EVT_SETUP 1
#define USB_EVT_OUT 2
#define USB_EVT_IN 3
#define USB_EVT_OUT_NAK 4
#define USB_EVT_IN_NAK 5
#define USB_EVT_OUT_STALL 6
#define USB_EVT_IN_STALL 7
#define USB_EVT_OUT_DMA_EOT 8
#define USB_EVT_IN_DMA_EOT 9
#define USB_EVT_OUT_DMA_NDR 10
#define USB_EVT_IN_DMA_NDR 11
#define USB_EVT_OUT_DMA_ERR 12
#define USB_EVT_IN_DMA_ERR 13

extern void (*const USB_P_EP[16])(uint32_t event);

extern void USB_EndPoint0(uint32_t event);
extern void USB_EndPoint1(uint32_t event);
extern void USB_EndPoint2(uint32_t event);
extern void USB_EndPoint3(uint32_t event);
extern void USB_EndPoint4(uint32_t event);
extern void USB_EndPoint5(uint32_t event);
extern void USB_EndPoint6(uint32_t event);
extern void USB_EndPoint7(uint32_t event);
extern void USB_EndPoint8(uint32_t event);
extern void USB_EndPoint9(uint32_t event);
extern void USB_EndPoint10(uint32_t event);
extern void USB_EndPoint11(uint32_t event);
extern void USB_EndPoint12(uint32_t event);
extern void USB_EndPoint13(uint32_t event);
extern void USB_EndPoint14(uint32_t event);
extern void USB_EndPoint15(uint32_t event);

extern void USB_Configure_Event(void);
extern void USB_Interface_Event(void);
extern void USB_Feature_Event(void);

#endif
