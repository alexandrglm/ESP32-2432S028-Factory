// #######################################################
//        ESP32-2432S028R Touch Test via serial
//
//  -> XPT2046 driver used: <XPT2046_Touchscreen.h>
//
// #######################################################

#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// XPT2046 GPIO
#define XPT2046_MISO 39
#define XPT2046_MOSI 32
#define XPT2046_CS   33
#define XPT2046_IRQ  36
#define XPT2046_SCLK  25

// <XPT2046_Touchscreen.h> Class adapted to this GPIOs board
SPIClass hspi(HSPI);

XPT2046_Touchscreen touchSets(XPT2046_CS, XPT2046_IRQ);


void setup() {

  Serial.begin(115200);
  
  hspi.begin(XPT2046_SCLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchSets.begin(hspi);

  touchSets.setRotation(1);
  /* #####################################################################
 *  (0) :  Portrait                     - Flex connector at the bottom
 *  (1) :  Landscape                    - Flex connector on the left side
 *  (2) :  Portrait Inverted            - Flex connector at the top
 *  (3) :  Landscape Inverted (default) - Flex connector on the right side
 ** ##################################################################### */
  
  Serial.println("[TOUCH] -> Initialized!");

}

void loop() {

  if (touchSets.touched()) {
    
    // From <XPT2046_Touchscreen.h>, Class plus own method for serializing outputs
    TS_Point p = touchSets.getPoint();
    
    Serial.printf("X = %d, Y = %d, Pressure = %d\n", p.x, p.y, p.z);
    
    delay(30);  // The loop delay, here, represents the touch delay refresh
    /* #####################################################################
    * About Refresh Rate using Arduino IDE / ESP-IDF IDE
    *
    *     This delay affects/represents the touch refresh rate in this case.
    * While it's not adjustable in the Arduino IDE, you can configure it
    * when using this script with ESP-IDF with some libraries as LVGL.
    * (idf.py menuconfig -> Components -> LVGL -> Touch -> Refresh Rates).
    *
    * These values should match if you're using ESP-IDF.
    ** #####################################################################*/
  }
}
