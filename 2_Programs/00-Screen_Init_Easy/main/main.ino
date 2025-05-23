
// #######################################################
//        ESP32-2432S028R SCREEN Init made easy
//
// 
//  -> ILI9341 driver:      <TFT_eSPI.h>
//  -> XPT2046 driver:      <XPT2046_Touchscreen.h>
//
//  >>>> IMPORTANT
//
//       Even it's not mandatory to set SPI hosts/ports,
//      you may engage knowledge aboout these, so that...
//
//      TFT uses SPI2 -> HSPI (
//      TOUCH uses SPI2 -> VSPI (BUT, using ESP-IDF, uses SPI3)
//
// #######################################################

#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>


// **************************************
// TFT DEFINITIONS
// **************************************
// TFT DRIVER
#define ILI9341_2_DRIVER  // Also available ILI9341_DRIVER
// TFT RESOLUTION
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
// TFT PINS
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TFT_BL   21
// ENABLE BACKLIGT at HIGH
#define TFT_BACKLIGHT_ON HIGH
// TFT FREQs
#define SPI_FREQUENCY        40000000
#define SPI_READ_FREQUENCY   20000000

// For TFT_eSPI default fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// **************************************
// TOUCH DEFINITIONS
// **************************************
// PINS
#define XPT2046_CS    33
#define XPT2046_IRQ   36
#define XPT2046_MOSI  32
#define XPT2046_MISO  39
#define XPT2046_CLK   25
// TOUCH FREQ
#define SPI_TOUCH_FREQUENCY 2500000


// **************************************
// VARS - INITS for libraries - CLASS
// **************************************
TFT_eSPI tft = TFT_eSPI();

SPIClass hspi(HSPI);
XPT2046_Touchscreen touchSets(XPT2046_CS, XPT2046_IRQ);

int minX = 0,   maxX = 4095;
int minY = 0,   maxY = 4095;

static bool touchWorksDisplayed = false;

// Button MAIN
class Button {
public:

    int x, y, w, h;

    Button(TFT_eSPI &tft) {
        w = 150;
        h = 50;
        x = (tft.width() / 2) - ( w / 2);
        y = (tft.height() / 2) + 20;    // Or ( w / 2)
    }
};



void initTFTandTouch() {

    int rotationFixed = 1;  // This value implies BOTH TFT & TOUCH
  /* ***********************************************************************
   * 0:  Portrait                     - Flex connector at the bottom
   * 1:  Landscape                    - Flex connector on the left side
   * 2:  Portrait Inverted            - Flex connector at the top
   * 3:  Landscape Inverted (default) - Flex connector on the right side
   *********************************************************************** */

    tft.begin();

    SPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchSets.begin();

    tft.setRotation(rotationFixed);
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
    
    
    // Main Black initialization
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
}

void setup() {

    Serial.begin(115200);

    initTFTandTouch();

    // **************************************
    // Main butoncito and text program for test
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_CYAN);
    tft.drawString("TFT and TOUCH initialization test", tft.width() / 2, tft.height() / 2 - 20);

    Button btn(tft);
    // Button init
    tft.fillRoundRect(btn.x, btn.y, btn.w, btn.h, 8, TFT_RED);
    tft.drawRoundRect(btn.x, btn.y, btn.w, btn.h, 8, TFT_WHITE);

    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE);

    tft.drawString("TEST ME!", btn.x + (btn.w / 2), btn.y + (btn.h / 2));

}

void loop() {

    Button btn(tft);

    if (touchSets.touched()) {

        TS_Point p = touchSets.getPoint();

        // Coords for Console
        int screenX = map(p.x, 0, 4095, 0, tft.width());
        int screenY = map(p.y, 0, 4095, 0, tft.height());
        Serial.printf("Raw Coords: X = %d, Y = %d, Mapped Coords: X = %d, Y = %d\n", p.x, p.y, screenX, screenY);

        // Button Touch event definiton
        if (screenX >= btn.x && screenX <= (btn.x + btn.w) &&
            screenY >= btn.y && screenY <= (btn.y + btn.h)) {

            // Button is pressed: Change to GRIS

            tft.fillRoundRect(btn.x, btn.y, btn.w, btn.h, 8, TFT_DARKGREY);
            tft.drawRoundRect(btn.x, btn.y, btn.w, btn.h, 8, TFT_RED);

            tft.setTextDatum(MC_DATUM);
            tft.setTextColor(TFT_WHITE);
            tft.drawString("...", btn.x + (btn.w / 2), btn.y + (btn.h / 2));

            // Wait until the touch is released
            while (touchSets.touched()) {

                delay(10);
            }

            // BUTTON -> Pressed
            Serial.println('[TOUCH] -> Button pressed!');
            
            tft.drawRoundRect(btn.x, btn.y, btn.w, btn.h, 8, TFT_BLUE);
            tft.fillRoundRect(btn.x, btn.y, btn.w, btn.h, 8, TFT_GREEN);

            tft.setTextDatum(MC_DATUM);
            tft.setTextColor(TFT_WHITE);
            tft.drawString("TRY ME AGAIN! :-D", btn.x + (btn.w / 2), btn.y + (btn.h / 2));

            // BUTTON -> !Pressed
            if (!touchWorksDisplayed) {

                tft.setTextDatum(MC_DATUM);
                tft.setTextColor(TFT_GREENYELLOW);

                tft.drawString("Touch works!", tft.width() / 2, btn.y + btn.h + 30);
                
                touchWorksDisplayed = true;
            }
        }
        delay(30);
    }

}
