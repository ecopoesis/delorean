#include "Arduino.h"
#include "Wakeup.h"

Wakeup::Wakeup(int clock, int data) {
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  _clock = clock;
  _data = data;
}

void Wakeup::set(uint8 r, uint8 g, uint8 b) {
  sendBlankFrame();
  sendColor(r, g, b);
  sendColor(r, g, b);
  sendColor(r, g, b);
  sendBlankFrame(); 
}

void Wakeup::sendBlankFrame(void) {
  uint8 i;
 
  for (i=0; i<32; i++) {
    digitalWrite(_data, LOW);
    clockProduce();
  }
}

void Wakeup::clockProduce(void) {
  digitalWrite(_clock, LOW);
  delayMicroseconds(20); 
  digitalWrite(_clock, HIGH);
  delayMicroseconds(20);   
}

uint8 Wakeup::anti(uint8 dat) {
  uint8 tmp = 0;
 
  if ((dat & 0x80) == 0) {
    tmp |= 0x02; 
  }
 
  if ((dat & 0x40) == 0) {
    tmp |= 0x01; 
  }
 
  return tmp;
}

void Wakeup::sendPacket(uint32 dx) {
  uint8 i;
 
  for (i=0; i<32; i++) {
    if ((dx & 0x80000000) != 0) {
      digitalWrite(_data, HIGH);
    } else {
      digitalWrite(_data, LOW);
    }
 
    dx <<= 1;
    clockProduce();
  }
}

void Wakeup::sendColor(uint8 r, uint8 g, uint8 b) {
    uint32 dx = 0;
 
    dx |= (uint32)0x03 << 30;             // highest two bits 1，flag bits
    dx |= (uint32)anti(b) << 28;
    dx |= (uint32)anti(g) << 26;	
    dx |= (uint32)anti(r) << 24;
 
    dx |= (uint32)b << 16;
    dx |= (uint32)g << 8;
    dx |= r;
 
    sendPacket(dx);
}
 
