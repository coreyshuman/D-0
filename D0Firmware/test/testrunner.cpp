#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../includes/test/catch.hpp"
#include "../includes/Util.h"


#include "../includes/Settings.h"

TEST_CASE( "Test settings", "[settings]" ) {
    Settings settings;
    config_t config = {
        .crc = 0,
        .length = SETTINGS_SIZE
    };
    config.crc = Util::calculateCrc(((uint8_t *)&config)+2, config.length-2);
    REQUIRE( settings.verifyConfig(NULL) == false );
    REQUIRE( config.length == SETTINGS_SIZE);
    REQUIRE( settings.verifyConfig(&config) == true );
}