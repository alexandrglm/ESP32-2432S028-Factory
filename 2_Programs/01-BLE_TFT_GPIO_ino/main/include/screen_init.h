// ###############################################################################
// ESP32-2432s028 SCREEN INITIALIZATION
//
// For ESP-IDF usage, add-components for "TFT_eSPI" and " <XPT2046_Touchscreen.h>"
//
// Main Func for Initialization ---> screenInit();
// ###############################################################################
initT
#ifndef SCREEN_INIT_H
#define SCREEN_INIT_H

// #include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#include "TFT_ILI9341_component.h"
#include "TOUCH_XPT2046_component.h"


// TFT_eSPI + XPT... Initializators
TFT_eSPI tft = TFT_eSPI();

SPIClass hspi(HSPI);
XPT2046_Touchscreen touchSets(XPT2046_CS, XPT2046_IRQ);


// init func made public for usage in main.,c
inline void screenInit() {

    int rotationFixed = 1;
    /* ***********************************************************************
   * 0:  Portrait                  - Flex connector at the bottom
   * 1:  Landscape                 - Flex connector on the left side
   * 2:  Portrait Inverted        - Flex connector at the top
   * 3:  Landscape Inverted (default) - Flex connector on the right side
   *********************************************************************** */

    tft.begin();
    touchSets.begin();

    tft.setRotation(rotationFixed);
    SPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchSets.setRotation(rotationFixed);

    
    tft.invertDisplay(1);
    /* ***********************************************************************
     * >>>> IMPORTANT
     *
     * When Using this in Arduino IDE, .invertDisplay MUST BE ON,
     * otherwise, screen will show inverted colours.
     *
     * In the other hand, when using ESP-IDF, you might choose 
     * what you need under 'idf.py menuconfig' or inside 'sdkconfig' files.
     *  -> Components -> <Your TFT Comp> -> "Swipe Bytes" and/or "Invert Colours" 
     * ***********************************************************************/
  
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
}

#endif
