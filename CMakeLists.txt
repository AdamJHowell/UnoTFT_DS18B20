CMAKE_MINIMUM_REQUIRED( VERSION 3.23 )
PROJECT( UnoTFT )

SET( CMAKE_CXX_STANDARD 14 )

ADD_EXECUTABLE( UnoTFT
      UnoTFT_DS18B20.h
      UnoTFT_DS18B20.ino
)

INCLUDE_DIRECTORIES( .
      C:/Users/adam.howell/AppData/Local/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/include/avr/wdt.h
      C:/Users/adam.howell/Documents/Arduino/libraries/DallasTemperature/
      C:/Users/adam.howell/Documents/Arduino/libraries/OneWire/
)

SET_TARGET_PROPERTIES( UnoTFT PROPERTIES LINKER_LANGUAGE CXX )
