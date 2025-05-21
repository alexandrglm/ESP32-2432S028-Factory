// ###############################################################################
// ESP32-2432s028 TFT_eSPI Screen COnfigs
//
// ILI9341 3,4" "(TFT) + XPT2046 (Resistive Touch)
// ###############################################################################
// File must be invoked in main.c's, BYPASSING the library-included User_Setup.h
//
// If colour issues are found (inverted colours), try with:
//
//    - TFT.invertDisplay(boolean);
//    - tft.setSwapBytes(boolean);

#define ILI9341_2_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_MISO 12
#define TFT_MOSI 13  // Data
#define TFT_SCLK 14  // Clock
#define TFT_CS   15  // Chip Selection
#define TFT_DC    2  // Data / Commands
#define TFT_RST  -1  // Reset (No Reset Pin)
#define TFT_BL   21  // Backlight
#define TOUCH_CS 33  // Touch

#define TFT_BL   21            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF

#define SMOOTH_FONT

// SPI specs for ILI9341
#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
