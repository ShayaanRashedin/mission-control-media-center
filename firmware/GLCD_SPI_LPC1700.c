/* Control the LCD and draw text and pictures. */

/* Includes */
#include "LPC17xx.h"
#include "GLCD.h"
#include "Font_6x8_h.h"
#include "Font_16x24_h.h"

#define LANDSCAPE 1
#define ROTATE180 0

#define PIN_CS (1 << 6)
#define PIN_CLK (1 << 7)
#define PIN_DAT (1 << 9)

#define IN 0x00
#define OUT 0x01

#define TFE 0x01
#define RNE 0x04
#define BSY 0x10

#define DELAY_2N 18

#if (LANDSCAPE == 1)
#define WIDTH 320
#define HEIGHT 240
#else
#define WIDTH 240
#define HEIGHT 320
#endif
#define BPP 16
#define BYPP ((BPP + 7) / 8)

#define LCD_CS(x) ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS))
#define LCD_CLK(x) ((x) ? (LPC_GPIO0->FIOSET = PIN_CLK) : (LPC_GPIO0->FIOCLR = PIN_CLK))
#define LCD_DAT(x) ((x) ? (LPC_GPIO0->FIOSET = PIN_DAT) : (LPC_GPIO0->FIOCLR = PIN_DAT))

#define DAT_MODE(x) ((x == OUT) ? (LPC_GPIO0->FIODIR |= PIN_DAT) : (LPC_GPIO0->FIODIR &= ~PIN_DAT))
#define BUS_VAL() ((LPC_GPIO0->FIOPIN & PIN_DAT) != 0)

#define SPI_START (0x70)
#define SPI_RD (0x01)
#define SPI_WR (0x00)
#define SPI_DATA (0x02)
#define SPI_INDEX (0x00)

#define BG_COLOR 0
#define TXT_COLOR 1

static volatile unsigned short Color[2] = {White, Black};
static unsigned char Himax;

/* delay */
static void delay(int cnt) {
    cnt <<= DELAY_2N;
    while (cnt--)
        ;
}

/* spi tran man */
static unsigned char spi_tran_man(unsigned char byte, unsigned int mode) {
    unsigned char val = 0;
    int i;

    if (mode == OUT) {
        DAT_MODE(OUT);
    } else {
        DAT_MODE(IN);
    }

    for (i = 7; i >= 0; i--) {
        LCD_CLK(0);
        delay(1);
        if (mode == OUT) {
            LCD_DAT((byte & (1 << i)) != 0);
        } else {
            val |= (BUS_VAL() << i);
        }
        LCD_CLK(1);
        delay(1);
    }
    return (val);
}

/* spi tran */
static __inline unsigned char spi_tran(unsigned char byte) {

    LPC_SSP1->DR = byte;
    while (!(LPC_SSP1->SR & RNE))
        ;
    return (LPC_SSP1->DR);
}

/* wr cmd */
static __inline void wr_cmd(unsigned char cmd) {
    LCD_CS(0);
    spi_tran(SPI_START | SPI_WR | SPI_INDEX);
    spi_tran(0);
    spi_tran(cmd);
    LCD_CS(1);
}

/* wr dat */
static __inline void wr_dat(unsigned short dat) {
    LCD_CS(0);
    spi_tran(SPI_START | SPI_WR | SPI_DATA);
    spi_tran((dat >> 8));
    spi_tran((dat & 0xFF));
    LCD_CS(1);
}

/* wr dat start */
static __inline void wr_dat_start(void) {
    LCD_CS(0);
    spi_tran(SPI_START | SPI_WR | SPI_DATA);
}

/* wr dat stop */
static __inline void wr_dat_stop(void) {

    LCD_CS(1);
}

/* wr dat only */
static __inline void wr_dat_only(unsigned short dat) {

    spi_tran((dat >> 8));
    spi_tran((dat & 0xFF));
}

/* rd dat */
static __inline unsigned short rd_dat(void) {
    unsigned short val = 0;

    LCD_CS(0);
    spi_tran(SPI_START | SPI_RD | SPI_DATA);
    spi_tran(0);
    val = spi_tran(0);
    val <<= 8;
    val |= spi_tran(0);
    LCD_CS(1);
    return (val);
}

/* wr reg */
static __inline void wr_reg(unsigned char reg, unsigned short val) {

    wr_cmd(reg);
    wr_dat(val);
}

/* rd reg */
static unsigned short rd_reg(unsigned char reg) {

    wr_cmd(reg);
    return (rd_dat());
}

/* rd id man */
static unsigned short rd_id_man(void) {
    unsigned short val;

    LPC_PINCON->PINSEL0 &= ~((3 << 18) | (3 << 16) | (3 << 14));
    LPC_PINCON->PINMODE0 |= 0x000AA000;
    LPC_GPIO0->FIODIR |= PIN_CLK;
    LCD_CS(1);
    LCD_CLK(1);

    LCD_CS(0);
    spi_tran_man(SPI_START | SPI_WR | SPI_INDEX, OUT);
    spi_tran_man(0x00, OUT);
    LCD_CS(1);

    LCD_CS(0);
    spi_tran_man(SPI_START | SPI_RD | SPI_DATA, OUT);
    val = spi_tran_man(0, IN);
    LCD_CS(1);

    LPC_GPIO0->FIODIR &= ~PIN_CLK;
    LPC_PINCON->PINSEL0 |= (2 << 18) | (2 << 16) | (2 << 14);
    LPC_PINCON->PINMODE0 &= ~0x000FF000;

    return (val);
}

/* GLCD Init */
void GLCD_Init(void) {
    unsigned short driverCode;

    LPC_SC->PCONP |= 0x00000400;
    LPC_SC->PCLKSEL0 |= 0x00200000;

    LPC_PINCON->PINSEL9 &= 0xF0FFFFFF;
    LPC_GPIO4->FIODIR |= 0x30000000;
    LPC_GPIO4->FIOSET = 0x20000000;

    LPC_GPIO0->FIODIR |= 0x00000040;
    LPC_GPIO0->FIOSET = 0x00000040;
    LPC_PINCON->PINSEL0 &= 0xFFF03FFF;
    LPC_PINCON->PINSEL0 |= 0x000A8000;

    LPC_SSP1->CR0 = 0x01C7;
    LPC_SSP1->CPSR = 0x02;
    LPC_SSP1->CR1 = 0x02;

    driverCode = rd_id_man();
    if (driverCode == 0) {
        driverCode = rd_reg(0x00);
    }

    if (driverCode == 0x47) {
        Himax = 1;

        wr_reg(0xEA, 0x00);
        wr_reg(0xEB, 0x20);
        wr_reg(0xEC, 0x0C);
        wr_reg(0xED, 0xC7);
        wr_reg(0xE8, 0x38);
        wr_reg(0xE9, 0x10);
        wr_reg(0xF1, 0x01);
        wr_reg(0xF2, 0x10);

        wr_reg(0x40, 0x01);
        wr_reg(0x41, 0x00);
        wr_reg(0x42, 0x00);
        wr_reg(0x43, 0x10);
        wr_reg(0x44, 0x0E);
        wr_reg(0x45, 0x24);
        wr_reg(0x46, 0x04);
        wr_reg(0x47, 0x50);
        wr_reg(0x48, 0x02);
        wr_reg(0x49, 0x13);
        wr_reg(0x4A, 0x19);
        wr_reg(0x4B, 0x19);
        wr_reg(0x4C, 0x16);

        wr_reg(0x50, 0x1B);
        wr_reg(0x51, 0x31);
        wr_reg(0x52, 0x2F);
        wr_reg(0x53, 0x3F);
        wr_reg(0x54, 0x3F);
        wr_reg(0x55, 0x3E);
        wr_reg(0x56, 0x2F);
        wr_reg(0x57, 0x7B);
        wr_reg(0x58, 0x09);
        wr_reg(0x59, 0x06);
        wr_reg(0x5A, 0x06);
        wr_reg(0x5B, 0x0C);
        wr_reg(0x5C, 0x1D);
        wr_reg(0x5D, 0xCC);

        wr_reg(0x1B, 0x1B);
        wr_reg(0x1A, 0x01);
        wr_reg(0x24, 0x2F);
        wr_reg(0x25, 0x57);
        wr_reg(0x23, 0x88);

        wr_reg(0x18, 0x36);
        wr_reg(0x19, 0x01);
        wr_reg(0x01, 0x00);
        wr_reg(0x1F, 0x88);
        delay(20);
        wr_reg(0x1F, 0x82);
        delay(5);
        wr_reg(0x1F, 0x92);
        delay(5);
        wr_reg(0x1F, 0xD2);
        delay(5);

        wr_reg(0x17, 0x55);
        wr_reg(0x00, 0x00);

        wr_reg(0x2F, 0x11);
        wr_reg(0x31, 0x00);
        wr_reg(0x32, 0x00);

        wr_reg(0x28, 0x38);
        delay(20);
        wr_reg(0x28, 0x3C);

#if (LANDSCAPE == 1)
#if (ROTATE180 == 0)
        wr_reg(0x16, 0xA8);
#else
        wr_reg(0x16, 0x68);
#endif
#else
#if (ROTATE180 == 0)
        wr_reg(0x16, 0x08);
#else
        wr_reg(0x16, 0xC8);
#endif
#endif

        wr_reg(0x0E, 0x00);
        wr_reg(0x0F, 0x00);
        wr_reg(0x10, 320 >> 8);
        wr_reg(0x11, 320 & 0xFF);
        wr_reg(0x12, 0x00);
        wr_reg(0x13, 0x00);
    } else {
        Himax = 0;

#if (ROTATE180 == 1)
        wr_reg(0x01, 0x0000);
#else
        wr_reg(0x01, 0x0100);
#endif
        wr_reg(0x02, 0x0700);
        wr_reg(0x04, 0x0000);
        wr_reg(0x08, 0x0207);
        wr_reg(0x09, 0x0000);
        wr_reg(0x0A, 0x0000);
        wr_reg(0x0C, 0x0000);
        wr_reg(0x0D, 0x0000);
        wr_reg(0x0F, 0x0000);

        wr_reg(0x10, 0x0000);
        wr_reg(0x11, 0x0000);
        wr_reg(0x12, 0x0000);
        wr_reg(0x13, 0x0000);
        delay(20);
        wr_reg(0x10, 0x12B0);
        wr_reg(0x11, 0x0007);
        delay(5);
        wr_reg(0x12, 0x01BD);
        delay(5);
        wr_reg(0x13, 0x1400);
        wr_reg(0x29, 0x000E);
        delay(5);
        wr_reg(0x20, 0x0000);
        wr_reg(0x21, 0x0000);

        switch (driverCode) {
        case 0x5408:
            wr_reg(0x30, 0x0B0D);
            wr_reg(0x31, 0x1923);
            wr_reg(0x32, 0x1C26);
            wr_reg(0x33, 0x261C);
            wr_reg(0x34, 0x2419);
            wr_reg(0x35, 0x0D0B);
            wr_reg(0x36, 0x1006);
            wr_reg(0x37, 0x0610);
            wr_reg(0x38, 0x0706);
            wr_reg(0x39, 0x0304);
            wr_reg(0x3A, 0x0E05);
            wr_reg(0x3B, 0x0E01);
            wr_reg(0x3C, 0x010E);
            wr_reg(0x3D, 0x050E);
            wr_reg(0x3E, 0x0403);
            wr_reg(0x3F, 0x0607);
            break;

        case 0x9325:
            wr_reg(0x0030, 0x0000);
            wr_reg(0x0031, 0x0607);
            wr_reg(0x0032, 0x0305);
            wr_reg(0x0035, 0x0000);
            wr_reg(0x0036, 0x1604);
            wr_reg(0x0037, 0x0204);
            wr_reg(0x0038, 0x0001);
            wr_reg(0x0039, 0x0707);
            wr_reg(0x003C, 0x0000);
            wr_reg(0x003D, 0x000F);
            break;

        case 0x9320:
        default:
            wr_reg(0x30, 0x0006);
            wr_reg(0x31, 0x0101);
            wr_reg(0x32, 0x0003);
            wr_reg(0x35, 0x0106);
            wr_reg(0x36, 0x0B02);
            wr_reg(0x37, 0x0302);
            wr_reg(0x38, 0x0707);
            wr_reg(0x39, 0x0007);
            wr_reg(0x3C, 0x0600);
            wr_reg(0x3D, 0x020B);
            break;
        }

        wr_reg(0x50, 0x0000);
        wr_reg(0x51, (HEIGHT - 1));
        wr_reg(0x52, 0x0000);
        wr_reg(0x53, (WIDTH - 1));

        switch (driverCode) {
        case 0x5408:
        case 0x9325:
#if (LANDSCAPE ^ ROTATE180)
            wr_reg(0x60, 0x2700);
#else
            wr_reg(0x60, 0xA700);
#endif
            break;

        case 0x9320:
        default:
#if (LANDSCAPE ^ ROTATE180)
            wr_reg(0x60, 0xA700);
#else
            wr_reg(0x60, 0x2700);
#endif
            break;
        }
        wr_reg(0x61, 0x0001);
        wr_reg(0x6A, 0x0000);

        wr_reg(0x80, 0x0000);
        wr_reg(0x81, 0x0000);
        wr_reg(0x82, 0x0000);
        wr_reg(0x83, 0x0000);
        wr_reg(0x84, 0x0000);
        wr_reg(0x85, 0x0000);

        wr_reg(0x90, 0x0010);
        wr_reg(0x92, 0x0000);
        wr_reg(0x93, 0x0003);
        wr_reg(0x95, 0x0110);
        wr_reg(0x97, 0x0000);
        wr_reg(0x98, 0x0000);

#if (LANDSCAPE == 1)

        wr_reg(0x03, 0x1038);
#else

        wr_reg(0x03, 0x1030);
#endif

        wr_reg(0x07, 0x0137);
    }
    LPC_GPIO4->FIOSET = 0x10000000;
}

/* GLCD Set Window */
void GLCD_SetWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    unsigned int xe, ye;

    if (Himax) {
        xe = x + w - 1;
        ye = y + h - 1;

        wr_reg(0x02, x >> 8);
        wr_reg(0x03, x & 0xFF);
        wr_reg(0x04, xe >> 8);
        wr_reg(0x05, xe & 0xFF);

        wr_reg(0x06, y >> 8);
        wr_reg(0x07, y & 0xFF);
        wr_reg(0x08, ye >> 8);
        wr_reg(0x09, ye & 0xFF);
    } else {
#if (LANDSCAPE == 1)
        wr_reg(0x50, y);
        wr_reg(0x51, y + h - 1);
        wr_reg(0x52, x);
        wr_reg(0x53, x + w - 1);
        wr_reg(0x20, y);
        wr_reg(0x21, x);
#else
        wr_reg(0x50, x);
        wr_reg(0x51, x + w - 1);
        wr_reg(0x52, y);
        wr_reg(0x53, y + h - 1);
        wr_reg(0x20, x);
        wr_reg(0x21, y);
#endif
    }
}

/* GLCD Window Max */
void GLCD_WindowMax(void) {
    GLCD_SetWindow(0, 0, WIDTH, HEIGHT);
}

/* GLCD Put Pixel */
void GLCD_PutPixel(unsigned int x, unsigned int y) {

    if (Himax) {
        wr_reg(0x02, x >> 8);
        wr_reg(0x03, x & 0xFF);
        wr_reg(0x04, x >> 8);
        wr_reg(0x05, x & 0xFF);

        wr_reg(0x06, y >> 8);
        wr_reg(0x07, y & 0xFF);
        wr_reg(0x08, y >> 8);
        wr_reg(0x09, y & 0xFF);
    } else {
#if (LANDSCAPE == 1)
        wr_reg(0x20, y);
        wr_reg(0x21, x);
#else
        wr_reg(0x20, x);
        wr_reg(0x21, y);
#endif
    }

    wr_cmd(0x22);
    wr_dat(Color[TXT_COLOR]);
}

/* GLCD Set Text Color */
void GLCD_SetTextColor(unsigned short color) {

    Color[TXT_COLOR] = color;
}

/* GLCD Set Back Color */
void GLCD_SetBackColor(unsigned short color) {

    Color[BG_COLOR] = color;
}

/* GLCD Clear */
void GLCD_Clear(unsigned short color) {
    unsigned int i;

    GLCD_WindowMax();
    wr_cmd(0x22);
    wr_dat_start();

    for (i = 0; i < (WIDTH * HEIGHT); i++)
        wr_dat_only(color);
    wr_dat_stop();
}

/* GLCD Draw Char */
void GLCD_DrawChar(unsigned int x, unsigned int y, unsigned int cw, unsigned int ch,
                   unsigned char *c) {
    unsigned int i, j, k, pixs;

    GLCD_SetWindow(x, y, cw, ch);

    wr_cmd(0x22);
    wr_dat_start();

    k = (cw + 7) / 8;

    if (k == 1) {
        for (j = 0; j < ch; j++) {
            pixs = *(unsigned char *)c;
            c += 1;

            for (i = 0; i < cw; i++) {
                wr_dat_only(Color[(pixs >> i) & 1]);
            }
        }
    } else if (k == 2) {
        for (j = 0; j < ch; j++) {
            pixs = *(unsigned short *)c;
            c += 2;

            for (i = 0; i < cw; i++) {
                wr_dat_only(Color[(pixs >> i) & 1]);
            }
        }
    }
    wr_dat_stop();
}

/* GLCD Display Char */
void GLCD_DisplayChar(unsigned int ln, unsigned int col, unsigned char fi, unsigned char c) {

    c -= 32;
    switch (fi) {
    case 0:
        GLCD_DrawChar(col * 6, ln * 8, 6, 8, (unsigned char *)&Font_6x8_h[c * 8]);
        break;
    case 1:
        GLCD_DrawChar(col * 16, ln * 24, 16, 24, (unsigned char *)&Font_16x24_h[c * 24]);
        break;
    }
}

/* GLCD Display String */
void GLCD_DisplayString(unsigned int ln, unsigned int col, unsigned char fi, unsigned char *s) {

    unsigned cw = fi ? 16 : 6, ch = fi ? 24 : 8;
    if (ln >= HEIGHT / ch)
        return;
    while (*s && col < WIDTH / cw) {
        GLCD_DisplayChar(ln, col++, fi, *s++);
    }
}

/* GLCD Clear Ln */
void GLCD_ClearLn(unsigned int ln, unsigned char fi) {
    unsigned height = fi ? 24 : 8;
    if (ln < HEIGHT / height)
        GLCD_FillRect(0, ln * height, WIDTH, height, Color[BG_COLOR]);
}

/* GLCD Bargraph */
void GLCD_Bargraph(unsigned int x, unsigned int y, unsigned int w, unsigned int h,
                   unsigned int val) {
    int i, j;

    val = (val * w) >> 10;
    GLCD_SetWindow(x, y, w, h);
    wr_cmd(0x22);
    wr_dat_start();
    for (i = 0; i < h; i++) {
        for (j = 0; j <= w - 1; j++) {
            if (j >= val) {
                wr_dat_only(Color[BG_COLOR]);
            } else {
                wr_dat_only(Color[TXT_COLOR]);
            }
        }
    }
    wr_dat_stop();
}

/* GLCD Bitmap */
void GLCD_Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h,
                 unsigned char *bitmap) {
    int i, j;
    unsigned short *bitmap_ptr = (unsigned short *)bitmap;

    GLCD_SetWindow(x, y, w, h);

    wr_cmd(0x22);
    wr_dat_start();
    for (i = (h - 1) * w; i > -1; i -= w) {
        for (j = 0; j < w; j++) {
            wr_dat_only(bitmap_ptr[i + j]);
        }
    }
    wr_dat_stop();
}

/* GLCD Scroll Vertical */
void GLCD_ScrollVertical(unsigned int dy) {
#if (LANDSCAPE == 0)
    static unsigned int y = 0;

    y = y + dy;
    while (y >= HEIGHT)
        y -= HEIGHT;

    if (Himax) {
        wr_reg(0x01, 0x08);
        wr_reg(0x14, y >> 8);
        wr_reg(0x15, y & 0xFF);
    } else {
        wr_reg(0x6A, y);
        wr_reg(0x61, 3);
    }
#endif
}

/* GLCD Wr Cmd */
void GLCD_WrCmd(unsigned char cmd) {
    wr_cmd(cmd);
}

/* GLCD Wr Reg */
void GLCD_WrReg(unsigned char reg, unsigned short val) {
    wr_reg(reg, val);
}

/* Filled rectangle */
void GLCD_FillRect(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short colour) {
    unsigned n;
    if (x >= WIDTH || y >= HEIGHT || !w || !h)
        return;
    if (w > WIDTH - x)
        w = WIDTH - x;
    if (h > HEIGHT - y)
        h = HEIGHT - y;
    GLCD_SetWindow(x, y, w, h);
    wr_cmd(0x22);
    wr_dat_start();
    for (n = 0; n < w * h; n++)
        wr_dat_only(colour);
    wr_dat_stop();
    GLCD_WindowMax();
}
