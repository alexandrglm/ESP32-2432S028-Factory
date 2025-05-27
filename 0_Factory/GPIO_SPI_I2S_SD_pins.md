### ESP32-2432S028R Pinout

#### TFT (HSPI)

| Function          | Pin (GPIO) | Description                                         |
| :---------------- | :--------- | :-------------------------------------------------- |
| **TFT CS** | `15`       | Chip Select for the TFT display.                    |
| **TFT MISO** | `12`       | Master In, Slave Out (Data from TFT to ESP32).      |
| **TFT MOSI** | `13`       | Master Out, Slave In (Data from ESP32 to TFT).      |
| **TFT SCLK** | `14`       | Serial Clock for the TFT display.                   |
| **TFT D/C** | `2`        | Data/Command line for the TFT controller.           |
| **TFT Backlight** | `21`       | Controls the display's backlight (High for ON).     |
| **TFT RESET** | `-1`       | No dedicated reset pin (software reset or not used). |
| **SPI Read Freq** | `40000000` Hz    | SPI frequency for reading data from TFT.            |
| **SPI Write Freq**| `20000000` Hz    | SPI frequency for writing data to TFT.              |

---

#### TOUCH

| Function          | Pin (GPIO) | Description                                       |
| :---------------- | :--------- | :------------------------------------------------ |
| **TOUCH CS** | `33`       | Chip Select for the resistive touch panel.        |
| **TOUCH IRQ** | `36`       | Interrupt Request from the touch controller. (When using LovyanGFX it might be disabled as -1)   |
| **TOUCH MISO** | `39`       | Master In, Slave Out (Data from Touch to ESP32).  |
| **TOUCH MOSI** | `32`       | Master Out, Slave In (Data from ESP32 to Touch).  |
| **TOUCH CLOCK** | `25`       | Serial Clock for the touch panel.                 |
| **SPI Freq** | `2500000` Hz   | SPI frequency for touch communication.            |


---

#### SD Card (VSPI)

| Function          | Pin (GPIO) | Description                                       |
| :---------------- | :--------- | :------------------------------------------------ |
| **SD CS** | `5`        | Chip Select for the SD card.                      |
| **SD CLOCK** | `18`       | Serial Clock for the SD card.                     |
| **SD MISO** | `19`       | Master In, Slave Out (Data from SD to ESP32).     |
| **SD MOSI** | `23`       | Master Out, Slave In (Data from ESP32 to SD).     |

---

#### Audio Out (I2S)

| Function            | Pin (GPIO) | Description                                                        |
| :------------------ | :--------- | :----------------------------------------------------------------- |
| **AUDIO BCK** | `0`        | Bit Clock for I2S audio (Note: GPIO 0 is also a boot pin, can be problematic). |
| **AUDIO WS / LRBCK**| `2`        | Word Select / Left/Right Bit Clock for I2S audio.                  |
| **AUDIO DOUT** | `8`        | Data Out for I2S audio (Note: GPIO 8 is also used for flash/SDIO). |


---

#### RGB LED Pinout

| Function          | Pin (GPIO) | Description                                   |
| :---------------- | :--------- | :-------------------------------------------- |
| **BLUE** | `16`       | Controls the Blue component of the RGB LED.   |
| **GREEN** | `17`       | Controls the Green component of the RGB LED.  |
| **RED** | `4`        | Controls the Red component of the RGB LED.    |

---

#### LIGHT SENSOR
| Function          | Pin (GPIO) | Description                                   |
| :---------------- | :--------- | :-------------------------------------------- |
| CDS INPUT          | `34`      |  Photoocnductive Input values pin.        |

### ESP32-WROOM-32 Chip Direct Pins

#### Buzzer - I2S (Not Included in ESP32-2462S028R)

| Function          | Pin (GPIO) | Description                                       |
| :---------------- | :--------- | :------------------------------------------------ |
| **DAC 2 / ADC2** | `26`       | Digital to Analog Converter output / Analog to Digital Converter input. |
| **DAC 1 / ADC1 / Input**| `34`       | Digital to Analog Converter output / Analog to Digital Converter input / General purpose input. |

---

#### FLASH

| Function          | Pin (GPIO) | Description                                     |
| :---------------- | :--------- | :---------------------------------------------- |
| **SHD / SW2** | `9`        | Flash control pin (SHD - shutdown / SW2).       |
| **SWP / SD3** | `10`       | Flash control pin (SWP - write protect / SD3).  |
| **SCS-CSD / CMD** | `11`       | Flash control pin (CSD - Chip Select / CMD).    |
| **SCK / CLK** | `6`        | Flash control pin (SCK - Serial Clock / CLK).   |
| **SDI / SD1** | `8`        | Flash control pin (SDI - Serial Data In / SD1). |
| **SDO / SD0** | `7`        | Flash control pin (SDO - Serial Data Out / SD0).|

---

#### TTL (Serial Communication)

| Function          | Pin (GPIO) | Description                                      |
| :---------------- | :--------- | :----------------------------------------------- |
| **UART0 TX** | `1`        | UART0 Transmit pin (for debugging/communication).|
| **UART0 RX** | `3`        | UART0 Receive pin (for debugging/communication). |

---

#### EXPANSION PORTS / OTHERS

#### EXPANDED IO (Labeled as P3)

| *PIN 1*  | *PIN 2* | *PIN 3* | *PIN 4* |
| :------- | :------ | :------ | :------ |
| **GND**  | **35**  | **22**  | **21** | 


#### EXPANDED IO (Labeled as CN1)

| *PIN 1*  | *PIN 2*           | *PIN 3* | *PIN 4* |
| :------- | :---------------- | :------ | :------ |
| **GND**  | **NOT CONNECTED**  | **27**  | **Vcc 3,3V** | 

#### EXT POWER CONNECTOR - SERIAL TTL EXT CONNECTOR (Labeles as P5)

| *PIN 1*       | *PIN 2* | *PIN 3* | *PIN 4* |
| :------------ | :------ | :------ | :------ |
| **Vcc (IN)**  | **TX**  | **RX**  | **GND** | 

#### EXT AUDIO OUT (LABELED AS P4)

| *PIN 1*       | *PIN 2* |
| :------------ | :------ |
| **VO1**        | **VO2**  |




| Function          | Pin (GPIO) | Description                                   |
| :---------------- | :--------- | :-------------------------------------------- |
| **SENSOR VP** | `36`       | Analog input for sensor (Voltage Positive).   |
| **SENSOR VN** | `39`       | Analog input for sensor (Voltage Negative).   |
| **I2C SDA** | `21`       | I2C Serial Data line.                         |
| **I2C SCL** | `22`       | I2C Serial Clock line.                        |


