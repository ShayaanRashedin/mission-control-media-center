/* Set up and track the processor clock. */

/* Includes */
#include <stdint.h>
#include "LPC17xx.h"

#define CLOCK_SETUP 1
#define SCS_Val 0x00000020
#define CLKSRCSEL_Val 0x00000001
#define PLL0_SETUP 1
#define PLL0CFG_Val 0x00050063
#define PLL1_SETUP 1
#define PLL1CFG_Val 0x00000023
#define CCLKCFG_Val 0x00000003
#define USBCLKCFG_Val 0x00000000
#define PCLKSEL0_Val 0x00000000
#define PCLKSEL1_Val 0x00000000
#define PCONP_Val 0x042887DE
#define CLKOUTCFG_Val 0x00000000

#define FLASH_SETUP 1
#define FLASHCFG_Val 0x00004000

#define CHECK_RANGE(val, min, max) ((val < min) || (val > max))
#define CHECK_RSVD(val, mask) (val & mask)

#if (CHECK_RSVD((SCS_Val), ~0x00000030))
#error "SCS: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((CLKSRCSEL_Val), 0, 2))
#error "CLKSRCSEL: Value out of range!"
#endif

#if (CHECK_RSVD((PLL0CFG_Val), ~0x00FF7FFF))
#error "PLL0CFG: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PLL1CFG_Val), ~0x0000007F))
#error "PLL1CFG: Invalid values of reserved bits!"
#endif

#if (PLL0_SETUP)
#if (CCLKCFG_Val < 2)
#error "CCLKCFG: CCLKSEL must be greater then 1 if PLL0 is used!"
#endif
#endif

#if (CHECK_RANGE((CCLKCFG_Val), 0, 255))
#error "CCLKCFG: Value out of range!"
#endif

#if (CHECK_RSVD((USBCLKCFG_Val), ~0x0000000F))
#error "USBCLKCFG: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCLKSEL0_Val), 0x000C0C00))
#error "PCLKSEL0: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCLKSEL1_Val), 0x03000300))
#error "PCLKSEL1: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCONP_Val), 0x10100821))
#error "PCONP: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CLKOUTCFG_Val), ~0x000001FF))
#error "CLKOUTCFG: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((FLASHCFG_Val), ~0x0000F000))
#error "FLASHCFG: Invalid values of reserved bits!"
#endif

#define XTAL (12000000UL)
#define OSC_CLK (XTAL)
#define RTC_CLK (32768UL)
#define IRC_OSC (4000000UL)

#define __M (((PLL0CFG_Val) & 0x7FFF) + 1)
#define __N (((PLL0CFG_Val >> 16) & 0x00FF) + 1)
#define __FCCO(__F_IN) ((2ULL * __M * __F_IN) / __N)
#define __CCLK_DIV (((CCLKCFG_Val) & 0x00FF) + 1)

#if (PLL0_SETUP)
#if ((CLKSRCSEL_Val & 0x03) == 1)
#define __CORE_CLK (__FCCO(OSC_CLK) / __CCLK_DIV)
#elif ((CLKSRCSEL_Val & 0x03) == 2)
#define __CORE_CLK (__FCCO(RTC_CLK) / __CCLK_DIV)
#else
#define __CORE_CLK (__FCCO(IRC_OSC) / __CCLK_DIV)
#endif
#else
#if ((CLKSRCSEL_Val & 0x03) == 1)
#define __CORE_CLK (OSC_CLK / __CCLK_DIV)
#elif ((CLKSRCSEL_Val & 0x03) == 2)
#define __CORE_CLK (RTC_CLK / __CCLK_DIV)
#else
#define __CORE_CLK (IRC_OSC / __CCLK_DIV)
#endif
#endif

uint32_t SystemCoreClock = __CORE_CLK;

void SystemCoreClockUpdate(void) {

    if (((LPC_SC->PLL0STAT >> 24) & 3) == 3) {
        switch (LPC_SC->CLKSRCSEL & 0x03) {
        case 0:
        case 3:
            SystemCoreClock =
                (IRC_OSC * ((2ULL * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                 (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1) / ((LPC_SC->CCLKCFG & 0xFF) + 1));
            break;
        case 1:
            SystemCoreClock =
                (OSC_CLK * ((2ULL * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                 (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1) / ((LPC_SC->CCLKCFG & 0xFF) + 1));
            break;
        case 2:
            SystemCoreClock =
                (RTC_CLK * ((2ULL * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                 (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1) / ((LPC_SC->CCLKCFG & 0xFF) + 1));
            break;
        }
    } else {
        switch (LPC_SC->CLKSRCSEL & 0x03) {
        case 0:
        case 3:
            SystemCoreClock = IRC_OSC / ((LPC_SC->CCLKCFG & 0xFF) + 1);
            break;
        case 1:
            SystemCoreClock = OSC_CLK / ((LPC_SC->CCLKCFG & 0xFF) + 1);
            break;
        case 2:
            SystemCoreClock = RTC_CLK / ((LPC_SC->CCLKCFG & 0xFF) + 1);
            break;
        }
    }
}

/* System Init */
void SystemInit(void) {
#if (CLOCK_SETUP)
    LPC_SC->SCS = SCS_Val;
    if (LPC_SC->SCS & (1 << 5)) {
        while ((LPC_SC->SCS & (1 << 6)) == 0)
            ;
    }

    LPC_SC->CCLKCFG = CCLKCFG_Val;

    LPC_SC->PCLKSEL0 = PCLKSEL0_Val;
    LPC_SC->PCLKSEL1 = PCLKSEL1_Val;

    LPC_SC->CLKSRCSEL = CLKSRCSEL_Val;

#if (PLL0_SETUP)
    LPC_SC->PLL0CFG = PLL0CFG_Val;
    LPC_SC->PLL0FEED = 0xAA;
    LPC_SC->PLL0FEED = 0x55;

    LPC_SC->PLL0CON = 0x01;
    LPC_SC->PLL0FEED = 0xAA;
    LPC_SC->PLL0FEED = 0x55;
    while (!(LPC_SC->PLL0STAT & (1 << 26)))
        ;

    LPC_SC->PLL0CON = 0x03;
    LPC_SC->PLL0FEED = 0xAA;
    LPC_SC->PLL0FEED = 0x55;
    while ((LPC_SC->PLL0STAT & ((1 << 25) | (1 << 24))) != ((1 << 25) | (1 << 24)))
        ;
#endif

#if (PLL1_SETUP)
    LPC_SC->PLL1CFG = PLL1CFG_Val;
    LPC_SC->PLL1FEED = 0xAA;
    LPC_SC->PLL1FEED = 0x55;

    LPC_SC->PLL1CON = 0x01;
    LPC_SC->PLL1FEED = 0xAA;
    LPC_SC->PLL1FEED = 0x55;
    while (!(LPC_SC->PLL1STAT & (1 << 10)))
        ;

    LPC_SC->PLL1CON = 0x03;
    LPC_SC->PLL1FEED = 0xAA;
    LPC_SC->PLL1FEED = 0x55;
    while ((LPC_SC->PLL1STAT & ((1 << 9) | (1 << 8))) != ((1 << 9) | (1 << 8)))
        ;
#else
    LPC_SC->USBCLKCFG = USBCLKCFG_Val;
#endif

    LPC_SC->PCONP = PCONP_Val;

    LPC_SC->CLKOUTCFG = CLKOUTCFG_Val;
#endif

#if (FLASH_SETUP == 1)
    LPC_SC->FLASHCFG = (LPC_SC->FLASHCFG & ~0x0000F000) | FLASHCFG_Val;
#endif
}

#define IRC_OSC (4000000UL)

uint32_t SystemFrequency = IRC_OSC;

/* System Clock Update */
void SystemClockUpdate(void) {

    if (((LPC_SC->PLL0STAT >> 24) & 3) == 3) {
        switch (LPC_SC->CLKSRCSEL & 0x03) {
        case 0:
        case 3:
            SystemFrequency = (IRC_OSC *
                               (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                                (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1)) /
                               ((LPC_SC->CCLKCFG & 0xFF) + 1));
            break;
        case 1:
            SystemFrequency = (OSC_CLK *
                               (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                                (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1)) /
                               ((LPC_SC->CCLKCFG & 0xFF) + 1));
            break;
        case 2:
            SystemFrequency = (RTC_CLK *
                               (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                                (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1)) /
                               ((LPC_SC->CCLKCFG & 0xFF) + 1));
            break;
        }

    } else {
        switch (LPC_SC->CLKSRCSEL & 0x03) {
        case 0:
        case 3:
            SystemFrequency = IRC_OSC / ((LPC_SC->CCLKCFG & 0xFF) + 1);
            break;
        case 1:
            SystemFrequency = OSC_CLK / ((LPC_SC->CCLKCFG & 0xFF) + 1);
            break;
        case 2:
            SystemFrequency = RTC_CLK / ((LPC_SC->CCLKCFG & 0xFF) + 1);
            break;
        }
    }
}
