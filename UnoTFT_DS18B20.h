#include <DallasTemperature.h>   // By Miles Burton - https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <OneWire.h>            // by Jum Studt - https://www.pjrc.com/teensy/td_libs_OneWire.html
#include <Elegoo_GFX.h>         // Core graphics library used by Elegoo_TFTLCD - https://www.elegoo.com/blogs/arduino-projects/elegoo-2-8-inch-touch-screen-for-raspberry-pi-manual
#include <Elegoo_TFTLCD.h>      // Hardware-specific library - https://www.elegoo.com/blogs/arduino-projects/elegoo-2-8-inch-touch-screen-for-raspberry-pi-manual
#include <avr/wdt.h>            // Needed for hardware reset.


// Elegoo TFT screen pins.
#define LCD_CS A3     // Chip Select goes to Analog 3
#define LCD_CD A2     // Command/Data goes to Analog 2
#define LCD_WR A1     // LCD Write goes to Analog 1
#define LCD_RD A0     // LCD Read goes to Analog 0
#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin
// Assign RGB565 color codes to human-readable names. More colors can be found at: https://github.com/newdigate/rgb565_colors
#define	BLACK   0x0000
#define	RED     0xFBE0
#define ORANGE  0xFF90
#define YELLOW  0xFFE0
#define	GREEN   0x07E0
#define	BLUE    0x001F
#define INDIGO  0x4810
#define VIOLET  0x781F
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define WHITE   0xFFFF
#define BROWN   0xCA0A


// The pin that the One-Wire bus is connected to.
const int ONE_WIRE_BUS = 12;
// The interval between timed sensor polls.
const int TIME_INTERVAL = 1500;
// The interval between Serial terminal printouts.
const unsigned long PRINT_LOOP_DELAY = 2000;
const unsigned long POLL_LOOP_DELAY = 700;
// A variable to keep track of the last print time.
unsigned long lastPrintLoop = 0;
unsigned long lastPollLoop = 0;
unsigned int zeroSensors = 0;
unsigned int setupCount = 0;
// The temperature variables.
float tempF1 = 21.12;
float tempF2 = 21.12;
float tempF3 = 21.12;
float tempF4 = 21.12;
float tempF5 = 21.12;
float tempF6 = 21.12;
uint8_t errorBits[8] = {0, 0, 0, 0, 0, 0, 0, 0};


Elegoo_TFTLCD tft( LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET );
OneWire oneWire( ONE_WIRE_BUS );
DallasTemperature dallasTemp( &oneWire );
