/* Declare LCD functions and colours. */

#ifndef _GLCD_H
#define _GLCD_H

/* Definitions */
#define Black 0x0000
#define Navy 0x000F
#define DarkGreen 0x03E0
#define DarkCyan 0x03EF
#define Maroon 0x7800
#define Purple 0x780F
#define Olive 0x7BE0
#define LightGrey 0xC618
#define DarkGrey 0x7BEF
#define Blue 0x001F
#define Green 0x07E0
#define Cyan 0x07FF
#define Red 0xF800
#define Magenta 0xF81F
#define Yellow 0xFFE0
#define White 0xFFFF

/* Shared declarations */
extern void GLCD_FillRect(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short colour);
extern void GLCD_Init(void);
extern void GLCD_WindowMax(void);
extern void GLCD_PutPixel(unsigned int x, unsigned int y);
extern void GLCD_SetTextColor(unsigned short color);
extern void GLCD_SetBackColor(unsigned short color);
extern void GLCD_Clear(unsigned short color);
extern void GLCD_DrawChar(unsigned int x, unsigned int y, unsigned int cw, unsigned int ch,
                          unsigned char *c);
extern void GLCD_DisplayChar(unsigned int ln, unsigned int col, unsigned char fi, unsigned char c);
extern void GLCD_DisplayString(unsigned int ln, unsigned int col, unsigned char fi,
                               unsigned char *s);
extern void GLCD_ClearLn(unsigned int ln, unsigned char fi);
extern void GLCD_Bargraph(unsigned int x, unsigned int y, unsigned int w, unsigned int h,
                          unsigned int val);
extern void GLCD_Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h,
                        unsigned char *bitmap);
extern void GLCD_ScrollVertical(unsigned int dy);

extern void GLCD_WrCmd(unsigned char cmd);
extern void GLCD_WrReg(unsigned char reg, unsigned short val);

#endif
