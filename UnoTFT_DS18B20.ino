/*
 * Elegoo 2.8" TFT touchscreen for the Arduino Uno - https://www.amazon.com/gp/product/B01EUVJYME/
 * Download the zipped manual from https://www.elegoo.com/blogs/arduino-projects/elegoo-2-8-inch-touch-screen-for-raspberry-pi-manual
 * Then install using: Sketch -> Include Library -> Add .ZIP Library
 * 
 * My three devices:
 *   28285410000000BB
 *   2845FF0D000000D4
 *   2855D10B000000DC
 * 
 * Typical run:
 * Found 0x9341 LCD driver
 * Found device with address: 28285410000000BB
 * Found device with address: 2845FF0D000000D4
 * Found device with address: 2855D10B000000DC
 * Setup has finished.
 * Sensor : 0 timer temperature 70.25
 * Sensor : 0 changed temperature 70.25
 * Sensor : 1 timer temperature 69.80
 * Sensor : 1 changed temperature 69.80
 * Sensor : 2 timer temperature 70.02
 * Sensor : 2 changed temperature 70.02
 */
#include <NonBlockingDallas.h>   // by Giovanno Bertazzoni - https://github.com/Gbertaz/NonBlockingDallas
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
// A variable to keep track of the last print time.
unsigned long lastPrintLoop = 0;
// A variable to keep track of sensor resets.
unsigned int resetCount = 0;
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
NonBlockingDallas sensorDs18b20( &dallasTemp );


// Find every device on the bus.
void findDevices() 
{
   byte address[8];

   // Reset the search.
   oneWire.reset_search();

   while( oneWire.search( address ) )
   {
      Serial.print( "Found device with address: " );
      printAddress( address );
      Serial.println();
   }
} // End of findDevices() function.


// Print the address that was found.
void printAddress( byte *address )
{
   for ( int i = 0; i < 8; i++ )
   {
      if( address[i] < 16 )
         Serial.print( "0" );
      Serial.print( address[i], HEX );
   }
} // End of printAddress() function.


// Invoked at every sensor reading interval.
void handleIntervalElapsed( int deviceIndex, long int temperature )
{
   Serial.print( "Sensor : " );
   Serial.print( deviceIndex );
   Serial.print( " timer temperature " );
   Serial.println( sensorDs18b20.rawToFahrenheit( temperature ) );
   if( deviceIndex == 0 )
      tempF1 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 1 )
      tempF2 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 2 )
      tempF3 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 3 )
      tempF4 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 4 )
      tempF5 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 5 )
      tempF6 = sensorDs18b20.rawToFahrenheit( temperature );
   else
   {
      Serial.print( "Unexpected device index: " );
      Serial.println( deviceIndex );
   }
} // End of the handleIntervalElapsed() function.


// Invoked ONLY when the temperature changes.
void handleTemperatureChange( int deviceIndex, long int temperature )
{
   // Set the device error bit to zero.
   errorBits[deviceIndex] = 0;
   Serial.print( "Sensor : " );
   Serial.print( deviceIndex );
   Serial.print( " changed temperature " );
   Serial.println( sensorDs18b20.rawToFahrenheit( temperature ) );
   if( deviceIndex == 0 )
      tempF1 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 1 )
      tempF2 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 2 )
      tempF3 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 3 )
      tempF4 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 4 )
      tempF5 = sensorDs18b20.rawToFahrenheit( temperature );
   else if( deviceIndex == 5 )
      tempF6 = sensorDs18b20.rawToFahrenheit( temperature );
   else
   {
      Serial.print( "Unexpected device index: " );
      Serial.println( deviceIndex );
   }
} // End of the handleTemperatureChange() function.


// Invoked when the sensor reading fails.
void handleDeviceDisconnected( int deviceIndex )
{
   resetCount++;
   // Set the device error bit to 1 to show that this sensor had an error.
   errorBits[deviceIndex] = 1;
   Serial.print( F("\n  Sensor # " ) );
   Serial.print( deviceIndex );
   Serial.println( F( " disconnected!\n" ) );
   // Reset the temperature variable to show there was a disconnect.
   if( deviceIndex == 0 )
      tempF1 = 11.11;
   else if( deviceIndex == 1 )
      tempF2 = 11.11;
   else if( deviceIndex == 2 )
      tempF3 = 11.11;
   else if( deviceIndex == 3 )
      tempF4 = 11.11;
   else if( deviceIndex == 4 )
      tempF5 = 11.11;
   else if( deviceIndex == 5 )
      tempF6 = 11.11;
} // End of handleDeviceDisconnected() function.


void reboot()
{
   Serial.println( "Rebooting the device..." );
   tft.fillScreen( BLACK );
   tft.setTextSize( 4 );
   tft.setCursor( 0, 0 );

   tft.setTextColor( RED );
   tft.print( "\n\nRebooting!" );

   delay( 4000 );

   wdt_disable();
   wdt_enable( WDTO_15MS );
   while( 1 ) {}
} // End of reboot() function.


void setupDallas()
{
   // Initialize the sensor passing the resolution and interval [milliseconds].
   sensorDs18b20.begin( NonBlockingDallas::resolution_12, TIME_INTERVAL );

   // Set up the temperature sensor callbacks.
   sensorDs18b20.onIntervalElapsed( handleIntervalElapsed );
   sensorDs18b20.onTemperatureChange( handleTemperatureChange );
   sensorDs18b20.onDeviceDisconnected( handleDeviceDisconnected );

   // Call the following function to request a new temperature reading without waiting for TIME_INTERVAL to elapse.
   sensorDs18b20.requestTemperature();

   // Increment a counter tracking how often these sensors have been configured.
   setupCount++;

   // Reset the device if this method has been called 5 times.
   if( setupCount > 5 )
      reboot();
   if( setupCount > 1 )
   Serial.println( "Finished setting up one-wire again." );
} // End of setupDallas() function.


void setup( void )
{
  Serial.begin( 115200 );
  Serial.println( F( "Uno TFT DS18B20" ) );

   // Configure the temperature sensors.
   setupDallas();

#ifdef USE_Elegoo_SHIELD_PINOUT
   Serial.println( F( "Using Elegoo 2.4\" TFT Arduino Shield Pinout" ) );
#else
   Serial.println( F( "Using Elegoo 2.4\" TFT Breakout Board Pinout" ) );
#endif

   Serial.print( "TFT size is " );
   Serial.print( tft.width() );
   Serial.print( "x" );
   Serial.println( tft.height() );

   tft.reset();

   uint16_t identifier = tft.readID();
   if( identifier == 0x9325 )
   {
      Serial.println( F( "Found ILI9325 LCD driver" ) );
   }
   else if( identifier == 0x9328 )
   {
      Serial.println( F( "Found ILI9328 LCD driver" ) );
   }
   else if( identifier == 0x4535 )
   {
      Serial.println( F( "Found LGDP4535 LCD driver" ) );
   }
   else if( identifier == 0x7575 )
   {
      Serial.println( F( "Found HX8347G LCD driver" ) );
   }
   else if( identifier == 0x9341 )
   {
      Serial.println( F( "Found ILI9341 LCD driver" ) );
   }
   else if( identifier == 0x8357 )
   {
      Serial.println( F( "Found HX8357D LCD driver" ) );
   }
   else if( identifier == 0x0101 )
   {
      identifier = 0x9341;
      Serial.println( F( "Found 0x9341 LCD driver" ) );
   }
   else if( identifier == 0x1111 )
   {
      identifier = 0x9328;
      Serial.println( F( "Found 0x9328 LCD driver" ) );
   }
   else
   {
      Serial.print( F( "Unknown LCD driver chip: " ) );
      Serial.println( identifier, HEX );
      Serial.println( F( "If using the Elegoo 2.8\" TFT Arduino shield, the line:" ) );
      Serial.println( F( "  #define USE_Elegoo_SHIELD_PINOUT" ) );
      Serial.println( F( "should appear in the library header (Elegoo_TFT.h)." ) );
      Serial.println( F( "If using the breakout board, it should NOT be #defined!" ) );
      Serial.println( F( "Also if using the breakout, double-check that all wiring" ) );
      Serial.println( F( "matches the tutorial." ) );
      identifier = 0x9328;
   }

   tft.begin( identifier );
   tft.fillScreen( BLACK );
   // 0 = oriented with USB port down.
   // 1 = oriented with USB port right.
   // 2 = oriented with USB port up.
   // 3 = oriented with USB port left.
   tft.setRotation( 3 );
   tft.setTextSize( 3 );

   findDevices();

   Serial.println( F( "Setup has finished." ) );
} // End of setup() function.


void loop( void )
{
   sensorDs18b20.update();

   if(( lastPrintLoop == 0 ) || ( millis() - lastPrintLoop ) > PRINT_LOOP_DELAY )
   {
      tft.fillScreen( BLACK );
      tft.setCursor( 0, 0 );
      tft.setTextSize( 3 ),

      tft.setTextColor( RED );
      tft.print( "Vent:     " );
      tft.println( tempF1 );
      tft.setTextColor( ORANGE );
      tft.print( "Ambient:  " );
      tft.println( tempF2 );
      tft.setTextColor( YELLOW );
      tft.print( "Floor:    " );
      tft.println( tempF3 );
      tft.setTextColor( GREEN );
      tft.print( "Roof:     " );
      tft.println( tempF4 );
      tft.setTextColor( BLUE );
      tft.print( "Trunk:    " );
      tft.println( tempF5 );
      tft.setTextColor( WHITE );
      tft.print( "Outside:  " );
      tft.println( tempF6 );
      // tft.setTextColor( MAGENTA );
      // tft.println( 0xDEADBEEF, HEX );
      tft.println( "" );

      tft.setTextSize( 2 ),
      tft.setTextColor( WHITE );
      tft.print( "Resets: " );
      tft.println( resetCount );
      lastPrintLoop = millis();


      int sensorCount = sensorDs18b20.getSensorsCount();
      if( sensorCount == 0 )
         zeroSensors++;
      else
         zeroSensors = 0;
      // Reset the one-wire interface if there has been 20 seconds of no sensors detected.
      if( zeroSensors > 10 )
      {
         zeroSensors = 0;
         setupDallas();
      }
      tft.print( "Sensors: " );
      tft.println( sensorCount );

      tft.print( "Error bits: " );
      static char binaryString[7];
      for (int i = 0; i < 6; i++)
         binaryString[i] = errorBits[i] ? '1' : '0';
      // Add the null terminator.
      binaryString[6] = '\0';
      tft.println( binaryString );

      tft.print( "Setup count: " );
      tft.println( setupCount );

      lastPrintLoop = millis();
   }
} // End of loop() function.
