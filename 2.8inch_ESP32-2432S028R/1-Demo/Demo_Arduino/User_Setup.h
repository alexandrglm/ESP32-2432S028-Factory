// ███████████████████████████████████████████
// ¡DESCOMENTA SOLO UN DRIVER! (Elige el correcto)
#define ILI9341_DRIVER     // Para pantallas ILI9341 estándar
// #define ST7789_DRIVER    // Para pantallas ST7789
// #define ILI9488_DRIVER   // Para pantallas ILI9488
// ███████████████████████████████████████████

// Configuración de pantalla (¡AJUSTA SEGÚN TU MODELO!)
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Configuración de pines SPI (¡AJUSTA A TU PLACA!)
#define TFT_MOSI 23  // Pin DATA
#define TFT_SCLK 18  // Pin CLK
#define TFT_CS   15  // Pin Chip Select
#define TFT_DC    2  // Pin Data/Command
#define TFT_RST  -1  // (-1 si no se usa reset)
#define TFT_BL   21  // Pin retroiluminación

// Configuración de SPI (opcional)
#define SPI_FREQUENCY  40000000  // 40MHz máximo para ILI9341
