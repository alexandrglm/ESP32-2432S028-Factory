// ###############################################################################
// ESP32-2432s028 TFT_eSPI Screen COnfigs
//
// ILI9341 3,4" "(TFT) + XPT2046 (Resistive Touch)
// ###############################################################################
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

// ###############################################################################
// OpenWeather Configs
// ###############################################################################
// These parameters set the data point count stored in program memory (not the datapoint
// count sent by the server). So they determine the memory used during collection
// of the data points.

#define MAX_HOURS 6     // Maximum "hourly" forecast period, can be up 1 to 48
                        // Hourly forecast not used by TFT_eSPI_OpenWeather example

#define MAX_DAYS 8      // Maximum "daily" forecast periods can be 1 to 8 (Today + 7 days = 8 maximum)
                        // TFT_eSPI_OpenWeather example requires this to be >= 5 (today + 4 forecast days)

#define SHOW_HEADER   // Debug only - for checking response header via serial message
#define SHOW_JSON     // Debug only - simple serial output formatting of whole JSON message
//#define SHOW_CALLBACK // Debug only to show the decode tree
#define OW_STATUS_ON    // Debug only - turn on/off progress and status messages


// ###############################################################################
// DO NOT tinker below, this is configuration checking that helps stop crashes:
// ###############################################################################

#ifdef OW_STATUS_ON
  #define OW_STATUS_PRINTF(C) Serial.print(F(C))
  #define OW_STATUS_PRINT(V) Serial.print(V)
#else
  #define OW_STATUS_PRINTF(C)
  #define OW_STATUS_PRINT(X)
#endif

// Check and correct bad setting
#if (MAX_HOURS > 48) || (MAX_HOURS < 1)
  #undef  MAX_HOURS
  #define MAX_HOURS 48 // Ignore compiler warning!
#endif

// Check and correct bad setting
#if (MAX_DAYS > 8) || (MAX_DAYS < 1)
  #undef  MAX_DAYS
  #define MAX_DAYS 8  // Ignore compiler warning!
#endif

#define MAX_3HRS (MAX_DAYS * 8)
