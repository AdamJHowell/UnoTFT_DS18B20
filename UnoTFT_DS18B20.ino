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
 * 
 */
#include "UnoTFT_DS18B20.h"


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
   dallasTemp.begin();

   // Request a new temperature reading.
   dallasTemp.requestTemperatures();

   // Increment a counter tracking how often these sensors have been configured.
   setupCount++;
   findDevices();

   // Get the number of devices on the bus
   int numberOfDevices = dallasTemp.getDeviceCount();
   Serial.print( "Found " );
   Serial.print( numberOfDevices );
   Serial.println( " devices." );

   // Loop through each device, get its address, and print it
   findDevices();

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

   Serial.println( F( "Setup has finished." ) );
} // End of setup() function.


void pollTelemetry()
{
   // Request a new temperature reading.
   dallasTemp.requestTemperatures();
   for( int i = 0; i < 6; i++ )
      temperatures[i] = dallasTemp.getTempFByIndex( i );
//   tempF1 = dallasTemp.getTempFByIndex( 0 );
//   tempF2 = dallasTemp.getTempFByIndex( 1 );
//   tempF3 = dallasTemp.getTempFByIndex( 2 );
//   tempF4 = dallasTemp.getTempFByIndex( 3 );
//   tempF5 = dallasTemp.getTempFByIndex( 4 );
//   tempF6 = dallasTemp.getTempFByIndex( 5 );
} // End of pollTelemetry() function.


void loop( void )
{
   if(( lastPollLoop == 0 ) || ( millis() - lastPollLoop ) > POLL_LOOP_DELAY )
   {
      pollTelemetry();
      lastPollLoop = millis();
   }

   if(( lastPrintLoop == 0 ) || ( millis() - lastPrintLoop ) > PRINT_LOOP_DELAY )
   {
      unsigned int sensorCount = dallasTemp.getDS18Count();
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

      tft.fillScreen( BLACK );
      tft.setCursor( 0, 0 );
      tft.setTextSize( 3 );

      for( int i = 0; i < 6; i++ )
      {
         tft.setTextColor( colors[i] );
         tft.print( labels[i] );
         tft.println( temperatures[0] );
      }
//      tft.setTextColor( RED );
//      tft.print( "Vent:    " );
////      tft.println( tempF1 );
//      tft.println( temperatures[0] );
//      tft.setTextColor( ORANGE );
//      tft.print( "Ambient: " );
////      tft.println( tempF2 );
//      tft.println( temperatures[1] );
//      tft.setTextColor( YELLOW );
//      tft.print( "Floor:   " );
////      tft.println( tempF3 );
//      tft.println( temperatures[2] );
//      tft.setTextColor( GREEN );
//      tft.print( "Roof:    " );
////      tft.println( tempF4 );
//      tft.println( temperatures[3] );
//      tft.setTextColor( BLUE );
//      tft.print( "Trunk:   " );
////      tft.println( tempF5 );
//      tft.println( temperatures[4] );
//      tft.setTextColor( WHITE );
//      tft.print( "Outside: " );
////      tft.println( tempF6 );
//      tft.println( temperatures[5] );

      tft.setTextSize( 2 ),
      tft.setTextColor( WHITE );
      tft.println( "" );

      tft.print( "Sensors: " );
      tft.println( sensorCount );

      tft.print( "Setup count: " );
      tft.println( setupCount );

      lastPrintLoop = millis();
   }
} // End of loop() function.
