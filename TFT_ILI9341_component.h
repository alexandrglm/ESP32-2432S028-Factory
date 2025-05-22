// TFT pins component, for esp-idf define "TFT_eSPI" dependency

// DRIVER
// Also available ILI9341_DRIVER
#define ILI9341_2_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// PINS
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TFT_BL   21

// BL HIGH/LOW
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
