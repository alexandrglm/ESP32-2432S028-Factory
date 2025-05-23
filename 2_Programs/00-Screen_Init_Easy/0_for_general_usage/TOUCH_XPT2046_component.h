/*
 * ESP32-2432S028R Touch Component:  XPT2046
 *
 * Easy implementation using <XPT2046_Touchscreen.h> from PaulStoffregen 
 * (https://github.com/PaulStoffregen/XPT2046_Touchscreen
*/ 

#ifndef TOUCH_XPT2046_COMPONENT_H
#define TOUCH_XPT2046_COMPONENT_H

// AS LONG AS YOU NEED TO USE THIS LIBRARIY, THEN YOU NEED TO INITIALIZE PROPERLY WITH A CUSTOM CLASS
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// PINS
#define XPT2046_CS    33
#define XPT2046_IRQ   36
#define XPT2046_MOSI  32
#define XPT2046_MISO  39
#define XPT2046_CLK   25

// TOUCH FREQ
#define SPI_TOUCH_FREQUENCY 2500000



#endif
