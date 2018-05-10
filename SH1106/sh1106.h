/*
 * sh1106.h
 *
 *  Created on: 14 gru 2017
 *      Author: Milo
 */

#ifndef SH1106_H_
#define SH1106_H_
//DEFINICJE PREPORCESORA

//PINY
#define MOSI (1<<3) //DIN
#define SCK (1<<4) // CLK
#define CS (1<<5) //CS
#define DC (1<<6) // D/C
#define RST (1<<7) // RST

 //PORTY
#define SCK_PORT	PORTD
#define SCK_DDR 	DDRD

#define MOSI_PORT	PORTD
#define MOSI_DDR 	DDRD

#define RST_PORT	PORTD
#define RST_DDR 	DDRD

#define DC_PORT		PORTD
#define DC_DDR 		DDRD

#define CS_PORT		PORTD
#define CS_DDR 		DDRD

//STAN PORTOW
#define SCK_LO	SCK_PORT &= ~SCK
#define SCK_HI 	SCK_PORT |= SCK

#define MOSI_LO	MOSI_PORT &= ~MOSI
#define MOSI_HI MOSI_PORT |= MOSI

#define RST_LO	RST_PORT &= ~RST
#define RST_HI 	RST_PORT |= RST

#define DC_LO	DC_PORT &= ~DC
#define DC_HI 	DC_PORT |= DC

#define CS_LO	CS_PORT &= ~CS
#define CS_HI 	CS_PORT |= CS

#define SH1106_SETCONTRAST 0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON 0xA5
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF

#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA

#define SH1106_SETVCOMDETECT 0xDB

#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9

#define SH1106_SETMULTIPLEX 0xA8

#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10

#define SH1106_SETSTARTLINE 0x40

#define SH1106_MEMORYMODE 0x20
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR   0x22

#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8

#define SH1106_SEGREMAP 0xA0

#define SH1106_CHARGEPUMP 0x8D

#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SH1106_ACTIVATE_SCROLL 0x2F
#define SH1106_DEACTIVATE_SCROLL 0x2E
#define SH1106_SET_VERTICAL_SCROLL_AREA 0xA3
#define SH1106_RIGHT_HORIZONTAL_SCROLL 0x26
#define SH1106_LEFT_HORIZONTAL_SCROLL 0x27
#define SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

//DEKLARACJE ZMIENNYCH
uint8_t oled_buf[1024];
const uint8_t bitmap_1[1024];
const uint8_t Font3216[13][64];
//DEKLARACJE FUNKCJI
void SH1106_command(uint8_t);
void sh1106_data(uint8_t);
void sh1106_begin_spi();
void sh1106_begin();
void sh1106_clear(uint8_t* bufor);
void sh1106_display();
void display_ada(void);
void bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width_bitmap, uint8_t height_bitmap, uint8_t* bufor);
void SH1106_char3216(uint8_t x, uint8_t y, uint8_t chChar, uint8_t* buffer);
void SH1106_string(uint8_t x, uint8_t y, const char *pString, uint8_t Size, uint8_t Mode, uint8_t* buffer);
void SH1106_char(uint8_t x, uint8_t y, uint8_t acsii, uint8_t size, uint8_t mode, uint8_t* buffer);

void sh1106_pixel_mirek(int x, int y, uint8_t bw, uint8_t* bufor);
void sh1106_clear1(uint8_t* buffer);
void sh1106_pixel(int x, int y, char color, uint8_t* bufer);
void sh1106_display2(uint8_t* buffer);

#endif /* SH1106_H_ */
