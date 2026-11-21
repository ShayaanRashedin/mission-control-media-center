/* Define LPC17xx hardware registers. */

#ifndef __LPC17xx_H__
/* Definitions */
#define __LPC17xx_H__

/* Data types */
typedef enum IRQn {

    NonMaskableInt_IRQn = -14,
    MemoryManagement_IRQn = -12,
    BusFault_IRQn = -11,
    UsageFault_IRQn = -10,
    SVCall_IRQn = -5,
    DebugMonitor_IRQn = -4,
    PendSV_IRQn = -2,
    SysTick_IRQn = -1,

    WDT_IRQn = 0,
    TIMER0_IRQn = 1,
    TIMER1_IRQn = 2,
    TIMER2_IRQn = 3,
    TIMER3_IRQn = 4,
    UART0_IRQn = 5,
    UART1_IRQn = 6,
    UART2_IRQn = 7,
    UART3_IRQn = 8,
    PWM1_IRQn = 9,
    I2C0_IRQn = 10,
    I2C1_IRQn = 11,
    I2C2_IRQn = 12,
    SPI_IRQn = 13,
    SSP0_IRQn = 14,
    SSP1_IRQn = 15,
    PLL0_IRQn = 16,
    RTC_IRQn = 17,
    EINT0_IRQn = 18,
    EINT1_IRQn = 19,
    EINT2_IRQn = 20,
    EINT3_IRQn = 21,
    ADC_IRQn = 22,
    BOD_IRQn = 23,
    USB_IRQn = 24,
    CAN_IRQn = 25,
    DMA_IRQn = 26,
    I2S_IRQn = 27,
    ENET_IRQn = 28,
    RIT_IRQn = 29,
    MCPWM_IRQn = 30,
    QEI_IRQn = 31,
    PLL1_IRQn = 32,
    USBActivity_IRQn = 33,
    CANActivity_IRQn = 34
} IRQn_Type;

#define __MPU_PRESENT 1
#define __NVIC_PRIO_BITS 5
#define __Vendor_SysTickConfig 0

/* Includes */
#include "core_cm3.h"
#include "system_LPC17xx.h"

#if defined(__CC_ARM)
#pragma anon_unions
#endif

typedef struct {
    __IO uint32_t FLASHCFG;
    uint32_t RESERVED0[31];
    __IO uint32_t PLL0CON;
    __IO uint32_t PLL0CFG;
    __I uint32_t PLL0STAT;
    __O uint32_t PLL0FEED;
    uint32_t RESERVED1[4];
    __IO uint32_t PLL1CON;
    __IO uint32_t PLL1CFG;
    __I uint32_t PLL1STAT;
    __O uint32_t PLL1FEED;
    uint32_t RESERVED2[4];
    __IO uint32_t PCON;
    __IO uint32_t PCONP;
    uint32_t RESERVED3[15];
    __IO uint32_t CCLKCFG;
    __IO uint32_t USBCLKCFG;
    __IO uint32_t CLKSRCSEL;
    __IO uint32_t CANSLEEPCLR;
    __IO uint32_t CANWAKEFLAGS;
    uint32_t RESERVED4[10];
    __IO uint32_t EXTINT;
    uint32_t RESERVED5[1];
    __IO uint32_t EXTMODE;
    __IO uint32_t EXTPOLAR;
    uint32_t RESERVED6[12];
    __IO uint32_t RSID;
    uint32_t RESERVED7[7];
    __IO uint32_t SCS;
    __IO uint32_t IRCTRIM;
    __IO uint32_t PCLKSEL0;
    __IO uint32_t PCLKSEL1;
    uint32_t RESERVED8[4];
    __IO uint32_t USBIntSt;
    __IO uint32_t DMAREQSEL;
    __IO uint32_t CLKOUTCFG;

} LPC_SC_TypeDef;

typedef struct {
    __IO uint32_t PINSEL0;
    __IO uint32_t PINSEL1;
    __IO uint32_t PINSEL2;
    __IO uint32_t PINSEL3;
    __IO uint32_t PINSEL4;
    __IO uint32_t PINSEL5;
    __IO uint32_t PINSEL6;
    __IO uint32_t PINSEL7;
    __IO uint32_t PINSEL8;
    __IO uint32_t PINSEL9;
    __IO uint32_t PINSEL10;
    uint32_t RESERVED0[5];
    __IO uint32_t PINMODE0;
    __IO uint32_t PINMODE1;
    __IO uint32_t PINMODE2;
    __IO uint32_t PINMODE3;
    __IO uint32_t PINMODE4;
    __IO uint32_t PINMODE5;
    __IO uint32_t PINMODE6;
    __IO uint32_t PINMODE7;
    __IO uint32_t PINMODE8;
    __IO uint32_t PINMODE9;
    __IO uint32_t PINMODE_OD0;
    __IO uint32_t PINMODE_OD1;
    __IO uint32_t PINMODE_OD2;
    __IO uint32_t PINMODE_OD3;
    __IO uint32_t PINMODE_OD4;
    __IO uint32_t I2CPADCFG;
} LPC_PINCON_TypeDef;

typedef struct {
    union {
        __IO uint32_t FIODIR;
        struct {
            __IO uint16_t FIODIRL;
            __IO uint16_t FIODIRH;
        };
        struct {
            __IO uint8_t FIODIR0;
            __IO uint8_t FIODIR1;
            __IO uint8_t FIODIR2;
            __IO uint8_t FIODIR3;
        };
    };
    uint32_t RESERVED0[3];
    union {
        __IO uint32_t FIOMASK;
        struct {
            __IO uint16_t FIOMASKL;
            __IO uint16_t FIOMASKH;
        };
        struct {
            __IO uint8_t FIOMASK0;
            __IO uint8_t FIOMASK1;
            __IO uint8_t FIOMASK2;
            __IO uint8_t FIOMASK3;
        };
    };
    union {
        __IO uint32_t FIOPIN;
        struct {
            __IO uint16_t FIOPINL;
            __IO uint16_t FIOPINH;
        };
        struct {
            __IO uint8_t FIOPIN0;
            __IO uint8_t FIOPIN1;
            __IO uint8_t FIOPIN2;
            __IO uint8_t FIOPIN3;
        };
    };
    union {
        __IO uint32_t FIOSET;
        struct {
            __IO uint16_t FIOSETL;
            __IO uint16_t FIOSETH;
        };
        struct {
            __IO uint8_t FIOSET0;
            __IO uint8_t FIOSET1;
            __IO uint8_t FIOSET2;
            __IO uint8_t FIOSET3;
        };
    };
    union {
        __O uint32_t FIOCLR;
        struct {
            __O uint16_t FIOCLRL;
            __O uint16_t FIOCLRH;
        };
        struct {
            __O uint8_t FIOCLR0;
            __O uint8_t FIOCLR1;
            __O uint8_t FIOCLR2;
            __O uint8_t FIOCLR3;
        };
    };
} LPC_GPIO_TypeDef;

typedef struct {
    __I uint32_t IntStatus;
    __I uint32_t IO0IntStatR;
    __I uint32_t IO0IntStatF;
    __O uint32_t IO0IntClr;
    __IO uint32_t IO0IntEnR;
    __IO uint32_t IO0IntEnF;
    uint32_t RESERVED0[3];
    __I uint32_t IO2IntStatR;
    __I uint32_t IO2IntStatF;
    __O uint32_t IO2IntClr;
    __IO uint32_t IO2IntEnR;
    __IO uint32_t IO2IntEnF;
} LPC_GPIOINT_TypeDef;

typedef struct {
    __IO uint32_t IR;
    __IO uint32_t TCR;
    __IO uint32_t TC;
    __IO uint32_t PR;
    __IO uint32_t PC;
    __IO uint32_t MCR;
    __IO uint32_t MR0;
    __IO uint32_t MR1;
    __IO uint32_t MR2;
    __IO uint32_t MR3;
    __IO uint32_t CCR;
    __I uint32_t CR0;
    __I uint32_t CR1;
    uint32_t RESERVED0[2];
    __IO uint32_t EMR;
    uint32_t RESERVED1[12];
    __IO uint32_t CTCR;
} LPC_TIM_TypeDef;

typedef struct {
    __IO uint32_t IR;
    __IO uint32_t TCR;
    __IO uint32_t TC;
    __IO uint32_t PR;
    __IO uint32_t PC;
    __IO uint32_t MCR;
    __IO uint32_t MR0;
    __IO uint32_t MR1;
    __IO uint32_t MR2;
    __IO uint32_t MR3;
    __IO uint32_t CCR;
    __I uint32_t CR0;
    __I uint32_t CR1;
    __I uint32_t CR2;
    __I uint32_t CR3;
    uint32_t RESERVED0;
    __IO uint32_t MR4;
    __IO uint32_t MR5;
    __IO uint32_t MR6;
    __IO uint32_t PCR;
    __IO uint32_t LER;
    uint32_t RESERVED1[7];
    __IO uint32_t CTCR;
} LPC_PWM_TypeDef;

typedef struct {
    union {
        __I uint32_t RBR;
        __O uint32_t THR;
        __IO uint32_t DLL;
    };
    union {
        __IO uint32_t DLM;
        __IO uint32_t IER;
    };
    union {
        __I uint32_t IIR;
        __O uint32_t FCR;
    };
    __IO uint32_t LCR;
    uint32_t RESERVED0;
    __I uint32_t LSR;
    uint32_t RESERVED1;
    __IO uint32_t SCR;
    __IO uint32_t ACR;
    __IO uint32_t ICR;
    __IO uint32_t FDR;
    uint32_t RESERVED2;
    __IO uint32_t TER;
} LPC_UART_TypeDef;

typedef struct {
    union {
        __I uint32_t RBR;
        __O uint32_t THR;
        __IO uint32_t DLL;
    };
    union {
        __IO uint32_t DLM;
        __IO uint32_t IER;
    };
    union {
        __I uint32_t IIR;
        __O uint32_t FCR;
    };
    __IO uint32_t LCR;
    __IO uint32_t MCR;
    __I uint32_t LSR;
    __I uint32_t MSR;
    __IO uint32_t SCR;
    __IO uint32_t ACR;
    uint32_t RESERVED0;
    __IO uint32_t FDR;
    uint32_t RESERVED1;
    __IO uint32_t TER;
    uint32_t RESERVED2[6];
    __IO uint32_t RS485CTRL;
    __IO uint32_t ADRMATCH;
    __IO uint32_t RS485DLY;
} LPC_UART1_TypeDef;

typedef struct {
    __IO uint32_t SPCR;
    __I uint32_t SPSR;
    __IO uint32_t SPDR;
    __IO uint32_t SPCCR;
    uint32_t RESERVED0[3];
    __IO uint32_t SPINT;
} LPC_SPI_TypeDef;

typedef struct {
    __IO uint32_t CR0;
    __IO uint32_t CR1;
    __IO uint32_t DR;
    __I uint32_t SR;
    __IO uint32_t CPSR;
    __IO uint32_t IMSC;
    __IO uint32_t RIS;
    __IO uint32_t MIS;
    __IO uint32_t ICR;
    __IO uint32_t DMACR;
} LPC_SSP_TypeDef;

typedef struct {
    __IO uint32_t CONSET;
    __I uint32_t STAT;
    __IO uint32_t DAT;
    __IO uint32_t ADR0;
    __IO uint32_t SCLH;
    __IO uint32_t SCLL;
    __O uint32_t CONCLR;
    __IO uint32_t MMCTRL;
    __IO uint32_t ADR1;
    __IO uint32_t ADR2;
    __IO uint32_t ADR3;
    __I uint32_t DATA_BUFFER;
    __IO uint32_t MASK0;
    __IO uint32_t MASK1;
    __IO uint32_t MASK2;
    __IO uint32_t MASK3;
} LPC_I2C_TypeDef;

typedef struct {
    __IO uint32_t DAO;
    __IO uint32_t DAI;
    __O uint32_t TXFIFO;
    __I uint32_t RXFIFO;
    __I uint32_t STATE;
    __IO uint32_t DMA1;
    __IO uint32_t DMA2;
    __IO uint32_t IRQ;
    __IO uint32_t TXRATE;
    __IO uint32_t RXRATE;
    __IO uint32_t TXBITRATE;
    __IO uint32_t RXBITRATE;
    __IO uint32_t TXMODE;
    __IO uint32_t RXMODE;
} LPC_I2S_TypeDef;

typedef struct {
    __IO uint32_t RICOMPVAL;
    __IO uint32_t RIMASK;
    __IO uint32_t RICTRL;
    __IO uint32_t RICOUNTER;
} LPC_RIT_TypeDef;

typedef struct {
    __IO uint32_t ILR;
    uint32_t RESERVED0;
    __IO uint32_t CCR;
    __IO uint32_t CIIR;
    __IO uint32_t AMR;
    __I uint32_t CTIME0;
    __I uint32_t CTIME1;
    __I uint32_t CTIME2;
    __IO uint32_t SEC;
    __IO uint32_t MIN;
    __IO uint32_t HOUR;
    __IO uint32_t DOM;
    __IO uint32_t DOW;
    __IO uint32_t DOY;
    __IO uint32_t MONTH;
    __IO uint32_t YEAR;
    __IO uint32_t CALIBRATION;
    __IO uint32_t GPREG0;
    __IO uint32_t GPREG1;
    __IO uint32_t GPREG2;
    __IO uint32_t GPREG3;
    __IO uint32_t GPREG4;
    __IO uint32_t RTC_AUXEN;
    __IO uint32_t RTC_AUX;
    __IO uint32_t ALSEC;
    __IO uint32_t ALMIN;
    __IO uint32_t ALHOUR;
    __IO uint32_t ALDOM;
    __IO uint32_t ALDOW;
    __IO uint32_t ALDOY;
    __IO uint32_t ALMON;
    __IO uint32_t ALYEAR;
} LPC_RTC_TypeDef;

typedef struct {
    __IO uint32_t MOD;
    __IO uint32_t TC;
    __O uint32_t FEED;
    __I uint32_t TV;
    __IO uint32_t WDCLKSEL;
} LPC_WDT_TypeDef;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t GDR;
    uint32_t RESERVED0;
    __IO uint32_t INTEN;
    __I uint32_t DR[8];
    __I uint32_t STAT;
    __IO uint32_t ADTRM;
    __IO uint32_t ADCR;
    __IO uint32_t ADGDR;
    __IO uint32_t ADINTEN;
    __I uint32_t ADDR0;
    __I uint32_t ADDR1;
    __I uint32_t ADDR2;
    __I uint32_t ADDR3;
    __I uint32_t ADDR4;
    __I uint32_t ADDR5;
    __I uint32_t ADDR6;
    __I uint32_t ADDR7;
    __I uint32_t ADSTAT;
} LPC_ADC_TypeDef;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t CTRL;
    __IO uint32_t CNTVAL;
    __IO uint32_t DACR;
    __IO uint32_t DACCTRL;
    __IO uint16_t DACCNTVAL;
} LPC_DAC_TypeDef;

typedef struct {
    __I uint32_t CON;
    __O uint32_t CON_SET;
    __O uint32_t CON_CLR;
    __I uint32_t CAPCON;
    __O uint32_t CAPCON_SET;
    __O uint32_t CAPCON_CLR;
    __IO uint32_t TC0;
    __IO uint32_t TC1;
    __IO uint32_t TC2;
    __IO uint32_t LIM0;
    __IO uint32_t LIM1;
    __IO uint32_t LIM2;
    __IO uint32_t MAT0;
    __IO uint32_t MAT1;
    __IO uint32_t MAT2;
    __IO uint32_t DT;
    __IO uint32_t CP;
    __IO uint32_t CAP0;
    __IO uint32_t CAP1;
    __IO uint32_t CAP2;
    __I uint32_t INTEN;
    __O uint32_t INTEN_SET;
    __O uint32_t INTEN_CLR;
    __I uint32_t CNTCON;
    __O uint32_t CNTCON_SET;
    __O uint32_t CNTCON_CLR;
    __I uint32_t INTF;
    __O uint32_t INTF_SET;
    __O uint32_t INTF_CLR;
    __O uint32_t CAP_CLR;
} LPC_MCPWM_TypeDef;

typedef struct {
    __O uint32_t CON;
    __I uint32_t STAT;
    __IO uint32_t CONF;
    __I uint32_t POS;
    __IO uint32_t MAXPOS;
    __IO uint32_t CMPOS0;
    __IO uint32_t CMPOS1;
    __IO uint32_t CMPOS2;
    __I uint32_t INXCNT;
    __IO uint32_t INXCMP0;
    __IO uint32_t LOAD;
    __I uint32_t TIME;
    __I uint32_t VEL;
    __I uint32_t CAP;
    __IO uint32_t VELCOMP;
    __IO uint32_t FILTER;
    uint32_t RESERVED0[998];
    __O uint32_t IEC;
    __O uint32_t IES;
    __I uint32_t INTSTAT;
    __I uint32_t IE;
    __O uint32_t CLR;
    __O uint32_t SET;
} LPC_QEI_TypeDef;

typedef struct {
    __IO uint32_t mask[512];
} LPC_CANAF_RAM_TypeDef;

typedef struct {
    __IO uint32_t AFMR;
    __IO uint32_t SFF_sa;
    __IO uint32_t SFF_GRP_sa;
    __IO uint32_t EFF_sa;
    __IO uint32_t EFF_GRP_sa;
    __IO uint32_t ENDofTable;
    __I uint32_t LUTerrAd;
    __I uint32_t LUTerr;
    __IO uint32_t FCANIE;
    __IO uint32_t FCANIC0;
    __IO uint32_t FCANIC1;
} LPC_CANAF_TypeDef;

typedef struct {
    __I uint32_t TxSR;
    __I uint32_t RxSR;
    __I uint32_t MSR;
} LPC_CANCR_TypeDef;

typedef struct {
    __IO uint32_t MOD;
    __O uint32_t CMR;
    __IO uint32_t GSR;
    __I uint32_t ICR;
    __IO uint32_t IER;
    __IO uint32_t BTR;
    __IO uint32_t EWL;
    __I uint32_t SR;
    __IO uint32_t RFS;
    __IO uint32_t RID;
    __IO uint32_t RDA;
    __IO uint32_t RDB;
    __IO uint32_t TFI1;
    __IO uint32_t TID1;
    __IO uint32_t TDA1;
    __IO uint32_t TDB1;
    __IO uint32_t TFI2;
    __IO uint32_t TID2;
    __IO uint32_t TDA2;
    __IO uint32_t TDB2;
    __IO uint32_t TFI3;
    __IO uint32_t TID3;
    __IO uint32_t TDA3;
    __IO uint32_t TDB3;
} LPC_CAN_TypeDef;

typedef struct {
    __I uint32_t IntStat;
    __I uint32_t IntTCStat;
    __O uint32_t IntTCClear;
    __I uint32_t IntErrStat;
    __O uint32_t IntErrClr;
    __I uint32_t RawIntTCStat;
    __I uint32_t RawIntErrStat;
    __I uint32_t EnbldChns;
    __IO uint32_t SoftBReq;
    __IO uint32_t SoftSReq;
    __IO uint32_t SoftLBReq;
    __IO uint32_t SoftLSReq;
    __IO uint32_t Config;
    __IO uint32_t Sync;
} LPC_GPDMA_TypeDef;

typedef struct {
    __IO uint32_t CSrcAddr;
    __IO uint32_t CDestAddr;
    __IO uint32_t CLLI;
    __IO uint32_t CControl;
    __IO uint32_t CConfig;
} LPC_GPDMACH_TypeDef;

typedef struct {
    __I uint32_t Revision;
    __IO uint32_t Control;
    __IO uint32_t CommandStatus;
    __IO uint32_t InterruptStatus;
    __IO uint32_t InterruptEnable;
    __IO uint32_t InterruptDisable;
    __IO uint32_t HCCA;
    __I uint32_t PeriodCurrentED;
    __IO uint32_t ControlHeadED;
    __IO uint32_t ControlCurrentED;
    __IO uint32_t BulkHeadED;
    __IO uint32_t BulkCurrentED;
    __I uint32_t DoneHead;
    __IO uint32_t FmInterval;
    __I uint32_t FmRemaining;
    __I uint32_t FmNumber;
    __IO uint32_t PeriodicStart;
    __IO uint32_t LSTreshold;
    __IO uint32_t RhDescriptorA;
    __IO uint32_t RhDescriptorB;
    __IO uint32_t RhStatus;
    __IO uint32_t RhPortStatus1;
    __IO uint32_t RhPortStatus2;
    uint32_t RESERVED0[40];
    __I uint32_t Module_ID;

    __I uint32_t IntSt;
    __IO uint32_t IntEn;
    __O uint32_t IntSet;
    __O uint32_t IntClr;
    __IO uint32_t StCtrl;
    __IO uint32_t Tmr;
    uint32_t RESERVED1[58];

    __I uint32_t DevIntSt;
    __IO uint32_t DevIntEn;
    __O uint32_t DevIntClr;
    __O uint32_t DevIntSet;

    __O uint32_t CmdCode;
    __I uint32_t CmdData;

    __I uint32_t RxData;
    __O uint32_t TxData;
    __I uint32_t RxPLen;
    __O uint32_t TxPLen;
    __IO uint32_t Ctrl;
    __O uint32_t DevIntPri;

    __I uint32_t EpIntSt;
    __IO uint32_t EpIntEn;
    __O uint32_t EpIntClr;
    __O uint32_t EpIntSet;
    __O uint32_t EpIntPri;

    __IO uint32_t ReEp;
    __O uint32_t EpInd;
    __IO uint32_t MaxPSize;

    __I uint32_t DMARSt;
    __O uint32_t DMARClr;
    __O uint32_t DMARSet;
    uint32_t RESERVED2[9];
    __IO uint32_t UDCAH;
    __I uint32_t EpDMASt;
    __O uint32_t EpDMAEn;
    __O uint32_t EpDMADis;
    __I uint32_t DMAIntSt;
    __IO uint32_t DMAIntEn;
    uint32_t RESERVED3[2];
    __I uint32_t EoTIntSt;
    __O uint32_t EoTIntClr;
    __O uint32_t EoTIntSet;
    __I uint32_t NDDRIntSt;
    __O uint32_t NDDRIntClr;
    __O uint32_t NDDRIntSet;
    __I uint32_t SysErrIntSt;
    __O uint32_t SysErrIntClr;
    __O uint32_t SysErrIntSet;
    uint32_t RESERVED4[15];

    union {
        __I uint32_t I2C_RX;
        __O uint32_t I2C_TX;
    };
    __I uint32_t I2C_STS;
    __IO uint32_t I2C_CTL;
    __IO uint32_t I2C_CLKHI;
    __O uint32_t I2C_CLKLO;
    uint32_t RESERVED5[824];

    union {
        __IO uint32_t USBClkCtrl;
        __IO uint32_t OTGClkCtrl;
    };
    union {
        __I uint32_t USBClkSt;
        __I uint32_t OTGClkSt;
    };
} LPC_USB_TypeDef;

typedef struct {
    __IO uint32_t MAC1;
    __IO uint32_t MAC2;
    __IO uint32_t IPGT;
    __IO uint32_t IPGR;
    __IO uint32_t CLRT;
    __IO uint32_t MAXF;
    __IO uint32_t SUPP;
    __IO uint32_t TEST;
    __IO uint32_t MCFG;
    __IO uint32_t MCMD;
    __IO uint32_t MADR;
    __O uint32_t MWTD;
    __I uint32_t MRDD;
    __I uint32_t MIND;
    uint32_t RESERVED0[2];
    __IO uint32_t SA0;
    __IO uint32_t SA1;
    __IO uint32_t SA2;
    uint32_t RESERVED1[45];
    __IO uint32_t Command;
    __I uint32_t Status;
    __IO uint32_t RxDescriptor;
    __IO uint32_t RxStatus;
    __IO uint32_t RxDescriptorNumber;
    __I uint32_t RxProduceIndex;
    __IO uint32_t RxConsumeIndex;
    __IO uint32_t TxDescriptor;
    __IO uint32_t TxStatus;
    __IO uint32_t TxDescriptorNumber;
    __IO uint32_t TxProduceIndex;
    __I uint32_t TxConsumeIndex;
    uint32_t RESERVED2[10];
    __I uint32_t TSV0;
    __I uint32_t TSV1;
    __I uint32_t RSV;
    uint32_t RESERVED3[3];
    __IO uint32_t FlowControlCounter;
    __I uint32_t FlowControlStatus;
    uint32_t RESERVED4[34];
    __IO uint32_t RxFilterCtrl;
    __I uint32_t RxFilterWoLStatus;
    __O uint32_t RxFilterWoLClear;
    uint32_t RESERVED5;
    __IO uint32_t HashFilterL;
    __IO uint32_t HashFilterH;
    uint32_t RESERVED6[882];
    __I uint32_t IntStatus;
    __IO uint32_t IntEnable;
    __O uint32_t IntClear;
    __O uint32_t IntSet;
    uint32_t RESERVED7;
    __IO uint32_t PowerDown;
} LPC_EMAC_TypeDef;

#if defined(__CC_ARM)
#pragma no_anon_unions
#endif

#define LPC_FLASH_BASE (0x00000000UL)
#define LPC_RAM_BASE (0x10000000UL)
#ifdef __LPC17XX_REV00
#define LPC_AHBRAM0_BASE (0x20000000UL)
#define LPC_AHBRAM1_BASE (0x20004000UL)
#else
#define LPC_AHBRAM0_BASE (0x2007C000UL)
#define LPC_AHBRAM1_BASE (0x20080000UL)
#endif
#define LPC_GPIO_BASE (0x2009C000UL)
#define LPC_APB0_BASE (0x40000000UL)
#define LPC_APB1_BASE (0x40080000UL)
#define LPC_AHB_BASE (0x50000000UL)
#define LPC_CM3_BASE (0xE0000000UL)

#define LPC_WDT_BASE (LPC_APB0_BASE + 0x00000)
#define LPC_TIM0_BASE (LPC_APB0_BASE + 0x04000)
#define LPC_TIM1_BASE (LPC_APB0_BASE + 0x08000)
#define LPC_UART0_BASE (LPC_APB0_BASE + 0x0C000)
#define LPC_UART1_BASE (LPC_APB0_BASE + 0x10000)
#define LPC_PWM1_BASE (LPC_APB0_BASE + 0x18000)
#define LPC_I2C0_BASE (LPC_APB0_BASE + 0x1C000)
#define LPC_SPI_BASE (LPC_APB0_BASE + 0x20000)
#define LPC_RTC_BASE (LPC_APB0_BASE + 0x24000)
#define LPC_GPIOINT_BASE (LPC_APB0_BASE + 0x28080)
#define LPC_PINCON_BASE (LPC_APB0_BASE + 0x2C000)
#define LPC_SSP1_BASE (LPC_APB0_BASE + 0x30000)
#define LPC_ADC_BASE (LPC_APB0_BASE + 0x34000)
#define LPC_CANAF_RAM_BASE (LPC_APB0_BASE + 0x38000)
#define LPC_CANAF_BASE (LPC_APB0_BASE + 0x3C000)
#define LPC_CANCR_BASE (LPC_APB0_BASE + 0x40000)
#define LPC_CAN1_BASE (LPC_APB0_BASE + 0x44000)
#define LPC_CAN2_BASE (LPC_APB0_BASE + 0x48000)
#define LPC_I2C1_BASE (LPC_APB0_BASE + 0x5C000)

#define LPC_SSP0_BASE (LPC_APB1_BASE + 0x08000)
#define LPC_DAC_BASE (LPC_APB1_BASE + 0x0C000)
#define LPC_TIM2_BASE (LPC_APB1_BASE + 0x10000)
#define LPC_TIM3_BASE (LPC_APB1_BASE + 0x14000)
#define LPC_UART2_BASE (LPC_APB1_BASE + 0x18000)
#define LPC_UART3_BASE (LPC_APB1_BASE + 0x1C000)
#define LPC_I2C2_BASE (LPC_APB1_BASE + 0x20000)
#define LPC_I2S_BASE (LPC_APB1_BASE + 0x28000)
#define LPC_RIT_BASE (LPC_APB1_BASE + 0x30000)
#define LPC_MCPWM_BASE (LPC_APB1_BASE + 0x38000)
#define LPC_QEI_BASE (LPC_APB1_BASE + 0x3C000)
#define LPC_SC_BASE (LPC_APB1_BASE + 0x7C000)

#define LPC_EMAC_BASE (LPC_AHB_BASE + 0x00000)
#define LPC_GPDMA_BASE (LPC_AHB_BASE + 0x04000)
#define LPC_GPDMACH0_BASE (LPC_AHB_BASE + 0x04100)
#define LPC_GPDMACH1_BASE (LPC_AHB_BASE + 0x04120)
#define LPC_GPDMACH2_BASE (LPC_AHB_BASE + 0x04140)
#define LPC_GPDMACH3_BASE (LPC_AHB_BASE + 0x04160)
#define LPC_GPDMACH4_BASE (LPC_AHB_BASE + 0x04180)
#define LPC_GPDMACH5_BASE (LPC_AHB_BASE + 0x041A0)
#define LPC_GPDMACH6_BASE (LPC_AHB_BASE + 0x041C0)
#define LPC_GPDMACH7_BASE (LPC_AHB_BASE + 0x041E0)
#define LPC_USB_BASE (LPC_AHB_BASE + 0x0C000)

#define LPC_GPIO0_BASE (LPC_GPIO_BASE + 0x00000)
#define LPC_GPIO1_BASE (LPC_GPIO_BASE + 0x00020)
#define LPC_GPIO2_BASE (LPC_GPIO_BASE + 0x00040)
#define LPC_GPIO3_BASE (LPC_GPIO_BASE + 0x00060)
#define LPC_GPIO4_BASE (LPC_GPIO_BASE + 0x00080)

#define LPC_SC ((LPC_SC_TypeDef *)LPC_SC_BASE)
#define LPC_GPIO0 ((LPC_GPIO_TypeDef *)LPC_GPIO0_BASE)
#define LPC_GPIO1 ((LPC_GPIO_TypeDef *)LPC_GPIO1_BASE)
#define LPC_GPIO2 ((LPC_GPIO_TypeDef *)LPC_GPIO2_BASE)
#define LPC_GPIO3 ((LPC_GPIO_TypeDef *)LPC_GPIO3_BASE)
#define LPC_GPIO4 ((LPC_GPIO_TypeDef *)LPC_GPIO4_BASE)
#define LPC_WDT ((LPC_WDT_TypeDef *)LPC_WDT_BASE)
#define LPC_TIM0 ((LPC_TIM_TypeDef *)LPC_TIM0_BASE)
#define LPC_TIM1 ((LPC_TIM_TypeDef *)LPC_TIM1_BASE)
#define LPC_TIM2 ((LPC_TIM_TypeDef *)LPC_TIM2_BASE)
#define LPC_TIM3 ((LPC_TIM_TypeDef *)LPC_TIM3_BASE)
#define LPC_RIT ((LPC_RIT_TypeDef *)LPC_RIT_BASE)
#define LPC_UART0 ((LPC_UART_TypeDef *)LPC_UART0_BASE)
#define LPC_UART1 ((LPC_UART1_TypeDef *)LPC_UART1_BASE)
#define LPC_UART2 ((LPC_UART_TypeDef *)LPC_UART2_BASE)
#define LPC_UART3 ((LPC_UART_TypeDef *)LPC_UART3_BASE)
#define LPC_PWM1 ((LPC_PWM_TypeDef *)LPC_PWM1_BASE)
#define LPC_I2C0 ((LPC_I2C_TypeDef *)LPC_I2C0_BASE)
#define LPC_I2C1 ((LPC_I2C_TypeDef *)LPC_I2C1_BASE)
#define LPC_I2C2 ((LPC_I2C_TypeDef *)LPC_I2C2_BASE)
#define LPC_I2S ((LPC_I2S_TypeDef *)LPC_I2S_BASE)
#define LPC_SPI ((LPC_SPI_TypeDef *)LPC_SPI_BASE)
#define LPC_RTC ((LPC_RTC_TypeDef *)LPC_RTC_BASE)
#define LPC_GPIOINT ((LPC_GPIOINT_TypeDef *)LPC_GPIOINT_BASE)
#define LPC_PINCON ((LPC_PINCON_TypeDef *)LPC_PINCON_BASE)
#define LPC_SSP0 ((LPC_SSP_TypeDef *)LPC_SSP0_BASE)
#define LPC_SSP1 ((LPC_SSP_TypeDef *)LPC_SSP1_BASE)
#define LPC_ADC ((LPC_ADC_TypeDef *)LPC_ADC_BASE)
#define LPC_DAC ((LPC_DAC_TypeDef *)LPC_DAC_BASE)
#define LPC_CANAF_RAM ((LPC_CANAF_RAM_TypeDef *)LPC_CANAF_RAM_BASE)
#define LPC_CANAF ((LPC_CANAF_TypeDef *)LPC_CANAF_BASE)
#define LPC_CANCR ((LPC_CANCR_TypeDef *)LPC_CANCR_BASE)
#define LPC_CAN1 ((LPC_CAN_TypeDef *)LPC_CAN1_BASE)
#define LPC_CAN2 ((LPC_CAN_TypeDef *)LPC_CAN2_BASE)
#define LPC_MCPWM ((LPC_MCPWM_TypeDef *)LPC_MCPWM_BASE)
#define LPC_QEI ((LPC_QEI_TypeDef *)LPC_QEI_BASE)
#define LPC_EMAC ((LPC_EMAC_TypeDef *)LPC_EMAC_BASE)
#define LPC_GPDMA ((LPC_GPDMA_TypeDef *)LPC_GPDMA_BASE)
#define LPC_GPDMACH0 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH0_BASE)
#define LPC_GPDMACH1 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH1_BASE)
#define LPC_GPDMACH2 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH2_BASE)
#define LPC_GPDMACH3 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH3_BASE)
#define LPC_GPDMACH4 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH4_BASE)
#define LPC_GPDMACH5 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH5_BASE)
#define LPC_GPDMACH6 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH6_BASE)
#define LPC_GPDMACH7 ((LPC_GPDMACH_TypeDef *)LPC_GPDMACH7_BASE)
#define LPC_USB ((LPC_USB_TypeDef *)LPC_USB_BASE)

#endif
