# ESP32 DigitalLevel

**ESP32 DigitalLevel** is a compact, precise digital level built using an ESP32-C3 microcontroller and a GY-521 (MPU-6050) accelerometer/gyroscope module. It displays exact angle readings on an OLED screen while using four directional LEDs as a quick visual guide, allowing you to level surfaces without constantly staring at the display.

## Hardware Requirements
* **Microcontroller:** ESP32-C3
* **Sensor:** GY-521 (MPU-6050) accelerometer/gyroscope
* **Display:** SSD1306 OLED Display (I2C)
* **Indicators:** 4x Directional LEDs
* **Buttons:** 2x Push buttons (Zero & Reset)
* **Charging Module** TP4056
* **Boost Converter** MT3608
* **LiPo Battery** 3.7V 1000mAh 10*20*50mm
* **Perfboard** Platform to connect components

## Installation

### 1. Hardware Setup
Solder the components on the perfboard according to the provided schematic diagram in the repository.

**Critical:** for the device to work properly, you must use a flat edge and a multimeter to adjust the silver
knob on the side of the MT3608 booster converter to 5 VOLTS


### 2. Software Setup
1. Open the project file in the **Arduino IDE**.
2. Install the following required libraries via the Arduino Library Manager (`Sketch` > `Include Library` > `Manage Libraries...`):
   * `Adafruit BusIO` by Adafruit
   * `Adafruit GFX Library` by Adafruit
   * `Adafruit SSD1306` by Adafruit
   * `GY521` by Rob Tillaart
3. Select your ESP32-C3 board, choose the correct COM port, and flash the code.

## Usage

* **"Zero" Button:** Sets the current angle as the relative baseline (0°). Once set, the 4 directional LEDs will dynamically light up to guide you back to this exact angle.
* **"On/Off" Button:** Power cycles the device to turn it On or Off.
* **Charging** Connect a power source via USB-C to charge the LiPo battery
