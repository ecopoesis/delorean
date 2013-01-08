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
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"   
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif

#include "Wakeup.h"

Wakeup wakeup(12, 13);

void setup() {
}

///
/// @brief	Loop
/// @details	Call blink
///
// Add loop code 
void loop() {
    for (int i = 0; i < 255; i++) {
        wakeup.set(i, 0, 255 - i);
        delay(50);
    }
    for (int i = 0; i < 255; i++) {
        wakeup.set(255 - i, 0, i);
        delay(50);
    }
}