/// 
/// @mainpage	delorean 
/// @details	<#details#>
/// @n 
/// @n 
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Michael Roberts
/// @author	Michael Roberts
/// @date	1/7/13 9:10 PM
/// @version	<#version#>
/// 
/// @copyright	© Michael Roberts, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	delorean.ino 
/// @brief	Main sketch
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Michael Roberts
/// @author	Michael Roberts
/// @date	1/7/13 9:10 PM
/// @version	<#version#>
/// 
/// @copyright	© Michael Roberts, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
/// @n
///


// Core library for code-sense
#include "Arduino.h"
#include "WMath.h"

#include "Wakeup.h"
//#include "ST7565.h"
#include "U8glib.h"

//ST7565* glcd = NULL;
U8GLIB_LM6059* u8g = NULL;

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

// a bitmap of a 16x16 fruit icon
static unsigned char __attribute__ ((progmem)) logo16_glcd_bmp[]={
    0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00,
    0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };

// ST7565
#define ST7565_CS       4   // white/purple
#define ST7565_RST      5   // blue
#define ST7565_A0       6   // green
#define ST7565_SCK      7   // yellow
#define ST7565_MOSI     8   // orange

// LCD backlight
#define BACKLIGHT_LED_R 9
#define BACKLIGHT_LED_G 10
#define BACKLIGHT_LED_B 11

void drawColorBox()
{
    u8g_uint_t w,h;
    u8g_uint_t r, g, b;
    
    w = u8g->getWidth()/32;
    h = u8g->getHeight()/8;
    for( b = 0; b < 4; b++ )
        for( g = 0; g < 8; g++ )
            for( r = 0; r < 8; r++ )
            {
                u8g->setColorIndex((r<<5) |  (g<<2) | b );
                u8g->drawBox(g*w + b*w*8, r*h, w, h);
            }
}

void drawLogo(uint8_t d)
{
    u8g->setFont(u8g_font_helvB24r);
    u8g->drawStr(0+d, 30+d, "miker");
    
    u8g->drawHLine(2+d, 35+d, 47);
    u8g->drawVLine(45+d, 32+d, 12);
}

void drawURL(void)
{
    u8g->setFont(u8g_font_4x6);
    u8g->drawStr(1,54,"www.miker.org");
}


void draw(void) {
    if ( u8g->getMode() == U8G_MODE_R3G3B2 ) {
        drawColorBox();
    }
    u8g->setColorIndex(1);
    if ( U8G_MODE_GET_BITS_PER_PIXEL(u8g->getMode()) > 1 ) {
        drawLogo(2);
        u8g->setColorIndex(2);
        drawLogo(1);
        u8g->setColorIndex(3);
    }
    drawLogo(0);
    drawURL();
    
}


void setup(void) {
    Serial.begin(9600);
    Serial.println("hello");
    //Serial.print(freeRam());
    
    // turn on backlight
    pinMode(BACKLIGHT_LED_R, OUTPUT);
    pinMode(BACKLIGHT_LED_G, OUTPUT);
    pinMode(BACKLIGHT_LED_B, OUTPUT);
    
    digitalWrite(BACKLIGHT_LED_R, HIGH);
    digitalWrite(BACKLIGHT_LED_G, HIGH);
    digitalWrite(BACKLIGHT_LED_B, HIGH);
    
    Serial.println("red!");
    digitalWrite(BACKLIGHT_LED_R, LOW);
    delay(2000);
    Serial.println("green!");
    digitalWrite(BACKLIGHT_LED_G, LOW);
    delay(2000);
    Serial.println("blue!");
    digitalWrite(BACKLIGHT_LED_B, LOW);

    u8g = new U8GLIB_LM6059(ST7565_SCK, ST7565_MOSI, ST7565_CS, ST7565_A0, ST7565_RST);
    //glcd = new ST7565(ST7565_MOSI, ST7565_SCK, ST7565_A0, ST7565_RST, ST7565_CS);
    //glcd->begin(0x18);
    //glcd->display();
}

void loop(void) {
    Serial.println("loop");
    
    // picture loop
    u8g->firstPage();
    do {
        draw();
        u8g->setColorIndex(1);
    } while( u8g->nextPage() ); 
    
    // rebuild the picture after some delay
    delay(2000);
 
}

