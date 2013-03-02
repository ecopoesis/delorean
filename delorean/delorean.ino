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
#include <Wire.h>

#include "Wakeup.h"
#include "Adafruit_Sensor.h"
#include "U8glib.h"
#include "Adafruit_TSL2561.h"
//#include "ST7565.h"

//ST7565* glcd = NULL;
U8GLIB_LM6059* u8g = NULL;
Adafruit_TSL2561* tsl = NULL;

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

// I2C ADDR
#define I2C_LUMINOSITY  0x39
#define I2C_CHRONO      0x68

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


void draw(float lux) {
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
    
    u8g->setFont(u8g_font_4x6);
    char luxbuffer[9];
    sprintf(luxbuffer, "Lux: %2.0f", lux);
    u8g->drawStr(90,6,luxbuffer);
}


void setup(void) {
    Serial.begin(9600);
    Wire.begin();
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
    
    // setup chrono
    Wire.beginTransmission(I2C_CHRONO); // address DS3231
    Wire.write(0x0E); // select register
    Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
    Wire.endTransmission();
    
    // setup luminosity
    tsl = new Adafruit_TSL2561(I2C_LUMINOSITY);
    if (tsl->begin()) {
        Serial.println("starting luminosity sensor");
    } else {
        Serial.println("could not find luminosity sensor");
    }
    
    tsl->enableAutoGain(true);
    tsl->setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
}

void loop(void) {
    Serial.println("loop");

    // send request to receive data starting at register 0
    Wire.beginTransmission(I2C_CHRONO); // 0x68 is DS3231 device address
    Wire.write((byte)0); // start at register 0
    Wire.endTransmission();
    Wire.requestFrom(I2C_CHRONO, 3); // request three bytes (seconds, minutes, hours)

    while(Wire.available())
    {
        int seconds = Wire.read(); // get seconds
        int minutes = Wire.read(); // get minutes
        int hours = Wire.read();   // get hours
        
        seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111)); // convert BCD to decimal
        minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111)); // convert BCD to decimal
        hours = (((hours & 0b00100000)>>5)*20 + ((hours & 0b00010000)>>4)*10 + (hours & 0b00001111)); // convert BCD to decimal (assume 24 hour mode)
        
        Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);
    }
    
    sensors_event_t event;
    tsl->getEvent(&event);
/*
    if (event.light) {

        Serial.print(event.light); Serial.println(" lux");
    } else {
        Serial.println("Luminosity sensor overload");
    } */
    
    // picture loop
    u8g->firstPage();
    do {
        draw(event.light);
        u8g->setColorIndex(1);
    } while(u8g->nextPage());  
}

