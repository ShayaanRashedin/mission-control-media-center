/* Declare USB control functions and state. */

#ifndef __USBCORE_H__
/* Definitions */
#define __USBCORE_H__

/* Data types */
typedef struct _USB_EP_DATA {
    uint8_t *pData;
    uint16_t Count;
} USB_EP_DATA;

/* Shared declarations */
extern uint16_t USB_DeviceStatus;
extern uint8_t USB_DeviceAddress;
extern uint8_t USB_Configuration;
extern uint32_t USB_EndPointMask;
extern uint32_t USB_EndPointHalt;
extern uint32_t USB_EndPointStall;
extern uint8_t USB_AltSetting[USB_IF_NUM];

extern uint8_t EP0Buf[USB_MAX_PACKET0];

extern USB_EP_DATA EP0Data;

extern USB_SETUP_PACKET SetupPacket;

extern void USB_ResetCore(void);

#endif
