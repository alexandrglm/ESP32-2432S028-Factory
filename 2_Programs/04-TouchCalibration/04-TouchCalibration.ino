// #######################################################
//        ESP32-2432S028R Touch Calibration
//
//  -> XPT2046 driver:      <XPT2046_Touchscreen.h>
//  -> ILI9341 driver:      <TFT_eSPI.h>
//
// #######################################################

#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>

#ifndef TFT_WIDTH
  #include "User_Setup.h"
#endif


// XPT2046 GPIO
#define XPT2046_MISO 39
#define XPT2046_MOSI 32
#define XPT2046_CS   33
#define XPT2046_IRQ  36
#define XPT2046_SCLK  25


// Variables
int rotationFIxed = 0;
  /* ***********************************************************************
   * 0:  Portrait                  - Flex connector at the bottom
   * 1:  Landscape                 - Flex connector on the left side
   * 2:  Portrait Inverted        - Flex connector at the top
   * 3:  Landscape Inverted (default) - Flex connector on the right side
   *********************************************************************** */


// <XPT2046_Touchscreen.h> Class adapted to this GPIOs board
SPIClass hspi(HSPI);

XPT2046_Touchscreen touchSets(XPT2046_CS, XPT2046_IRQ);

// TFT_eSPI object
TFT_eSPI tft = TFT_eSPI();

// Calibration point coordinates
const int NUM_CAL_POINTS = 4;

int calibrationX[NUM_CAL_POINTS];
int calibrationY[NUM_CAL_POINTS];

String calibrationText[NUM_CAL_POINTS] = {
  
  "Top-Left", "Top-Right", "Bottom-Left", "Bottom-Right"
};

void calibrationXPT2046();

void setup() {

  Serial.begin(115200);

  hspi.begin(XPT2046_SCLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchSets.begin(hspi);

  // Initialize TFT
  tft.init();
  tft.invertDisplay(1);


  // ! >>>> IMPORTANT
  //        Set BOTH TFT and TOUCH rotations with the same parameters
  
  touchSets.setRotation(rotationFIxed);
  tft.setRotation(rotationFIxed);

  
  tft.fillScreen(TFT_BLACK); // Start with a black screen
  
  Serial.println("[TOUCH] -> Initialized!");
  Serial.println("[TFT] -> Initialized!");

  calibrationXPT2046();
}


void loop() {

  if (touchSets.touched()) {

    TS_Point p = touchSets.getPoint();
    Serial.printf("[TOUCH/Coords.] --> X = %d, Y = %d, Pressure = %d\n", p.x, p.y, p.z);
    
    
    // ! >>>> IMPORTANT
    //        Raw coordinates means entire coords, from 0 to 4095
    //        not the real-usable coords on screen.
    int screenX = map(p.x, 0, 4095, 0, tft.width());
    int screenY = map(p.y, 0, 4095, 0, tft.height());
    
    tft.fillCircle(screenX, screenY, 5, TFT_RED);
    
    delay(30);

  } else {

    // tft.fillScreen(TFT_BLACK);
  }
}

// ##################################
// Calibration
// #################################

void calibrationXPT2046() {
  
  Serial.println("\n=== Touchscreen Calibration Test ===");
  
  tft.fillScreen(TFT_YELLOW);
  tft.setTextColor(TFT_PURPLE);
  tft.setTextSize(2);
  tft.setCursor(50, 150);
  tft.print("Touch Calibration");


  // CAL POINTS -> Uses the min max width/height configured in TFT_eSPI User_Setups
  int points[NUM_CAL_POINTS][2] = {

    {0, 0},                      // Top-Left (X=0, Y=0)
    {tft.width() - 1, 0},        // Top-Right (X=max, Y=0)
    {0, tft.height() - 1},       // Bottom-Left (X=0, Y=max)
    {tft.width() - 1, tft.height() - 1} // Bottom-Right (X=max, Y=max)
  };

  for (int i = 0; i < NUM_CAL_POINTS; i++) {
    
    tft.fillScreen(TFT_YELLOW);
    tft.setCursor(50, 20);
    tft.printf("Touch %s corner", calibrationText[i].c_str());

    // CALPOINT MARK
    tft.drawLine(points[i][0] - 10, points[i][1], points[i][0] + 10, points[i][1], TFT_RED);
    tft.drawLine(points[i][0], points[i][1] - 10, points[i][0], points[i][1] + 10, TFT_RED);
    tft.fillCircle(points[i][0], points[i][1], 3, TFT_RED);

    bool touched = false;
    while (!touched) {
      
      if ( touchSets.touched() ) {
        
        TS_Point p = touchSets.getPoint();
        
        Serial.printf("Raw Coords %s: X = %d, Y = %d, Pressure(Z, Fac) = %d\n", calibrationText[i].c_str(), p.x, p.y, p.z);
        
        calibrationX[i] = p.x;
        calibrationY[i] = p.y;
        
        touched = true;
        
        delay(500);
      }

      delay(10);
    }
  }

  Serial.println("\n=== Calibration Complete! ---");
  Serial.println("Calibration Raw Points:");
  
  for (int i = 0; i < NUM_CAL_POINTS; i++) {
    
    Serial.printf("%s: Raw X = %d, Raw Y = %d\n", calibrationText[i].c_str(), calibrationX[i], calibrationY[i]);
  
  }

  // Exit 
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(10, 10);
  tft.print("\nCalibration D0ne!\n");
  delay(10000);
  
  tft.fillScreen(TFT_BLACK);
}
