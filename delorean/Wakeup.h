/*
  Handle the wakeup light
*/

// Core library - IDE-based
#if defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0x and 1.5x specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100)  // Arduino 23 specific
#include "WProgram.h"
#endif // end IDE

#ifndef Wakeup_h
#define Wakeup_h

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long int

class Wakeup {
  public:
    Wakeup(int clock, int data);
    void set(uint8 r, uint8 g, uint8 b);
  private:
    int _clock;
    int _data;
    void sendBlankFrame(void);
    uint8 anti(uint8 dat);
    void sendPacket(uint32 dx);
    void sendColor(uint8 r, uint8 g, uint8 b);
    void clockProduce(void);
};

#endif
