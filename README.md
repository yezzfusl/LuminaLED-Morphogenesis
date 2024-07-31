# LuminaLED-Morphogenesis
LuminaLED-Morphogenesis is an advanced Arduino project that combines mathematics and artistry. It creates dynamic LED displays using matrix transformations, fractal noise, and fluid motion, producing ever-evolving light patterns. Ideal for those interested in blending technical precision with creative illumination.

## Hardware Setup:
1. Connect 4 LEDs to your Arduino as follows:
   - LED 1: Arduino pin 4 (PD4)
   - LED 2: Arduino pin 5 (PD5)
   - LED 3: Arduino pin 6 (PD6)
   - LED 4: Arduino pin 7 (PD7)

2. For each LED, connect the longer leg (anode) to the Arduino pin through a 220-ohm resistor.
3. Connect the shorter leg (cathode) of each LED to Arduino's GND.

## Building and Uploading the Code:
1. clone the repo: 
    ```git clone https://github.com/yezzfusl/LuminaLED-Morphogenesis.git```

2.Navigate to the project directory:
    `cd LuminaLED-Morphogenesis`

3. Compile the code using avr-gcc:
    `avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o led_pattern.elf led_pattern.c -lm`

4. Create the hex file:
    `avr-objcopy -O ihex -R .eeprom led_pattern.elf led_pattern.hex`

5. Upload to Arduino (assuming it's connected to /dev/ttyUSB0):
    `avrdude -c arduino -p atmega328p -P /dev/ttyUSB0 -U flash:w:led_pattern.hex`
## **Note:** 
  - Ensure you have the AVR toolchain and avrdude installed on your system.
  - After uploading, the LEDs should start displaying the generated pattern.



