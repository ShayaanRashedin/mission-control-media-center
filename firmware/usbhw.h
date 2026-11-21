/* Declare USB hardware functions. */

#ifndef __USBHW_H__
/* Definitions */
#define __USBHW_H__

#define USB_RAM_ADR 0x20080000
#define USB_RAM_SZ 0x00004000

#define DD_NISO_CNT 16
#define DD_ISO_CNT 8
#define DD_NISO_SZ (DD_NISO_CNT * 16)
#define DD_ISO_SZ (DD_ISO_CNT * 20)
#define DD_NISO_ADR (USB_RAM_ADR + 128)
#define DD_ISO_ADR (DD_NISO_ADR + DD_NISO_SZ)
#define DD_SZ (128 + DD_NISO_SZ + DD_ISO_SZ)

#define DMA_BUF_ADR (USB_RAM_ADR + DD_SZ)
#define DMA_BUF_SZ (USB_RAM_SZ - DD_SZ)

#define USB_ERR_PID 0x0001
#define USB_ERR_UEPKT 0x0002
#define USB_ERR_DCRC 0x0004
#define USB_ERR_TIMOUT 0x0008
#define USB_ERR_EOP 0x0010
#define USB_ERR_B_OVRN 0x0020
#define USB_ERR_BTSTF 0x0040
#define USB_ERR_TGL 0x0080

#define USB_DMA_INVALID 0x0000
#define USB_DMA_IDLE 0x0001
#define USB_DMA_BUSY 0x0002
#define USB_DMA_DONE 0x0003
#define USB_DMA_OVER_RUN 0x0004
#define USB_DMA_UNDER_RUN 0x0005
#define USB_DMA_ERROR 0x0006
#define USB_DMA_UNKNOWN 0xFFFF

/* Data types */
typedef struct _USB_DMA_DESCRIPTOR {
    uint32_t BufAdr;
    uint16_t BufLen;
    uint16_t MaxSize;
    uint32_t InfoAdr;
    union {
        struct {
            uint32_t Link : 1;
            uint32_t IsoEP : 1;
            uint32_t ATLE : 1;
            uint32_t Rsrvd : 5;
            uint32_t LenPos : 8;
        } Type;
        uint32_t Val;
    } Cfg;
} USB_DMA_DESCRIPTOR;

/* Shared declarations */
extern void USB_Init(void);
extern void USB_Connect(uint32_t con);
extern void USB_Reset(void);
extern void USB_Suspend(void);
extern void USB_Resume(void);
extern void USB_WakeUp(void);
extern void USB_WakeUpCfg(uint32_t cfg);
extern void USB_SetAddress(uint32_t adr);
extern void USB_Configure(uint32_t cfg);
extern void USB_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD);
extern void USB_DirCtrlEP(uint32_t dir);
extern void USB_EnableEP(uint32_t EPNum);
extern void USB_DisableEP(uint32_t EPNum);
extern void USB_ResetEP(uint32_t EPNum);
extern void USB_SetStallEP(uint32_t EPNum);
extern void USB_ClrStallEP(uint32_t EPNum);
extern void USB_ClearEPBuf(uint32_t EPNum);
extern uint32_t USB_ReadEP(uint32_t EPNum, uint8_t *pData);
extern uint32_t USB_WriteEP(uint32_t EPNum, uint8_t *pData, uint32_t cnt);
extern uint32_t USB_DMA_Setup(uint32_t EPNum, USB_DMA_DESCRIPTOR *pDD);
extern void USB_DMA_Enable(uint32_t EPNum);
extern void USB_DMA_Disable(uint32_t EPNum);
extern uint32_t USB_DMA_Status(uint32_t EPNum);
extern uint32_t USB_DMA_BufAdr(uint32_t EPNum);
extern uint32_t USB_DMA_BufCnt(uint32_t EPNum);
extern uint32_t USB_GetFrame(void);
extern void USB_IRQHandler(void);

#endif
