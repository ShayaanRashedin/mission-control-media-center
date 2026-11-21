/* Board setup and joystick input. */
#include "LPC17xx.h"
#include "GLCD.h"
#include "media.h"
#include "games.h"
extern void SystemClockUpdate(void);
extern uint32_t SystemFrequency;
static volatile unsigned ticks;

/* Millisecond clock */
void SysTick_Handler(void) {
    ticks++;
}

/* Joystick buttons */
static unsigned read_buttons(void) {
    unsigned pins = ~LPC_GPIO1->FIOPIN;
    unsigned keys = 0;
    if (pins & (1UL << 23))
        keys |= KEY_UP;
    if (pins & (1UL << 25))
        keys |= KEY_DOWN;
    if (pins & (1UL << 24))
        keys |= KEY_RIGHT;
    if (pins & (1UL << 26))
        keys |= KEY_LEFT;
    if (pins & (1UL << 20))
        keys |= KEY_SELECT;
    return keys;
}

/* Main loop */
int main(void) {
    unsigned last = 0, stable = 0, since = 0, now, keys, pressed, k;
    SystemClockUpdate();
    LPC_SC->PCONP |= 1UL << 15;
    LPC_PINCON->PINSEL3 &= ~0x003FC300UL;
    LPC_PINCON->PINMODE3 &= ~0x003FC300UL;
    LPC_GPIO1->FIODIR &= ~0x07900000UL;
    GLCD_Init();
    media_init();
    if (SysTick_Config(SystemFrequency / 1000))
        while (1) {
        }
    NVIC_SetPriority(SysTick_IRQn, 3);
    while (1) {
        now = ticks;
        keys = read_buttons();
        if (keys != last) {
            last = keys;
            since = now;
        }
        if (now - since >= 25 && keys != stable) {
            pressed = keys & ~stable;
            stable = keys;
            if (pressed & KEY_SELECT)
                media_key(SELECT);
            else
                for (k = UP; k <= RIGHT; k++)
                    if (pressed & (1U << k)) {
                        media_key(k);
                        break;
                    }
        }
        media_update(now, stable);
    }
}
