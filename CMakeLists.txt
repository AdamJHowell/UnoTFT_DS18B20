cmake_minimum_required( VERSION 3.25 )
project( UnoTFT_DS18B20 )

set( CMAKE_CXX_STANDARD 14 )

include_directories( . )

add_executable( UnoTFT_DS18B20
		UnoTFT_DS18B20.ino
)

set_target_properties( UnoTFT_DS18B20 PROPERTIES LINKER_LANGUAGE CXX )
