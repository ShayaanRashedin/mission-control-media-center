/* Define Cortex-M3 registers and core functions. */

#ifndef __CM3_CORE_H__
/* Definitions */
#define __CM3_CORE_H__

#ifdef __cplusplus
 /* Shared declarations */
 extern "C" {
#endif

#define __CM3_CMSIS_VERSION_MAIN  (0x01)
#define __CM3_CMSIS_VERSION_SUB   (0x20)
#define __CM3_CMSIS_VERSION       ((__CM3_CMSIS_VERSION_MAIN << 16) | __CM3_CMSIS_VERSION_SUB)

#define __CORTEX_M                (0x03)

/* Includes */
#include <stdint.h>

#if defined (__ICCARM__)
  #include <intrinsics.h>
#endif

#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    4
#endif

#ifdef __cplusplus
#define     __I     volatile
#else
#define     __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

#define NVIC_VECTRESET              0
#define NVIC_SYSRESETREQ            2
#define NVIC_AIRCR_VECTKEY    (0x5FA << 16)
#define NVIC_AIRCR_ENDIANESS        15

#define CoreDebug_DEMCR_TRCENA (1 << 24)
#define ITM_TCR_ITMENA              1

/* Data types */
typedef struct
{
  __IO uint32_t ISER[8];
       uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];
       uint32_t RSERVED1[24];
  __IO uint32_t ISPR[8];
       uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];
       uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];
       uint32_t RESERVED4[56];
  __IO uint8_t  IP[240];
       uint32_t RESERVED5[644];
  __O  uint32_t STIR;
}  NVIC_Type;

typedef struct
{
  __I  uint32_t CPUID;
  __IO uint32_t ICSR;
  __IO uint32_t VTOR;
  __IO uint32_t AIRCR;
  __IO uint32_t SCR;
  __IO uint32_t CCR;
  __IO uint8_t  SHP[12];
  __IO uint32_t SHCSR;
  __IO uint32_t CFSR;
  __IO uint32_t HFSR;
  __IO uint32_t DFSR;
  __IO uint32_t MMFAR;
  __IO uint32_t BFAR;
  __IO uint32_t AFSR;
  __I  uint32_t PFR[2];
  __I  uint32_t DFR;
  __I  uint32_t ADR;
  __I  uint32_t MMFR[4];
  __I  uint32_t ISAR[5];
} SCB_Type;

typedef struct
{
  __IO uint32_t CTRL;
  __IO uint32_t LOAD;
  __IO uint32_t VAL;
  __I  uint32_t CALIB;
} SysTick_Type;

typedef struct
{
  __O  union
  {
    __O  uint8_t    u8;
    __O  uint16_t   u16;
    __O  uint32_t   u32;
  }  PORT [32];
       uint32_t RESERVED0[864];
  __IO uint32_t TER;
       uint32_t RESERVED1[15];
  __IO uint32_t TPR;
       uint32_t RESERVED2[15];
  __IO uint32_t TCR;
       uint32_t RESERVED3[29];
  __IO uint32_t IWR;
  __IO uint32_t IRR;
  __IO uint32_t IMCR;
       uint32_t RESERVED4[43];
  __IO uint32_t LAR;
  __IO uint32_t LSR;
       uint32_t RESERVED5[6];
  __I  uint32_t PID4;
  __I  uint32_t PID5;
  __I  uint32_t PID6;
  __I  uint32_t PID7;
  __I  uint32_t PID0;
  __I  uint32_t PID1;
  __I  uint32_t PID2;
  __I  uint32_t PID3;
  __I  uint32_t CID0;
  __I  uint32_t CID1;
  __I  uint32_t CID2;
  __I  uint32_t CID3;
} ITM_Type;

typedef struct
{
       uint32_t RESERVED0;
  __I  uint32_t ICTR;
#if ((defined __CM3_REV) && (__CM3_REV >= 0x200))
  __IO uint32_t ACTLR;
#else
       uint32_t RESERVED1;
#endif
} InterruptType_Type;

#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1)
typedef struct
{
  __I  uint32_t TYPE;
  __IO uint32_t CTRL;
  __IO uint32_t RNR;
  __IO uint32_t RBAR;
  __IO uint32_t RASR;
  __IO uint32_t RBAR_A1;
  __IO uint32_t RASR_A1;
  __IO uint32_t RBAR_A2;
  __IO uint32_t RASR_A2;
  __IO uint32_t RBAR_A3;
  __IO uint32_t RASR_A3;
} MPU_Type;
#endif

typedef struct
{
  __IO uint32_t DHCSR;
  __O  uint32_t DCRSR;
  __IO uint32_t DCRDR;
  __IO uint32_t DEMCR;
} CoreDebug_Type;

#define SCS_BASE            (0xE000E000)
#define ITM_BASE            (0xE0000000)
#define CoreDebug_BASE      (0xE000EDF0)
#define SysTick_BASE        (SCS_BASE +  0x0010)
#define NVIC_BASE           (SCS_BASE +  0x0100)
#define SCB_BASE            (SCS_BASE +  0x0D00)

#define InterruptType       ((InterruptType_Type *) SCS_BASE)
#define SCB                 ((SCB_Type *)           SCB_BASE)
#define SysTick             ((SysTick_Type *)       SysTick_BASE)
#define NVIC                ((NVIC_Type *)          NVIC_BASE)
#define ITM                 ((ITM_Type *)           ITM_BASE)
#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)

#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1)
  #define MPU_BASE          (SCS_BASE +  0x0D90)
  #define MPU               ((MPU_Type*)            MPU_BASE)
#endif

#if defined ( __CC_ARM   )
  #define __ASM            __asm
  #define __INLINE         __inline

#elif defined ( __ICCARM__ )
  #define __ASM           __asm
  #define __INLINE        inline

#elif defined   (  __GNUC__  )
  #define __ASM            __asm
  #define __INLINE         inline

#elif defined   (  __TASKING__  )
  #define __ASM            __asm
  #define __INLINE         inline

#endif

#if defined ( __CC_ARM   )

#define __enable_fault_irq                __enable_fiq
#define __disable_fault_irq               __disable_fiq

#define __NOP                             __nop
#define __WFI                             __wfi
#define __WFE                             __wfe
#define __SEV                             __sev
#define __ISB()                           __isb(0)
#define __DSB()                           __dsb(0)
#define __DMB()                           __dmb(0)
#define __REV                             __rev
#define __RBIT                            __rbit
#define __LDREXB(ptr)                     ((unsigned char ) __ldrex(ptr))
#define __LDREXH(ptr)                     ((unsigned short) __ldrex(ptr))
#define __LDREXW(ptr)                     ((unsigned int  ) __ldrex(ptr))
#define __STREXB(value, ptr)              __strex(value, ptr)
#define __STREXH(value, ptr)              __strex(value, ptr)
#define __STREXW(value, ptr)              __strex(value, ptr)

extern uint32_t __get_PSP(void);

extern void __set_PSP(uint32_t topOfProcStack);

extern uint32_t __get_MSP(void);

extern void __set_MSP(uint32_t topOfMainStack);

extern uint32_t __REV16(uint16_t value);

extern int32_t __REVSH(int16_t value);

#if (__ARMCC_VERSION < 400000)

extern void __CLREX(void);

extern uint32_t __get_BASEPRI(void);

extern void __set_BASEPRI(uint32_t basePri);

extern uint32_t __get_PRIMASK(void);

extern void __set_PRIMASK(uint32_t priMask);

extern uint32_t __get_FAULTMASK(void);

extern void __set_FAULTMASK(uint32_t faultMask);

extern uint32_t __get_CONTROL(void);

extern void __set_CONTROL(uint32_t control);

#else

#define __CLREX                           __clrex

/* get BASEPRI */
static __INLINE uint32_t  __get_BASEPRI(void)
{
  register uint32_t __regBasePri         __ASM("basepri");
  return(__regBasePri);
}

/* set BASEPRI */
static __INLINE void __set_BASEPRI(uint32_t basePri)
{
  register uint32_t __regBasePri         __ASM("basepri");
  __regBasePri = (basePri & 0x1ff);
}

/* get PRIMASK */
static __INLINE uint32_t __get_PRIMASK(void)
{
  register uint32_t __regPriMask         __ASM("primask");
  return(__regPriMask);
}

/* set PRIMASK */
static __INLINE void __set_PRIMASK(uint32_t priMask)
{
  register uint32_t __regPriMask         __ASM("primask");
  __regPriMask = (priMask);
}

/* get FAULTMASK */
static __INLINE uint32_t __get_FAULTMASK(void)
{
  register uint32_t __regFaultMask       __ASM("faultmask");
  return(__regFaultMask);
}

/* set FAULTMASK */
static __INLINE void __set_FAULTMASK(uint32_t faultMask)
{
  register uint32_t __regFaultMask       __ASM("faultmask");
  __regFaultMask = (faultMask & 1);
}

/* get CONTROL */
static __INLINE uint32_t __get_CONTROL(void)
{
  register uint32_t __regControl         __ASM("control");
  return(__regControl);
}

/* set CONTROL */
static __INLINE void __set_CONTROL(uint32_t control)
{
  register uint32_t __regControl         __ASM("control");
  __regControl = control;
}

#endif

#elif (defined (__ICCARM__))

#define __enable_irq                              __enable_interrupt
#define __disable_irq                             __disable_interrupt

/* enable fault irq */
static __INLINE void __enable_fault_irq()         { __ASM ("cpsie f"); }
/* disable fault irq */
static __INLINE void __disable_fault_irq()        { __ASM ("cpsid f"); }

#define __NOP                                     __no_operation()
/* WFI */
static __INLINE  void __WFI()                     { __ASM ("wfi"); }
/* WFE */
static __INLINE  void __WFE()                     { __ASM ("wfe"); }
/* SEV */
static __INLINE  void __SEV()                     { __ASM ("sev"); }
/* CLREX */
static __INLINE  void __CLREX()                   { __ASM ("clrex"); }

extern uint32_t __get_PSP(void);

extern void __set_PSP(uint32_t topOfProcStack);

extern uint32_t __get_MSP(void);

extern void __set_MSP(uint32_t topOfMainStack);

extern uint32_t __REV16(uint16_t value);

extern uint32_t __RBIT(uint32_t value);

extern uint8_t __LDREXB(uint8_t *addr);

extern uint16_t __LDREXH(uint16_t *addr);

extern uint32_t __LDREXW(uint32_t *addr);

extern uint32_t __STREXB(uint8_t value, uint8_t *addr);

extern uint32_t __STREXH(uint16_t value, uint16_t *addr);

extern uint32_t __STREXW(uint32_t value, uint32_t *addr);

#elif (defined (__GNUC__))

/* enable irq */
static __INLINE void __enable_irq()               { __ASM volatile ("cpsie i"); }
/* disable irq */
static __INLINE void __disable_irq()              { __ASM volatile ("cpsid i"); }

/* enable fault irq */
static __INLINE void __enable_fault_irq()         { __ASM volatile ("cpsie f"); }
/* disable fault irq */
static __INLINE void __disable_fault_irq()        { __ASM volatile ("cpsid f"); }

/* NOP */
static __INLINE void __NOP()                      { __ASM volatile ("nop"); }
/* WFI */
static __INLINE void __WFI()                      { __ASM volatile ("wfi"); }
/* WFE */
static __INLINE void __WFE()                      { __ASM volatile ("wfe"); }
/* SEV */
static __INLINE void __SEV()                      { __ASM volatile ("sev"); }
/* ISB */
static __INLINE void __ISB()                      { __ASM volatile ("isb"); }
/* DSB */
static __INLINE void __DSB()                      { __ASM volatile ("dsb"); }
/* DMB */
static __INLINE void __DMB()                      { __ASM volatile ("dmb"); }
/* CLREX */
static __INLINE void __CLREX()                    { __ASM volatile ("clrex"); }

extern uint32_t __get_PSP(void);

extern void __set_PSP(uint32_t topOfProcStack);

extern uint32_t __get_MSP(void);

extern void __set_MSP(uint32_t topOfMainStack);

extern uint32_t __get_BASEPRI(void);

extern void __set_BASEPRI(uint32_t basePri);

extern uint32_t  __get_PRIMASK(void);

extern void __set_PRIMASK(uint32_t priMask);

extern uint32_t __get_FAULTMASK(void);

extern void __set_FAULTMASK(uint32_t faultMask);

extern uint32_t __get_CONTROL(void);

extern void __set_CONTROL(uint32_t control);

extern uint32_t __REV(uint32_t value);

extern uint32_t __REV16(uint16_t value);

extern int32_t __REVSH(int16_t value);

extern uint32_t __RBIT(uint32_t value);

extern uint8_t __LDREXB(uint8_t *addr);

extern uint16_t __LDREXH(uint16_t *addr);

extern uint32_t __LDREXW(uint32_t *addr);

extern uint32_t __STREXB(uint8_t value, uint8_t *addr);

extern uint32_t __STREXH(uint16_t value, uint16_t *addr);

extern uint32_t __STREXW(uint32_t value, uint32_t *addr);

#elif (defined (__TASKING__))

#endif

/* NVIC Set Priority Grouping */
static __INLINE void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);

  reg_value  = SCB->AIRCR;
  reg_value &= ~((0xFFFFU << 16) | (0x0F << 8));
  reg_value  = ((reg_value | NVIC_AIRCR_VECTKEY | (PriorityGroupTmp << 8)));
  SCB->AIRCR = reg_value;
}

/* NVIC Get Priority Grouping */
static __INLINE uint32_t NVIC_GetPriorityGrouping(void)
{
  return ((SCB->AIRCR >> 8) & 0x07);
}

/* NVIC Enable IRQ */
static __INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/* NVIC Disable IRQ */
static __INLINE void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/* NVIC Get Pending IRQ */
static __INLINE uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((NVIC->ISPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/* NVIC Set Pending IRQ */
static __INLINE void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  NVIC->ISPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/* NVIC Clear Pending IRQ */
static __INLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/* NVIC Get Active */
static __INLINE uint32_t NVIC_GetActive(IRQn_Type IRQn)
{
  return((uint32_t)((NVIC->IABR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/* NVIC Set Priority */
static __INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if(IRQn < 0) {
    SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); }
  else {
    NVIC->IP[(uint32_t)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);    }
}

/* NVIC Get Priority */
static __INLINE uint32_t NVIC_GetPriority(IRQn_Type IRQn)
{

  if(IRQn < 0) {
    return((uint32_t)(SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] >> (8 - __NVIC_PRIO_BITS)));  }
  else {
    return((uint32_t)(NVIC->IP[(uint32_t)(IRQn)]           >> (8 - __NVIC_PRIO_BITS)));  }
}

/* NVIC Encode Priority */
static __INLINE uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

  return (
           ((PreemptPriority & ((1 << (PreemptPriorityBits)) - 1)) << SubPriorityBits) |
           ((SubPriority     & ((1 << (SubPriorityBits    )) - 1)))
         );
}

/* NVIC Decode Priority */
static __INLINE void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

  *pPreemptPriority = (Priority >> SubPriorityBits) & ((1 << (PreemptPriorityBits)) - 1);
  *pSubPriority     = (Priority                   ) & ((1 << (SubPriorityBits    )) - 1);
}

#if (!defined (__Vendor_SysTickConfig)) || (__Vendor_SysTickConfig == 0)

#define SYSTICK_ENABLE              0
#define SYSTICK_TICKINT             1
#define SYSTICK_CLKSOURCE           2
#define SYSTICK_MAXCOUNT       ((1<<24) -1)

/* Sys Tick Config */
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{
  if (ticks > SYSTICK_MAXCOUNT)  return (1);

  SysTick->LOAD  =  (ticks & SYSTICK_MAXCOUNT) - 1;
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
  SysTick->VAL   =  (0x00);
  SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (1<<SYSTICK_TICKINT);
  return (0);
}

#endif

/* NVIC System Reset */
static __INLINE void NVIC_SystemReset(void)
{
  SCB->AIRCR  = (NVIC_AIRCR_VECTKEY | (SCB->AIRCR & (0x700)) | (1<<NVIC_SYSRESETREQ));
  __DSB();
  while(1);
}

/* ITM Send Char */
static __INLINE uint32_t ITM_SendChar (uint32_t ch)
{
  if (ch == '\n') ITM_SendChar('\r');

  if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA)  &&
      (ITM->TCR & ITM_TCR_ITMENA)                  &&
      (ITM->TER & (1UL << 0))  )
  {
    while (ITM->PORT[0].u32 == 0);
    ITM->PORT[0].u8 = (uint8_t) ch;
  }
  return (ch);
}

#ifdef __cplusplus
}
#endif

#endif
