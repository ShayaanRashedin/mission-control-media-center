/* Handle USB mute and volume requests. */

/* Includes */
#include "type.h"

#include "usb.h"
#include "audio.h"
#include "usbcfg.h"
#include "usbcore.h"
#include "adcuser.h"

#include "usbaudio.h"

volatile uint16_t VolCur = 0x0000;
const uint16_t VolMin = 0xE200;
const uint16_t VolMax = 0x0000;
const uint16_t VolRes = 0x0100;

/* ADC IF Get Request */
uint32_t ADC_IF_GetRequest(void) {

    if (SetupPacket.wIndex.W == 0x0200) {

        if (SetupPacket.wValue.WB.L == 0) {

            switch (SetupPacket.wValue.WB.H) {
            case AUDIO_MUTE_CONTROL:
                switch (SetupPacket.bRequest) {
                case AUDIO_REQUEST_GET_CUR:
                    EP0Buf[0] = Mute;
                    return (TRUE);
                }
                break;
            case AUDIO_VOLUME_CONTROL:
                switch (SetupPacket.bRequest) {
                case AUDIO_REQUEST_GET_CUR:
                    *((__packed uint16_t *)EP0Buf) = VolCur;
                    return (TRUE);
                case AUDIO_REQUEST_GET_MIN:
                    *((__packed uint16_t *)EP0Buf) = VolMin;
                    return (TRUE);
                case AUDIO_REQUEST_GET_MAX:
                    *((__packed uint16_t *)EP0Buf) = VolMax;
                    return (TRUE);
                case AUDIO_REQUEST_GET_RES:
                    *((__packed uint16_t *)EP0Buf) = VolRes;
                    return (TRUE);
                }
                break;
            }
        }
    }
    return (FALSE);
}

/* ADC IF Set Request */
uint32_t ADC_IF_SetRequest(void) {

    if (SetupPacket.wIndex.W == 0x0200) {

        if (SetupPacket.wValue.WB.L == 0) {

            switch (SetupPacket.wValue.WB.H) {
            case AUDIO_MUTE_CONTROL:
                switch (SetupPacket.bRequest) {
                case AUDIO_REQUEST_SET_CUR:
                    if (SetupPacket.wLength != 1)
                        return FALSE;
                    Mute = EP0Buf[0] ? 1 : 0;
                    return (TRUE);
                }
                break;
            case AUDIO_VOLUME_CONTROL:
                switch (SetupPacket.bRequest) {
                case AUDIO_REQUEST_SET_CUR:
                    if (SetupPacket.wLength != 2)
                        return FALSE;
                    VolCur = *((__packed uint16_t *)EP0Buf);
                    if ((short)VolCur > 0)
                        VolCur = 0;
                    if ((short)VolCur < -7680)
                        VolCur = 0xE200;
                    return (TRUE);
                }
                break;
            }
        }
    }
    return (FALSE);
}

/* ADC EP Get Request */
uint32_t ADC_EP_GetRequest(void) {

    return (FALSE);
}

/* ADC EP Set Request */
uint32_t ADC_EP_SetRequest(void) {

    return (FALSE);
}
