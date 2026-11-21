/* Audio settings and shared buffers. */
#ifndef USBAUDIO_H
#define USBAUDIO_H
#define DATA_FREQ 32000
#define P_S 32
#define P_C 1
#define B_S 1024
extern volatile uint8_t Mute;
extern volatile uint32_t Volume;
extern volatile uint16_t VolCur;
extern volatile short DataBuf[B_S];
extern volatile uint16_t DataOut, DataIn;
extern volatile uint8_t DataRun;
#endif
