# ESP32 DigitalLevel

**ESP32 DigitalLevel** is a compact, precise digital level built using an ESP32-C3 microcontroller and a GY-521 (MPU-6050) accelerometer/gyroscope module. It displays exact angle readings on an OLED screen while using four directional LEDs as a quick visual guide, allowing you to level surfaces without constantly staring at the display.

## Hardware Requirements
* **Microcontroller:** ESP32-C3
* **Sensor:** GY-521 (MPU-6050) accelerometer/gyroscope
* **Display:** SSD1306 OLED Display (I2C)
* **Indicators:** 4x Directional LEDs
* **Buttons:** 2x Push buttons (Zero & Reset)

## Installation

### 1. Hardware Setup
Assemble the components according to the provided schematic diagram in the repository.

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
* **"Reset" Button:** Power cycles the device to turn it On or Off.
