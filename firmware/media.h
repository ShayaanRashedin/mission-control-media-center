/* Menu controls. */
#ifndef MEDIA_H
#define MEDIA_H
enum { NONE, UP, DOWN, LEFT, RIGHT, SELECT };
enum { MENU, GALLERY, AUDIO, LANDER, RESCUE };
void media_init(void);
void media_key(unsigned key);
void media_update(unsigned now, unsigned held);
void audio_start(void);
void audio_stop(void);
void audio_poll(unsigned now);
void audio_test(void);
unsigned audio_status(void);
unsigned audio_error(void);
unsigned audio_volume(void);
#endif
