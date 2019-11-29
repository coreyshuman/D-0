#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../includes/test/catch.hpp"
#include "../includes/Util.h"
#include "../includes/Settings.h"

Serial_ Serial;
EEPROM_ EEPROM;

TEST_CASE( "Test settings", "[settings]" ) {
    Settings settings;
    config_t config = {
        .crc = 0,
        .length = SETTINGS_SIZE
    };
    config.crc = Util::calculateCrc(((uint8_t *)&config)+2, config.length-2);
    REQUIRE( settings.verifyConfig((config_t *) 0) == false );
    REQUIRE( config.length == SETTINGS_SIZE);
    REQUIRE( settings.verifyConfig(&config) == true );
}

TEST_CASE( "Test string int parsing", "[Util]" ) {
    REQUIRE( Util::atoi((const char*)"1") == 1 );
    REQUIRE( Util::atoi((const char*)"-1") == -1 );
    REQUIRE( Util::atoi((const char*)"0") == 0 );
    REQUIRE( Util::atoi((const char*)"123") == 123 );
    REQUIRE( Util::atoi((const char*)"-123") == -123 );
    REQUIRE( Util::atoi((const char*)"-32768") == -32768 );
    REQUIRE( Util::atoi((const char*)"32767") == 32767 );
    REQUIRE( Util::atoi((const char*)"-1a1") == 0 );
    REQUIRE( Util::atoi((const char*)"1a1") == 0 );
}

TEST_CASE( "Test string float parsing", "[Util]" ) {
    REQUIRE( Util::atof((const char*)"1") == 1.0f );
    REQUIRE( Util::atof((const char*)"-1") == -1.0f );
    REQUIRE( Util::atof((const char*)"0") == 0.0f );
    float negativeZero = Util::atof((const char*)"-0");
    REQUIRE( (negativeZero == -0.0f && std::signbit(negativeZero) == true) );
    REQUIRE( Util::atof((const char*)"1.1") == 1.1f );
    REQUIRE( Util::atof((const char*)"-1.1") == -1.1f );
    REQUIRE( Util::atof((const char*)"123.456") == 123.456f );
    REQUIRE( Util::atof((const char*)"-123.456") == -123.456f );
    REQUIRE( Util::atof((const char*)"65500.0") == 65500.0f );
    REQUIRE( Util::atof((const char*)".0000610") == .0000610f );
    REQUIRE( Util::atof((const char*)"340282350000000000000000000000000000000.0") == 3.4028235e+38f );
    REQUIRE( Util::atof((const char*)"-340282350000000000000000000000000000000.0") == -3.4028235e+38f );
    REQUIRE( Util::atof((const char*)"0.000000000000000000000001") == 1.0e-24f );
    REQUIRE( Util::atof((const char*)"-0.000000000000000000000001") == -1.0e-24f );
    // won't work due to floating point error
    // REQUIRE( Util::atof((const char*)"0.0000000000000000000000000000000000000000000014") == 1.4e-45f );
    // REQUIRE( Util::atof((const char*)"-0.0000000000000000000000000000000000000000000014") == -1.4e-45f );
    REQUIRE( Util::atof((const char*)"-1.a1") == 0.0f );
    REQUIRE( Util::atof((const char*)"1.a1") == 0.0f );
}