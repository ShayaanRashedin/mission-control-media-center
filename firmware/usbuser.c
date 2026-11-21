/* Handle USB events and receive audio samples. */

/* Includes */
#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"

#include "usbaudio.h"

#if USB_POWER_EVENT
/* USB Power Event */
void USB_Power_Event(uint32_t power) {}
#endif

#if USB_RESET_EVENT
/* USB Reset Event */
void USB_Reset_Event(void) {
    DataRun = 0;
    DataIn = DataOut = 0;
    USB_ResetCore();
}
#endif

#if USB_SUSPEND_EVENT
/* USB Suspend Event */
void USB_Suspend_Event(void) {}
#endif

#if USB_RESUME_EVENT
/* USB Resume Event */
void USB_Resume_Event(void) {}
#endif

#if USB_WAKEUP_EVENT
/* USB Wake Up Event */
void USB_WakeUp_Event(void) {}
#endif

#if USB_SOF_EVENT

/* USB SOF Event */
/* Receive audio data for this USB frame. */

void USB_SOF_Event(void) {
    static short packet[32];
    unsigned bytes, count, i, in, free_samples;
    if (!USB_Configuration || USB_AltSetting[1] != 1) {
        DataRun = 0;
        DataIn = DataOut;
        return;
    }
    bytes = USB_ReadEP(3, (uint8_t *)packet);
    if (!bytes || bytes > 64 || (bytes & 1))
        return;
    count = bytes / 2;
    in = DataIn;
    free_samples = (DataOut - in - 1) & (B_S - 1);
    if (free_samples < count)
        return;
    for (i = 0; i < count; i++) {
        DataBuf[in] = packet[i];
        in = (in + 1) & (B_S - 1);
    }
    DataIn = in;
    if (((DataIn - DataOut) & (B_S - 1)) >= B_S / 2)
        DataRun = 1;
}

#endif

#if USB_ERROR_EVENT
/* USB Error Event */
void USB_Error_Event(uint32_t error) {}
#endif

#if USB_CONFIGURE_EVENT
/* USB Configure Event */
void USB_Configure_Event(void) {

    if (USB_Configuration) {
    }
}
#endif

#if USB_INTERFACE_EVENT
/* USB Interface Event */
void USB_Interface_Event(void) {}
#endif

#if USB_FEATURE_EVENT
/* USB Feature Event */
void USB_Feature_Event(void) {}
#endif

#define P_EP(n) ((USB_EP_EVENT & (1 << (n))) ? USB_EndPoint##n : NULL)

void (*const USB_P_EP[16])(uint32_t event) = {
    P_EP(0), P_EP(1), P_EP(2),  P_EP(3),  P_EP(4),  P_EP(5),  P_EP(6),  P_EP(7),
    P_EP(8), P_EP(9), P_EP(10), P_EP(11), P_EP(12), P_EP(13), P_EP(14), P_EP(15),
};

/* USB End Point1 */
void USB_EndPoint1(uint32_t event) {}

/* USB End Point2 */
void USB_EndPoint2(uint32_t event) {}

/* USB End Point3 */
void USB_EndPoint3(uint32_t event) {
#if USB_DMA
    USB_DMA_DESCRIPTOR DD;

    if (event & USB_EVT_OUT_DMA_EOT) {

        if (USB_DMA_BufAdr(0x03) != ((uint32_t)DataBuf + 2 * DataIn)) {

            DataIn += P_C * P_S;
            DataIn &= B_S - 1;
            if (((DataIn - DataOut) & (B_S - 1)) == (B_S / 2)) {
                DataRun = 1;
            }
        } else {

            DataRun = 0;
            DataOut = DataIn;
        }
    }
    if (event & (USB_EVT_OUT_DMA_EOT | USB_EVT_OUT_DMA_NDR)) {

        DD.BufAdr = (uint32_t)DataBuf + 2 * DataIn;
        DD.BufLen = P_C;
        DD.MaxSize = 0;
        DD.InfoAdr = (uint32_t)InfoBuf;
        DD.Cfg.Val = 0;
        DD.Cfg.Type.IsoEP = 1;
        USB_DMA_Setup(0x03, &DD);
        USB_DMA_Enable(0x03);
    }
#else
    event = event;
#endif
}

/* USB End Point4 */
void USB_EndPoint4(uint32_t event) {}

/* USB End Point5 */
void USB_EndPoint5(uint32_t event) {}

/* USB End Point6 */
void USB_EndPoint6(uint32_t event) {}

/* USB End Point7 */
void USB_EndPoint7(uint32_t event) {}

/* USB End Point8 */
void USB_EndPoint8(uint32_t event) {}

/* USB End Point9 */
void USB_EndPoint9(uint32_t event) {}

/* USB End Point10 */
void USB_EndPoint10(uint32_t event) {}

/* USB End Point11 */
void USB_EndPoint11(uint32_t event) {}

/* USB End Point12 */
void USB_EndPoint12(uint32_t event) {}

/* USB End Point13 */
void USB_EndPoint13(uint32_t event) {}

/* USB End Point14 */
void USB_EndPoint14(uint32_t event) {}

/* USB End Point15 */
void USB_EndPoint15(uint32_t event) {}
