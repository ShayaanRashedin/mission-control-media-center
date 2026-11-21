/* USB audio and speaker output. */
#include "LPC17xx.h"
#include "type.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbaudio.h"
#include "media.h"
extern void SystemClockUpdate(void);
extern uint32_t SystemFrequency;
extern volatile unsigned USB_HWError;
volatile uint8_t Mute;
volatile uint32_t Volume;
volatile uint16_t DataOut, DataIn;
volatile uint8_t DataRun;
volatile short DataBuf[B_S];
static unsigned active, stopped_error;
static unsigned connect_pending, disconnect_time;
static volatile unsigned test_tone;
static volatile unsigned knob = 128;
static uint32_t tone_phase;

/* Host volume, from -30 dB to 0 dB */
static const unsigned short gain[31] = {1036,  1163,  1305,  1464,  1642,  1843,  2068,  2320,
                                        2603,  2920,  3277,  3677,  4125,  4629,  5193,  5827,
                                        6538,  7336,  8231,  9235,  10362, 11627, 13045, 14637,
                                        16422, 18427, 20675, 23197, 26029, 29204, 32767};

/* Sample output */
void TIMER0_IRQHandler(void) {
    long sample = 0;
    unsigned out;
    LPC_TIM0->IR = 1;
    if (test_tone) {
        tone_phase += 440;
        if (tone_phase >= DATA_FREQ)
            tone_phase -= DATA_FREQ;
        sample = tone_phase < DATA_FREQ / 2 ? 6000 : -6000;
        sample = sample * (long)knob / 256;
    } else if (DataRun) {
        out = DataOut;
        if (out != DataIn) {
            sample = DataBuf[out];
            DataOut = (out + 1) & (B_S - 1);
            sample = sample * (long)Volume / 32768;
        } else
            DataRun = 0;
    }
    if (!test_tone && Mute)
        sample = 0;
    LPC_DAC->CR = (unsigned long)(sample + 32768) & 0xFFC0;
}

/* Stop the USB connection */
void audio_stop(void) {
    NVIC_DisableIRQ(USB_IRQn);
    NVIC_DisableIRQ(TIMER0_IRQn);
    test_tone = connect_pending = 0;
    LPC_TIM0->TCR = 0;
    if (active) {
        /* Disconnect even if a previous command timed out. */
        USB_HWError = 0;
        USB_Connect(FALSE);
        LPC_PINCON->PINSEL4 &= ~(3UL << 18);
        LPC_GPIO2->FIOSET = 1UL << 9;
        LPC_GPIO2->FIODIR |= 1UL << 9;
    }
    NVIC_ClearPendingIRQ(USB_IRQn);
    NVIC_ClearPendingIRQ(TIMER0_IRQn);
    DataRun = DataIn = DataOut = 0;
    Volume = 0;
    Mute = 1;
    active = 0;
    LPC_DAC->CR = 0x8000;
}

/* Start the player */
void audio_start(void) {
    unsigned i, div, pclk;
    active = stopped_error = connect_pending = 0;
    USB_HWError = 0;
    NVIC_DisableIRQ(USB_IRQn);
    NVIC_DisableIRQ(TIMER0_IRQn);
    DataRun = DataIn = DataOut = 0;
    Mute = test_tone = 0;
    Volume = tone_phase = 0;
    VolCur = 0;
    for (i = 0; i < B_S; i++)
        DataBuf[i] = 0;
    SystemClockUpdate();
    LPC_SC->PCONP |= (1UL << 1) | (1UL << 12);
    LPC_PINCON->PINSEL1 &= ~((3UL << 18) | (3UL << 20));
    LPC_PINCON->PINSEL1 |= (1UL << 18) | (2UL << 20);
    LPC_ADC->CR = 0x00200E04;
    LPC_ADC->CR |= 1UL << 24;
    LPC_DAC->CR = 0x8000;
    div = (LPC_SC->PCLKSEL0 >> 2) & 3;
    pclk = SystemFrequency / (div == 0 ? 4 : div == 1 ? 1 : div == 2 ? 2 : 8);
    LPC_TIM0->TCR = 2;
    LPC_TIM0->PR = 0;
    LPC_TIM0->MR0 = pclk / DATA_FREQ - 1;
    LPC_TIM0->MCR = 3;
    LPC_TIM0->IR = 0xFF;
    NVIC_ClearPendingIRQ(TIMER0_IRQn);
    NVIC_ClearPendingIRQ(USB_IRQn);
    NVIC_SetPriority(TIMER0_IRQn, 0);
    NVIC_SetPriority(USB_IRQn, 1);
    USB_ResetCore();
    USB_Init();
    active = 1;
    if (!USB_HWError) {
        /* Hold SoftConnect off before attaching again. */
        USB_Connect(FALSE);
        LPC_PINCON->PINSEL4 &= ~(3UL << 18);
        LPC_GPIO2->FIOSET = 1UL << 9;
        LPC_GPIO2->FIODIR |= 1UL << 9;
        connect_pending = 1;
    }
    /* The speaker test also works if USB setup fails. */
    LPC_TIM0->TCR = 1;
    NVIC_EnableIRQ(TIMER0_IRQn);
}

/* Foreground updates */
void audio_poll(unsigned now) {
    unsigned val, db;
    if (!active) return;
    if (connect_pending == 1) {
        disconnect_time = now;
        connect_pending = 2;
    } else if (connect_pending == 2 && now - disconnect_time >= 250) {
        connect_pending = 0;
        LPC_PINCON->PINSEL4 &= ~(3UL << 18);
        LPC_PINCON->PINSEL4 |= 1UL << 18;
        if (!USB_HWError) USB_Connect(TRUE);
        if (!USB_HWError) NVIC_EnableIRQ(USB_IRQn);
    }
    val = LPC_ADC->GDR;
    if (val & (1UL << 31)) {
        knob = 256 - ((val >> 4) & 0xFFF) / 16;
        LPC_ADC->CR &= ~(7UL << 24);
        LPC_ADC->CR |= 1UL << 24;
    }
    db = (unsigned)((-(int)(short)VolCur + 128) / 256);
    if (db > 30)
        db = 30;
    Volume = (unsigned)gain[30 - db] * knob / 256;
    if (USB_HWError && !stopped_error) {
        stopped_error = USB_HWError;
        NVIC_DisableIRQ(USB_IRQn);
        /* Leave the timer available for the speaker test. */
        DataRun = 0;
    }
}

/* Audio screen values */
void audio_test(void) {
    test_tone = !test_tone;
}
unsigned audio_error(void) {
    return stopped_error ? stopped_error : USB_HWError;
}
unsigned audio_volume(void) {
    return knob * 100 / 256;
}
unsigned audio_status(void) {
    if (test_tone)
        return 4;
    if (!active)
        return 0;
    if (connect_pending)
        return 5;
    if (DataRun)
        return 3;
    if (USB_Configuration)
        return 2;
    return 1;
}
