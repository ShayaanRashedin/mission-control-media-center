# Setup and controls

## Hardware and tools

- Keil MCB1700 board populated with an NXP LPC1768.
- On-board color LCD, five-way joystick, potentiometer, and speaker.
- Keil MDK / uVision and Arm Compiler 5. The saved project selects V5.06 update 6 (build 750).
- Keil LPC1700_DFP 2.6.0 device support, as recorded in the project.
- Compatible ULINK debug connection.

The project uses compiler-specific startup and CMSIS support. It has not been migrated to Arm Compiler 6 or a GCC build. A compatible licensed Keil installation may be required depending on the installed edition's code-size limits.

## Build

Open `firmware/MediaCenter.uvprojx`, select `MediaCenter`, and build. Keep the source files together in `firmware/` because the project uses local include paths and `DMA.sct` for memory layout. Despite the scatter file's historical name, USB DMA is disabled in the current configuration.

The project configures the LPC1768, 512 KB flash, and a 32 KB main RAM region. The audio ring buffer contains 1,024 16-bit samples. The three 160 x 120 RGB565 gallery images occupy 115,200 bytes of pixel data in constant storage.

Download using the project's ULINK configuration and run on the board. Generated objects, listings, binaries, and per-user debugger settings are excluded from version control.

## Controls

| Screen | Control | Action |
| --- | --- | --- |
| Menu | Up / Down | Choose a feature |
| Menu | Centre | Open the selection |
| Gallery | Left / Right | Browse images |
| Any feature | Centre | Return to the menu |
| Lunar Lander | Up | Apply thrust |
| Lunar Lander | Left / Right | Change horizontal velocity |
| Robot Rescue | Directions | Move one grid cell |
| Level-clear screen | Right | Advance to level 2 |
| Final result or failure | Right | Restart from level 1 |
| Audio | Potentiometer | Counterclockwise increases volume, clockwise decreases it |
| Audio | Right | Toggle the local speaker test |
| Audio | Left | Retry USB initialization |

## Input and timing

Joystick switches are active low on GPIO1. The application maps P1.23 to Up, P1.25 to Down, P1.24 to Right, P1.26 to Left, and P1.20 to Centre. Input must remain stable for 25 ms before a new press is accepted.

Games use nominal 50 ms updates. A held direction drives gameplay while new press events drive menu and result-screen actions. Drawing occurs in the foreground. SysTick supplies milliseconds, and Timer 0 handles audio samples at a configured 32 kHz rate.

## USB audio playback

The configured USB audio format is 32 kHz mono 16-bit PCM. The host PC decodes compressed audio and sends PCM samples to the board. The board-side code provides sample buffering, host mute/volume requests, ADC-controlled gain, and DAC output.

PC-to-board USB audio playback was verified through the on-board speaker. The separate local test tone and potentiometer adjustment were also verified. Host-observed disconnect and reconnect behaviour was outside the playback check.

## Reproducing the board demonstration

1. Open and browse all three gallery images, then return to the menu.
2. Play Lunar Lander through both levels and observe a hard landing as well as a successful landing.
3. Play Robot Rescue through both maps, collect supplies, and observe completion and damage-related failure screens.
4. Open the Audio screen. Toggle the local test tone and adjust the potentiometer to check the speaker path, then turn the tone off.
5. Connect the board to the PC via its USB device port. Select the board as the PC's audio output and play audio. Confirm the PC audio is audible through the board's speaker.

The photos in `docs/images/` record the original board trials. They show different runs, so displayed scores are not a single continuous playthrough.
