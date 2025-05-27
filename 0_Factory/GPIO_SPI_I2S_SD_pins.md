
### ESP32-2432S028R Pins for 


#### TFT (HSPI)
* TFT CS:           **15**
* TFT MISO:         **12**
* TFT MOSI:         **13**
* TFT SCLK:         **14**
* TFT D/C:          **2**
* TFT Backlight:    **21**
  TFT Backlight Init Status:   HIHG
* TFT RESET:        No Pin (-1)
  
* TFT SPI Read freq:   40000000  (40MHz)
* TFT SPI Write freq:  20000000


#### TOUCH
* TOUCH CS:           **33**
* TOUCH IRQ:          **36**
* TOUCH MISO:         **39**
* TOUCH MOSI:         **32**
* TOUCH CLOCK:        **25**
  
* TFT SPI  freq:   2500000  (2.5MHz)


#### SD (VSPI)

* SD CS:             **5**
* SD CLOCK:          **18**
* SD MISO:           **19**
* SD MOSI:           **23**

#### AUDIO OUT - (SDI - I2S)

* AUDIO BCK          **0**  ADC2
* AUDIO WS / LRBCK:  **2**  ADC2
* AUDIO DOUT:        **8**  SDI/SD1


#### RGB LED PINOUT
* BLUE                **16**
* GREEN               **17**
* RED                 **4**


***
CHIP direct pins as diagrams detail:
ESP32-WROOM-32

#### Buzzer - I2S (Not Included in ESP32-2462S028R)
- GPIO should be      **26** for DAC 2-ADC2
                      **34** for DAC 1 - ADC1 or Input

#### FLASH
* SHD/SW2            **9**
* SWP/SD3            **10**
* SCS-CSD / CMD      **11**
* SCK / CLK          **6**
* SDI / SD1          **8**
* SDO / SD0          **7**

#### TTL
* UART0 TX          **1**
* UART0 RX          **3**


#### Others
* SENSOR VP        **36**
* SENSOR VN        **39**

* I2C SDA          **21**
* I2C SDL          **22**


