/* Set USB device options. */

#ifndef __USBCFG_H__
/* Definitions */
#define __USBCFG_H__

#define USB_POWER 0
#define USB_IF_NUM 2
#define USB_EP_NUM 32
#define USB_MAX_PACKET0 64
#define USB_DMA 0
#define USB_DMA_EP 0x00000000

#define USB_POWER_EVENT 0
#define USB_RESET_EVENT 1
#define USB_SUSPEND_EVENT 0
#define USB_RESUME_EVENT 0
#define USB_WAKEUP_EVENT 0
#define USB_SOF_EVENT 1
#define USB_ERROR_EVENT 0
#define USB_EP_EVENT 0x0001
#define USB_CONFIGURE_EVENT 0
#define USB_INTERFACE_EVENT 0
#define USB_FEATURE_EVENT 0

#define USB_CLASS 1
#define USB_HID 0
#define USB_HID_IF_NUM 0
#define USB_MSC 0
#define USB_MSC_IF_NUM 0
#define USB_AUDIO 1
#define USB_ADC_CIF_NUM 0
#define USB_ADC_SIF1_NUM 1
#define USB_ADC_SIF2_NUM 1
#define USB_CDC 0
#define USB_CDC_CIF_NUM 0
#define USB_CDC_DIF_NUM 1
#define USB_CDC_BUFSIZE 64

#define USB_VENDOR 0

#endif
