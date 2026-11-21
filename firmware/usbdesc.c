/* Store the USB device and audio descriptors. */

/* Includes */
#include "type.h"

#include "usb.h"
#include "audio.h"
#include "usbcfg.h"
#include "usbdesc.h"

const uint8_t USB_DeviceDescriptor[] = {USB_DEVICE_DESC_SIZE,
                                        USB_DEVICE_DESCRIPTOR_TYPE,
                                        WBVAL(0x0200),
                                        0x00,
                                        0x00,
                                        0x00,
                                        USB_MAX_PACKET0,
                                        WBVAL(0x1FC9),
                                        WBVAL(0x4002),
                                        WBVAL(0x0100),
                                        0x01,
                                        0x02,
                                        0x03,
                                        0x01};

const uint8_t USB_ConfigDescriptor[] = {

    USB_CONFIGUARTION_DESC_SIZE,
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    WBVAL(USB_CONFIGUARTION_DESC_SIZE + USB_INTERFACE_DESC_SIZE +
          AUDIO_CONTROL_INTERFACE_DESC_SZ(1) + AUDIO_INPUT_TERMINAL_DESC_SIZE +
          AUDIO_FEATURE_UNIT_DESC_SZ(1, 1) + AUDIO_OUTPUT_TERMINAL_DESC_SIZE +
          USB_INTERFACE_DESC_SIZE + USB_INTERFACE_DESC_SIZE + AUDIO_STREAMING_INTERFACE_DESC_SIZE +
          AUDIO_FORMAT_TYPE_I_DESC_SZ(1) + AUDIO_STANDARD_ENDPOINT_DESC_SIZE +
          AUDIO_STREAMING_ENDPOINT_DESC_SIZE),
    0x02,
    0x01,
    0x00,
    USB_CONFIG_BUS_POWERED,
    USB_CONFIG_POWER_MA(100),

    USB_INTERFACE_DESC_SIZE,
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x00,
    0x00,
    0x00,
    USB_DEVICE_CLASS_AUDIO,
    AUDIO_SUBCLASS_AUDIOCONTROL,
    AUDIO_PROTOCOL_UNDEFINED,
    0x00,

    AUDIO_CONTROL_INTERFACE_DESC_SZ(1),
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,
    AUDIO_CONTROL_HEADER,
    WBVAL(0x0100),
    WBVAL(AUDIO_CONTROL_INTERFACE_DESC_SZ(1) + AUDIO_INPUT_TERMINAL_DESC_SIZE +
          AUDIO_FEATURE_UNIT_DESC_SZ(1, 1) + AUDIO_OUTPUT_TERMINAL_DESC_SIZE),
    0x01,
    0x01,

    AUDIO_INPUT_TERMINAL_DESC_SIZE,
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,
    AUDIO_CONTROL_INPUT_TERMINAL,
    0x01,
    WBVAL(AUDIO_TERMINAL_USB_STREAMING),
    0x00,
    0x01,
    WBVAL(AUDIO_CHANNEL_M),
    0x00,
    0x00,

    AUDIO_FEATURE_UNIT_DESC_SZ(1, 1),
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,
    AUDIO_CONTROL_FEATURE_UNIT,
    0x02,
    0x01,
    0x01,
    AUDIO_CONTROL_MUTE | AUDIO_CONTROL_VOLUME,
    0x00,
    0x00,

    AUDIO_OUTPUT_TERMINAL_DESC_SIZE,
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,
    AUDIO_CONTROL_OUTPUT_TERMINAL,
    0x03,
    WBVAL(AUDIO_TERMINAL_SPEAKER),
    0x00,
    0x02,
    0x00,

    USB_INTERFACE_DESC_SIZE,
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x01,
    0x00,
    0x00,
    USB_DEVICE_CLASS_AUDIO,
    AUDIO_SUBCLASS_AUDIOSTREAMING,
    AUDIO_PROTOCOL_UNDEFINED,
    0x00,

    USB_INTERFACE_DESC_SIZE,
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x01,
    0x01,
    0x01,
    USB_DEVICE_CLASS_AUDIO,
    AUDIO_SUBCLASS_AUDIOSTREAMING,
    AUDIO_PROTOCOL_UNDEFINED,
    0x00,

    AUDIO_STREAMING_INTERFACE_DESC_SIZE,
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,
    AUDIO_STREAMING_GENERAL,
    0x01,
    0x01,
    WBVAL(AUDIO_FORMAT_PCM),

    AUDIO_FORMAT_TYPE_I_DESC_SZ(1),
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,
    AUDIO_STREAMING_FORMAT_TYPE,
    AUDIO_FORMAT_TYPE_I,
    0x01,
    0x02,
    16,
    0x01,
    B3VAL(32000),

    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_OUT(3),
    USB_ENDPOINT_TYPE_ISOCHRONOUS,
    WBVAL(64),
    0x01,
    0x00,
    0x00,

    AUDIO_STREAMING_ENDPOINT_DESC_SIZE,
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,
    AUDIO_ENDPOINT_GENERAL,
    0x00,
    0x00,
    WBVAL(0x0000),

    0};

const uint8_t USB_StringDescriptor[] = {

    0x04,          USB_STRING_DESCRIPTOR_TYPE,
    WBVAL(0x0409),

    (13 * 2 + 2),  USB_STRING_DESCRIPTOR_TYPE,
    'N',           0,
    'X',           0,
    'P',           0,
    ' ',           0,
    'S',           0,
    'e',           0,
    'm',           0,
    'i',           0,
    'c',           0,
    'o',           0,
    'n',           0,
    'd',           0,
    ' ',           0,

    (20 * 2 + 2),  USB_STRING_DESCRIPTOR_TYPE,
    'N',           0,
    'X',           0,
    'P',           0,
    ' ',           0,
    'L',           0,
    'P',           0,
    'C',           0,
    '1',           0,
    '7',           0,
    'x',           0,
    'x',           0,
    ' ',           0,
    'S',           0,
    'p',           0,
    'e',           0,
    'a',           0,
    'k',           0,
    'e',           0,
    'r',           0,
    ' ',           0,

    (12 * 2 + 2),  USB_STRING_DESCRIPTOR_TYPE,
    'D',           0,
    'E',           0,
    'M',           0,
    'O',           0,
    '0',           0,
    '0',           0,
    '0',           0,
    '0',           0,
    '0',           0,
    '0',           0,
    '0',           0,
    '0',           0,
};
