# ESP32 Advice Slip Fetcher
This sketch for the ESP32 fetches a quote from https://adviceslip.com and displays it in scrolling text on a 16x2 LCD display.
It is fully customisable, so if desired you can easily modify it to fetch from a different API or display the output on a differently-sized
LCD.

## Components
- ESP32
- 16x2 backlit LCD display with I2C module
- Momentary push button
- Wires, breadboard, etc.

## Getting started

### Wiring
The button is set up in `INPUT_PULLUP` mode. Therefore it can be wired between ground and GPIO pin 5.

As for the LCD:
| ESP32 pin | I2C module pin |
|-----------|----------------|
|VIN        |VCC             |
|GND        |GND             |
|GPIO 21    |SDA             |
|GPIO 22    |SCL             |

### WiFi Credentials
After downloading, rename the `arduino_secrets_sample.h` file to `arduino_secrets.h`. Then add your WiFi credentials inside this file.

### Use
If everything goes to plan, you should be able to push the button and see a quote scroll across the screen. 
I recommend experimenting with the LCD contrast - with scrolling text, it can make a big difference to readability.
