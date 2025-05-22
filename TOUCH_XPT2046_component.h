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


// <XPT2046_Touchscreen.h> custom class modification to initialize TOUCH
SPIClass touchSPI = SPIClass(VSPI);

XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

void initTouch () {

  touchSPI.begin( XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS );

  ts.begin(touchSPI);


  /* ***********************************************************************
 *  ts.setRotation(std::int)  
 *
 * Possible display orientations according to XPT2046_Touchscreen library:
 *
 *  0:  Portrait                  - Flex connector at the bottom
 *  1:  Landscape                 - Flex connector on the left side
 *  2:  Portrait Inverted        - Flex connector at the top
 *  3:  Landscape Inverted (default) - Flex connector on the right side
 *
 *  These apply when using SPIClass(VSPI)
 *********************************************************************** */
  ts.setRotation(1)
  
}

#endif
