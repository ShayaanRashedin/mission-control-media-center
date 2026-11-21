/* Declare USB descriptors. */

#ifndef __USBDESC_H__
/* Definitions */
#define __USBDESC_H__

#define WBVAL(x) ((x) & 0xFF), (((x) >> 8) & 0xFF)
#define B3VAL(x) ((x) & 0xFF), (((x) >> 8) & 0xFF), (((x) >> 16) & 0xFF)

#define USB_DEVICE_DESC_SIZE (sizeof(USB_DEVICE_DESCRIPTOR))
#define USB_CONFIGUARTION_DESC_SIZE (sizeof(USB_CONFIGURATION_DESCRIPTOR))
#define USB_INTERFACE_DESC_SIZE (sizeof(USB_INTERFACE_DESCRIPTOR))
#define USB_ENDPOINT_DESC_SIZE (sizeof(USB_ENDPOINT_DESCRIPTOR))

/* Shared declarations */
extern const uint8_t USB_DeviceDescriptor[];
extern const uint8_t USB_ConfigDescriptor[];
extern const uint8_t USB_StringDescriptor[];

#endif
