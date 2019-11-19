#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../includes/test/catch.hpp"
#include "../includes/Util.h"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

#include "../includes/Settings.h"

TEST_CASE( "Test settings", "[settings]" ) {
    Settings settings;
    settings_t config = {
        .crc = 0,
        .length = SETTINGS_SIZE
    };
    config.crc = Util::CalculateCrc(((uint8_t *)&config)+2, config.length-2);
    REQUIRE( settings.VerifyConfig(NULL) == false );
    REQUIRE( config.length == SETTINGS_SIZE);
    REQUIRE( settings.VerifyConfig(&config) == true );
}