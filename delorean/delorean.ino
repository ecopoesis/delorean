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
#include "ST7565.h"

// pin 8 - Serial data out (SID) - orange
// pin 7 - Serial clock out (SCLK) - yellow
// pin 6 - Data/Command select (RS or A0) - green
// pin 5 - LCD reset (RST) - blue
// pin 4 - LCD chip select (CS) - white
ST7565 glcd(8, 7, 6, 5, 4);

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

// a bitmap of a 16x16 fruit icon
static unsigned char __attribute__ ((progmem)) logo16_glcd_bmp[]={
    0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00,
    0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };

// the LCD backlight is connected up to a pin so you can turn it on & off
#define BACKLIGHT_LED_R 9
#define BACKLIGHT_LED_G 10
#define BACKLIGHT_LED_B 11

void loop()
{}

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
    uint8_t icons[NUMFLAKES][3];
    randomSeed(666);     // whatever seed
    
    // initialize
    for (uint8_t f=0; f< NUMFLAKES; f++) {
        icons[f][XPOS] = random(128);
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
    }
    
    while (1) {
        // draw each icon
        for (uint8_t f=0; f< NUMFLAKES; f++) {
            glcd.drawbitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
        }
        glcd.display();
        delay(200);
        
        // then erase it + move it
        for (uint8_t f=0; f< NUMFLAKES; f++) {
            glcd.drawbitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, 0);
            // move it
            icons[f][YPOS] += icons[f][DELTAY];
            // if its gone, reinit
            if (icons[f][YPOS] > 64) {
                icons[f][XPOS] = random(128);
                icons[f][YPOS] = 0;
                icons[f][DELTAY] = random(5) + 1;
            }
        }
    }
}


void testdrawchar(void) {
    for (uint8_t i=0; i < 168; i++) {
        glcd.drawchar((i % 21) * 6, i/21, i);
    }
}

void testdrawcircle(void) {
    for (uint8_t i=0; i<64; i+=2) {
        glcd.drawcircle(63, 31, i, BLACK);
    }
}


void testdrawrect(void) {
    for (uint8_t i=0; i<64; i+=2) {
        glcd.drawrect(i, i, 128-i, 64-i, BLACK);
    }
}

void testfillrect(void) {
    for (uint8_t i=0; i<64; i++) {
        // alternate colors for moire effect
        glcd.fillrect(i, i, 128-i, 64-i, i%2);
    }
}

void testdrawline() {
    for (uint8_t i=0; i<128; i+=4) {
        glcd.drawline(0, 0, i, 63, BLACK);
    }
    for (uint8_t i=0; i<64; i+=4) {
        glcd.drawline(0, 0, 127, i, BLACK);
    }
    
    glcd.display();
    delay(1000);
    
    for (uint8_t i=0; i<128; i+=4) {
        glcd.drawline(i, 63, 0, 0, WHITE);
    }
    for (uint8_t i=0; i<64; i+=4) {
        glcd.drawline(127, i, 0, 0, WHITE);
    }
}

// The setup() method runs once, when the sketch starts
void setup()   {
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
    
    // initialize and set the contrast to 0x18
    glcd.begin(0x18);
    
    glcd.display(); // show splashscreen
    delay(2000);
    glcd.clear();
    
   
    // draw a single pixel
    glcd.setpixel(10, 10, BLACK);
    glcd.display();        // show the changes to the buffer
    delay(2000);
    glcd.clear();
    
    // draw many lines
    testdrawline();
    glcd.display();       // show the lines
    delay(2000);
    glcd.clear();
    
    // draw rectangles
    testdrawrect();
    glcd.display();
    delay(2000);
    glcd.clear();
    
    // draw multiple rectangles
    testfillrect();
    glcd.display();
    delay(2000);
    glcd.clear();
    
    // draw mulitple circles
    testdrawcircle();
    glcd.display();
    delay(2000);
    glcd.clear();
    
    // draw a black circle, 10 pixel radius, at location (32,32)
    glcd.fillcircle(32, 32, 10, BLACK);
    glcd.display();
    delay(2000);
    glcd.clear();
    
    // draw the first ~120 characters in the font
    testdrawchar();
    glcd.display();
    delay(2000);
    glcd.clear();
    
    // draw a string at location (0,0)
    glcd.drawstring(0, 0, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation");
    glcd.display();
    delay(2000);
    glcd.clear();
    
    // draw a bitmap icon and 'animate' movement
    testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}
