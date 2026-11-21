/* Control USB hardware and handle interrupts. */

/* Includes */
#include "LPC17xx.h"
#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbreg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"

volatile unsigned USB_HWError;

/* USB hardware wait */
/* Wait for USB hardware with a timeout. */
static int usb_wait(const volatile uint32_t *reg, uint32_t mask, unsigned code) {
    unsigned count = 1000000;
    if (USB_HWError)
        return 0;
    while ((*reg & mask) != mask) {
        if (--count == 0) {
            USB_HWError = code;
            return 0;
        }
    }
    return 1;
}

#pragma diag_suppress 1441

#define EP_MSK_CTRL 0x0001
#define EP_MSK_BULK 0xC924
#define EP_MSK_INT 0x4492
#define EP_MSK_ISO 0x1248

#if USB_DMA

#pragma arm section zidata = "USB_RAM"
uint32_t UDCA[USB_EP_NUM];

uint32_t DD_NISO_Mem[4 * DD_NISO_CNT];
uint32_t DD_ISO_Mem[5 * DD_ISO_CNT];
#pragma arm section zidata
uint32_t udca[USB_EP_NUM];

uint32_t DDMemMap[2];

#endif

/* EPAdr */
uint32_t EPAdr(uint32_t EPNum) {
    uint32_t val;

    val = (EPNum & 0x0F) << 1;
    if (EPNum & 0x80) {
        val += 1;
    }
    return (val);
}

/* Wr Cmd */
void WrCmd(uint32_t cmd) {

    LPC_USB->DevIntClr = CCEMTY_INT;
    LPC_USB->CmdCode = cmd;
    if (!usb_wait(&LPC_USB->DevIntSt, CCEMTY_INT, 2))
        return;
}

/* Wr Cmd Dat */
void WrCmdDat(uint32_t cmd, uint32_t val) {

    LPC_USB->DevIntClr = CCEMTY_INT;
    LPC_USB->CmdCode = cmd;
    if (!usb_wait(&LPC_USB->DevIntSt, CCEMTY_INT, 2))
        return;
    LPC_USB->DevIntClr = CCEMTY_INT;
    LPC_USB->CmdCode = val;
    if (!usb_wait(&LPC_USB->DevIntSt, CCEMTY_INT, 2))
        return;
}

/* Wr Cmd EP */
void WrCmdEP(uint32_t EPNum, uint32_t cmd) {

    LPC_USB->DevIntClr = CCEMTY_INT;
    LPC_USB->CmdCode = CMD_SEL_EP(EPAdr(EPNum));
    if (!usb_wait(&LPC_USB->DevIntSt, CCEMTY_INT, 2))
        return;
    LPC_USB->DevIntClr = CCEMTY_INT;
    LPC_USB->CmdCode = cmd;
    if (!usb_wait(&LPC_USB->DevIntSt, CCEMTY_INT, 2))
        return;
}

/* Rd Cmd Dat */
uint32_t RdCmdDat(uint32_t cmd) {

    LPC_USB->DevIntClr = CCEMTY_INT | CDFULL_INT;
    LPC_USB->CmdCode = cmd;
    if (!usb_wait(&LPC_USB->DevIntSt, CDFULL_INT, 3))
        return 0;
    return (LPC_USB->CmdData);
}

/* USB Init */
void USB_Init(void) {

    LPC_PINCON->PINSEL1 &= ~((3 << 26) | (3 << 28));
    LPC_PINCON->PINSEL1 |= ((1 << 26) | (1 << 28));

    LPC_PINCON->PINSEL3 &= ~((3 << 4) | (3 << 28));
    LPC_PINCON->PINSEL3 |= ((1 << 4) | (2 << 28));

    LPC_PINCON->PINSEL4 &= ~((3 << 18));
    LPC_PINCON->PINSEL4 |= ((1 << 18));

    LPC_SC->PCONP |= (1UL << 31);

    LPC_USB->USBClkCtrl = 0x12;
    if (!usb_wait(&LPC_USB->USBClkSt, 0x12, 1))
        return;

    NVIC_DisableIRQ(USB_IRQn);

    USB_Reset();
    if (USB_HWError)
        return;
    USB_SetAddress(0);
}

/* USB Connect */
void USB_Connect(uint32_t con) {
    WrCmdDat(CMD_SET_DEV_STAT, DAT_WR_BYTE(con ? DEV_CON : 0));
}

/* USB Reset */
void USB_Reset(void) {
#if USB_DMA
    uint32_t n;
#endif

    LPC_USB->DevIntClr = EP_RLZED_INT;
    LPC_USB->ReEp = 3;
    LPC_USB->EpInd = 0;
    LPC_USB->MaxPSize = USB_MAX_PACKET0;
    if (!usb_wait(&LPC_USB->DevIntSt, EP_RLZED_INT, 4))
        return;
    LPC_USB->DevIntClr = EP_RLZED_INT;
    LPC_USB->EpInd = 1;
    LPC_USB->MaxPSize = USB_MAX_PACKET0;
    if (!usb_wait(&LPC_USB->DevIntSt, EP_RLZED_INT, 4))
        return;
    LPC_USB->DevIntClr = EP_RLZED_INT;

    LPC_USB->EpIntClr = 0xFFFFFFFF;
    LPC_USB->EpIntEn = 0xFFFFFFFF ^ USB_DMA_EP;
    LPC_USB->DevIntClr = 0xFFFFFFFF;
    LPC_USB->DevIntEn = DEV_STAT_INT | EP_SLOW_INT | (USB_SOF_EVENT ? FRAME_INT : 0) |
                        (USB_ERROR_EVENT ? ERR_INT : 0);

#if USB_DMA
    LPC_USB->UDCAH = USB_RAM_ADR;
    LPC_USB->DMARClr = 0xFFFFFFFF;
    LPC_USB->EpDMADis = 0xFFFFFFFF;
    LPC_USB->EpDMAEn = USB_DMA_EP;
    LPC_USB->EoTIntClr = 0xFFFFFFFF;
    LPC_USB->NDDRIntClr = 0xFFFFFFFF;
    LPC_USB->SysErrIntClr = 0xFFFFFFFF;
    LPC_USB->DMAIntEn = 0x00000007;
    DDMemMap[0] = 0x00000000;
    DDMemMap[1] = 0x00000000;
    for (n = 0; n < USB_EP_NUM; n++) {
        udca[n] = 0;
        UDCA[n] = 0;
    }
#endif
}

/* USB Suspend */
void USB_Suspend(void) {}

/* USB Resume */
void USB_Resume(void) {}

/* USB Wake Up */
void USB_WakeUp(void) {

    if (USB_DeviceStatus & USB_GETSTATUS_REMOTE_WAKEUP) {
        WrCmdDat(CMD_SET_DEV_STAT, DAT_WR_BYTE(DEV_CON));
    }
}

/* USB Wake Up Cfg */
void USB_WakeUpCfg(uint32_t cfg) {}

/* USB Set Address */
void USB_SetAddress(uint32_t adr) {
    WrCmdDat(CMD_SET_ADDR, DAT_WR_BYTE(DEV_EN | adr));
    WrCmdDat(CMD_SET_ADDR, DAT_WR_BYTE(DEV_EN | adr));
}

/* USB Configure */
void USB_Configure(uint32_t cfg) {

    WrCmdDat(CMD_CFG_DEV, DAT_WR_BYTE(cfg ? CONF_DVICE : 0));

    LPC_USB->ReEp = 0x00000003;
}

/* USB Config EP */
void USB_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD) {
    uint32_t num;

    num = EPAdr(pEPD->bEndpointAddress);
    LPC_USB->ReEp |= (1 << num);
    LPC_USB->EpInd = num;
    LPC_USB->MaxPSize = pEPD->wMaxPacketSize;
    if (!usb_wait(&LPC_USB->DevIntSt, EP_RLZED_INT, 4))
        return;
    LPC_USB->DevIntClr = EP_RLZED_INT;
}

/* USB Dir Ctrl EP */
void USB_DirCtrlEP(uint32_t dir) {}

/* USB Enable EP */
void USB_EnableEP(uint32_t EPNum) {
    WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(0));
}

/* USB Disable EP */
void USB_DisableEP(uint32_t EPNum) {
    WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(EP_STAT_DA));
}

/* USB Reset EP */
void USB_ResetEP(uint32_t EPNum) {
    WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(0));
}

/* USB Set Stall EP */
void USB_SetStallEP(uint32_t EPNum) {
    WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(EP_STAT_ST));
}

/* USB Clr Stall EP */
void USB_ClrStallEP(uint32_t EPNum) {
    WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(0));
}

/* USB Clear EPBuf */
void USB_ClearEPBuf(uint32_t EPNum) {
    WrCmdEP(EPNum, CMD_CLR_BUF);
}

/* USB Read EP */
uint32_t USB_ReadEP(uint32_t EPNum, uint8_t *pData) {
    uint32_t cnt, n, word = 0, limit = 10000;

    LPC_USB->Ctrl = ((EPNum & 0x0F) << 2) | CTRL_RD_EN;

    do {
        cnt = LPC_USB->RxPLen;
        if (--limit == 0) {
            LPC_USB->Ctrl = 0;
            return 0;
        }
    } while ((cnt & PKT_RDY) == 0);
    cnt &= PKT_LNGTH_MASK;

    for (n = 0; n < cnt; n++) {
        if ((n & 3) == 0)
            word = LPC_USB->RxData;
        if (n < 64)
            pData[n] = (uint8_t)(word >> (8 * (n & 3)));
    }
    LPC_USB->Ctrl = 0;

    if (((EP_MSK_ISO >> EPNum) & 1) == 0) {
        WrCmdEP(EPNum, CMD_CLR_BUF);
    }
    return (cnt);
}

/* USB Write EP */
uint32_t USB_WriteEP(uint32_t EPNum, uint8_t *pData, uint32_t cnt) {
    uint32_t n, j, word;
    LPC_USB->Ctrl = ((EPNum & 0x0F) << 2) | CTRL_WR_EN;
    LPC_USB->TxPLen = cnt;
    for (n = 0; n < cnt; n += 4) {
        word = 0;
        for (j = 0; j < 4 && n + j < cnt; j++)
            word |= (uint32_t)pData[n + j] << (j * 8);
        LPC_USB->TxData = word;
    }
    LPC_USB->Ctrl = 0;
    WrCmdEP(EPNum, CMD_VALID_BUF);
    return cnt;
}

#if USB_DMA

const uint32_t DDAdr[2] = {DD_NISO_ADR, DD_ISO_ADR};
const uint32_t DDSz[2] = {16, 20};

/* USB DMA Setup */
uint32_t USB_DMA_Setup(uint32_t EPNum, USB_DMA_DESCRIPTOR *pDD) {
    uint32_t num, ptr, nxt, iso, n;

    iso = pDD->Cfg.Type.IsoEP;
    num = EPAdr(EPNum);

    ptr = 0;
    nxt = udca[num];
    while (nxt) {
        ptr = nxt;
        if (!pDD->Cfg.Type.Link) {
            n = (ptr - DDAdr[iso]) / DDSz[iso];
            DDMemMap[iso] &= ~(1 << n);
        }
        nxt = *((uint32_t *)ptr);
    }

    for (n = 0; n < 32; n++) {
        if ((DDMemMap[iso] & (1 << n)) == 0) {
            break;
        }
    }
    if (n == 32)
        return (FALSE);

    DDMemMap[iso] |= 1 << n;
    nxt = DDAdr[iso] + n * DDSz[iso];

    if (ptr && pDD->Cfg.Type.Link) {
        *((uint32_t *)(ptr + 0)) = nxt;
        *((uint32_t *)(ptr + 4)) |= 0x00000004;
    } else {
        udca[num] = nxt;
        UDCA[num] = nxt;
    }

    *(((uint32_t *)nxt)++) = 0;
    *(((uint32_t *)nxt)++) =
        pDD->Cfg.Type.ATLE | (pDD->Cfg.Type.IsoEP << 4) | (pDD->MaxSize << 5) | (pDD->BufLen << 16);
    *(((uint32_t *)nxt)++) = pDD->BufAdr;
    *(((uint32_t *)nxt)++) = pDD->Cfg.Type.LenPos << 8;
    if (iso) {
        *((uint32_t *)nxt) = pDD->InfoAdr;
    }

    return (TRUE);
}

/* USB DMA Enable */
void USB_DMA_Enable(uint32_t EPNum) {
    LPC_USB->EpDMAEn = 1 << EPAdr(EPNum);
}

/* USB DMA Disable */
void USB_DMA_Disable(uint32_t EPNum) {
    LPC_USB->EpDMADis = 1 << EPAdr(EPNum);
}

/* USB DMA Status */
uint32_t USB_DMA_Status(uint32_t EPNum) {
    uint32_t ptr, val;

    ptr = UDCA[EPAdr(EPNum)];
    if (ptr == 0)
        return (USB_DMA_INVALID);

    val = *((uint32_t *)(ptr + 3 * 4));
    switch ((val >> 1) & 0x0F) {
    case 0x00:
        return (USB_DMA_IDLE);
    case 0x01:
        return (USB_DMA_BUSY);
    case 0x02:
        return (USB_DMA_DONE);
    case 0x03:
        return (USB_DMA_UNDER_RUN);
    case 0x08:
        return (USB_DMA_OVER_RUN);
    case 0x09:
        return (USB_DMA_ERROR);
    }

    return (USB_DMA_UNKNOWN);
}

/* USB DMA Buf Adr */
uint32_t USB_DMA_BufAdr(uint32_t EPNum) {
    uint32_t ptr, val;

    ptr = UDCA[EPAdr(EPNum)];
    if (ptr == 0) {
        return ((uint32_t)(-1));
    }

    val = *((uint32_t *)(ptr + 2 * 4));
    return (val);
}

/* USB DMA Buf Cnt */
uint32_t USB_DMA_BufCnt(uint32_t EPNum) {
    uint32_t ptr, val;

    ptr = UDCA[EPAdr(EPNum)];
    if (ptr == 0) {
        return ((uint32_t)(-1));
    }
    val = *((uint32_t *)(ptr + 3 * 4));
    return (val >> 16);
}

#endif

/* USB Get Frame */
uint32_t USB_GetFrame(void) {
    uint32_t val;

    WrCmd(CMD_RD_FRAME);
    val = RdCmdDat(DAT_RD_FRAME);
    val = val | (RdCmdDat(DAT_RD_FRAME) << 8);

    return (val);
}

/* USB interrupts */
/* Handle pending USB events. */
void USB_IRQHandler(void) {
    uint32_t disr, val, n, m;
    uint32_t episr, episrCur;

    if (USB_HWError) {
        NVIC_DisableIRQ(USB_IRQn);
        return;
    }
    disr = LPC_USB->DevIntSt;

    /* USB device status */
    if (disr & DEV_STAT_INT) {
        LPC_USB->DevIntClr = DEV_STAT_INT;
        WrCmd(CMD_GET_DEV_STAT);
        val = RdCmdDat(DAT_GET_DEV_STAT);
        if (val & DEV_RST) {
            USB_Reset();
#if USB_RESET_EVENT
            USB_Reset_Event();
#endif
        }
        if (val & DEV_CON_CH) {
#if USB_POWER_EVENT
            USB_Power_Event(val & DEV_CON);
#endif
        }
        if (val & DEV_SUS_CH) {
            if (val & DEV_SUS) {
                USB_Suspend();
#if USB_SUSPEND_EVENT
                USB_Suspend_Event();
#endif
            } else {
                USB_Resume();
#if USB_RESUME_EVENT
                USB_Resume_Event();
#endif
            }
        }
        goto isr_end;
    }

#if USB_SOF_EVENT

    /* USB frame interrupt */
    if (disr & FRAME_INT) {
        LPC_USB->DevIntClr = FRAME_INT;
        USB_SOF_Event();
    }
#endif

#if USB_ERROR_EVENT

    if (disr & ERR_INT) {
        WrCmd(CMD_RD_ERR_STAT);
        val = RdCmdDat(DAT_RD_ERR_STAT);
        USB_Error_Event(val);
    }
#endif

    if (disr & EP_SLOW_INT) {
        episrCur = 0;
        episr = LPC_USB->EpIntSt;
        for (n = 0; n < USB_EP_NUM; n++) {
            if (episr == episrCur)
                break;
            if (episr & (1 << n)) {
                episrCur |= (1 << n);
                m = n >> 1;

                LPC_USB->DevIntClr = CDFULL_INT;
                LPC_USB->EpIntClr = (1UL << n);
                if (!usb_wait(&LPC_USB->DevIntSt, CDFULL_INT, 3))
                    return;
                val = LPC_USB->CmdData;

                if ((n & 1) == 0) {
                    if (n == 0) {
                        if (val & EP_SEL_STP) {
                            if (USB_P_EP[0]) {
                                USB_P_EP[0](USB_EVT_SETUP);
                                continue;
                            }
                        }
                    }
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_OUT);
                    }
                } else {
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_IN);
                    }
                }
            }
        }
        LPC_USB->DevIntClr = EP_SLOW_INT;
    }

#if USB_DMA

    if (LPC_USB->DMAIntSt & 0x00000001) {
        val = LPC_USB->EoTIntSt;
        for (n = 2; n < USB_EP_NUM; n++) {
            if (val & (1 << n)) {
                m = n >> 1;
                if ((n & 1) == 0) {
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_OUT_DMA_EOT);
                    }
                } else {
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_IN_DMA_EOT);
                    }
                }
            }
        }
        LPC_USB->EoTIntClr = val;
    }

    if (LPC_USB->DMAIntSt & 0x00000002) {
        val = LPC_USB->NDDRIntSt;
        for (n = 2; n < USB_EP_NUM; n++) {
            if (val & (1 << n)) {
                m = n >> 1;
                if ((n & 1) == 0) {
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_OUT_DMA_NDR);
                    }
                } else {
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_IN_DMA_NDR);
                    }
                }
            }
        }
        LPC_USB->NDDRIntClr = val;
    }

    if (LPC_USB->DMAIntSt & 0x00000004) {
        val = LPC_USB->SysErrIntSt;
        for (n = 2; n < USB_EP_NUM; n++) {
            if (val & (1 << n)) {
                m = n >> 1;
                if ((n & 1) == 0) {
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_OUT_DMA_ERR);
                    }
                } else {
                    if (USB_P_EP[m]) {
                        USB_P_EP[m](USB_EVT_IN_DMA_ERR);
                    }
                }
            }
        }
        LPC_USB->SysErrIntClr = val;
    }

#endif

isr_end:
    return;
}
