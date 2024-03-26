#include <DallasTemperature.h>
#include <NonBlockingDallas.h> // https://github.com/Gbertaz/NonBlockingDallas
#include <OneWire.h>
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library


// Elegoo TFT screen pins.
#define LCD_CS A3     // Chip Select goes to Analog 3
#define LCD_CD A2     // Command/Data goes to Analog 2
#define LCD_WR A1     // LCD Write goes to Analog 1
#define LCD_RD A0     // LCD Read goes to Analog 0
#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin
// Assign RGB565 color codes to human-readable names. More colors can be found at: https://github.com/newdigate/rgb565_colors
#define	BLACK   0x0000
#define	RED     0xF800
#define ORANGE  0xFBE0
#define YELLOW  0xFFE0
#define	GREEN   0x07E0
#define	BLUE    0x001F
#define INDIGO  0x4810
#define VIOLET  0x781F
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define WHITE   0xFFFF


const int ONE_WIRE_BUS = 3;
const int TIME_INTERVAL = 1500;
const unsigned long PRINT_LOOP_DELAY = 5000;
unsigned long lastPrintLoop = 0;
float tempF0 = 21.12;
float tempF1 = 21.12;


Elegoo_TFTLCD tft( LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET );
OneWire oneWire( ONE_WIRE_BUS );
DallasTemperature dallasTemp( &oneWire );
NonBlockingDallas sensorDs18b20( &dallasTemp );


// Invoked at every sensor reading interval.
void handleIntervalElapsed( int temperature, long int deviceIndex )
{
	Serial.print( "Sensor : " );
	Serial.print( deviceIndex );
	Serial.print( " timer temperature " );
	Serial.println( temperature );
	if( deviceIndex == 0 )
		tempF0 = temperature;
	else if( deviceIndex == 1 )
		tempF1 = temperature;
	else
  {
		Serial.print( "Unexpected device index: " );
    Serial.println( deviceIndex );
  }
} // End of the handleIntervalElapsed() function.


// Invoked ONLY when the temperature changes.
void handleTemperatureChange( int temperature, long int deviceIndex )
{
	Serial.print( "Sensor : " );
	Serial.print( deviceIndex );
	Serial.print( " changed temperature " );
	Serial.println( temperature );
	if( deviceIndex == 0 )
		tempF0 = temperature;
	else if( deviceIndex == 1 )
		tempF1 = temperature;
  else
  {
		Serial.print( "Unexpected device index: " );
    Serial.println( deviceIndex );
  }
} // End of the handleTemperatureChange() function.


void setup( void )
{
  Serial.begin( 115200 );
  Serial.println( F( "Uno TFT DS18B20" ) );

	// Initialize the sensor passing the resolution and interval [milliseconds].
	sensorDs18b20.begin( NonBlockingDallas::resolution_12, TIME_INTERVAL );

	// Set up the temperature sensor callbacks.
	sensorDs18b20.onIntervalElapsed( handleIntervalElapsed );
	sensorDs18b20.onTemperatureChange( handleTemperatureChange );

	// Call the following function to request a new temperature reading without waiting for TIME_INTERVAL to elapse.
	sensorDs18b20.requestTemperature();

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
	Serial.println( F("Setup has finished.") );
}


void loop( void )
{
	sensorDs18b20.update();
  if(( lastPrintLoop == 0 ) || ( millis() - lastPrintLoop ) > PRINT_LOOP_DELAY )
  {
    tft.fillScreen( BLACK );
    // 0 = oriented with USB port down.
    // 1 = oriented with USB port right.
    // 2 = oriented with USB port up.
    // 3 = oriented with USB port left.
    tft.setRotation( 3 );
    tft.setCursor( 0, 0 );
    tft.setTextSize( 3 );

    tft.setTextColor( RED );
    tft.println( "Temp 1: 73.3" );
    tft.setTextColor( ORANGE );
    tft.println( "Temp 2: 123.4" );
    tft.setTextColor( YELLOW );
    tft.println( "Temp 3: 21.12" );
    tft.setTextColor( GREEN );
    tft.println( "Temp 4: 88.8" );
    tft.setTextColor( BLUE );
    tft.println( "Temp 5: 0.12" );
    tft.setTextColor( INDIGO );
    tft.println( "Temp 6: 12.3" );
    tft.setTextColor( VIOLET );
    tft.println( "Temp 7: 55" );
    tft.setTextColor( CYAN );
    tft.println( 01234.56789 );
    tft.setTextColor( MAGENTA );
    tft.println( 0xDEADBEEF, HEX );
    lastPrintLoop = millis();
  }
}
