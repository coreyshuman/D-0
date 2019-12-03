#ifndef _TEST_RUNNER
    #include <Arduino.h>
#endif
#include <stdint.h>
#include "../includes/Util.h"



/**
 * Static table used for the table_driven implementation.
 *****************************************************************************/
static const uint16_t crc_table[16] = 
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

/********************************************************************
* Function: 	calculateCrc()
*
* Precondition: 
*
* Input: 		Data pointer and data length
*
* Output:		CRC.
*
* Side Effects:	None.
*
* Overview:     Calculates CRC for the given data and len
*
*			
* Note:		 	None.
********************************************************************/	
uint16_t Util::calculateCrc(uint8_t *data, uint32_t len)
{
    uint32_t i;
    uint16_t crc = 0;
    
    while(len--)
    {
        i = (crc >> 12) ^ (*data >> 4);
	    crc = crc_table[i & 0x0F] ^ (crc << 4);
	    i = (crc >> 12) ^ (*data >> 0);
	    crc = crc_table[i & 0x0F] ^ (crc << 4);
	    data++;
	} 

    return (crc & 0xFFFF);
}

void Util::printHex(uint8_t val) {
    if(val < 10) {
        Serial.print("0");
    }
    Serial.print(val, HEX);
}

/********************************************************************
* Function: 	itoh()
*
* Precondition: 
*
* Input: 		Unsigned 16 bit integer, char* with at least 4 byte of memory
*
* Output:		4-byte hexidecimal string
*
* Side Effects:	None.
*
* Overview:     Calculates the hexidecimal string representation of an
*               unsigned integer and saves to memory location pointed to
*               by returnStringDestination
*
*			
* Note:		 	Return string is most-significant digit first.
********************************************************************/	
void Util::itoh(uint16_t val, char* returnStringDestination)
{
    int i;
    for(i = 0; i < 4; i++) {
        uint8_t tempNibble = (val >> (12 - 4*i)) & 0x0F;
        if(tempNibble < 10) {
            returnStringDestination[i] = (char)('0' + tempNibble);
        } else {
            returnStringDestination[i] = (char)('A' + tempNibble - 10);
        }
    }
}

/********************************************************************
* Function: 	htoi()
*
* Precondition: 
*
* Input: 		String containing hexidecimal characters. 
*
* Output:		16-bit unsigned integer.
*
* Side Effects:	None.
*
* Overview:     Calculates the numeric value of a multicharacter hexidecimal
*               string representation. String can be up to 4 characters long.
*
*			
* Note:		 	Hexidecimal string is most-significant digit first.
********************************************************************/	
uint16_t Util::htoi(const char *str)
{
    uint16_t val = 0;
    int idx = 0;

    while(str[idx] != 0 && idx < 4) {
        char c = str[idx];
        val <<= idx > 0 ? 4 : 0;
        if(c <= '9' && c >= '0') {
            val += ((c - '0') & 0x0F);
        } else if(c <= 'f' && c >= 'a') {
            val += 10 + ((c - 'a') & 0x0F);
        } else if(c <= 'F' && c >= 'A') {
            val += 10 + ((c - 'A') & 0x0F);
        } else {
            // invalid character
            return 0;
        }
        idx++;
    }

    return val;
}

/********************************************************************
* Function: 	atoi()
*
* Precondition: 
*
* Input: 		String containing numeric characters. Can start with hyphen
*               to indicate negative number.
*
* Output:		16-bit signed integer.
*
* Side Effects:	None.
*
* Overview:     Calculates the numeric value of a multicharacter numeric
*               string representation.
*
*			
* Note:		 	None.
********************************************************************/	
int16_t Util::atoi(const char *str)
{
    int16_t val = 0;
    bool isNegative = false;
    bool isOnesPlace = true;
    int idx = 0;

    if(str[0] == '-') {
        isNegative = true;
        idx++;
    }

    while(str[idx] != 0) {
        char c = str[idx];
        if(c <= '9' && c >= '0') {
            if(isOnesPlace) {
                isOnesPlace = false;
            } else {
                val *= 10;
            }
            val += (c - '0');
        } else {
            // invalid character
            return 0;
        }
        idx++;
    }

    return isNegative ? -val : val;
}

/********************************************************************
* Function: 	atof()
*
* Precondition: 
*
* Input: 		String containing floating point numeric characters. 
*               Can start with hyphen to indicate negative number.
*
* Output:		floating point number.
*
* Side Effects:	None.
*
* Overview:     Calculates the numeric value of a multicharacter numeric
*               string representation.
*
*			
* Note:		 	None.
********************************************************************/	
float Util::atof(const char *str)
{
    char c;
    float val = 0;
    bool isNegative = false;
    bool isOnesPlace = true;
    bool isFraction = false;
    int idx = 0;
    float divisor = 1;

    if(str[0] == '-') {
        isNegative = true;
        idx++;
    }

    while(str[idx] != 0) {
        c = str[idx];
        if(c == '.') {
            if(isFraction) { // duplicate decimal point
                return 0.0;
            }
            isFraction = true;
            idx++;
            continue;
        }
        if(c <= '9' && c >= '0') {
            if(isFraction) {
                divisor *= 10;
            } else if(isOnesPlace) {
                isOnesPlace = false;
            } else {
                val *= 10;
            }
            val += (float)(c - '0') / divisor;
        } else {
            // invalid character
            return 0;
        }
        idx++;
    }

    return isNegative ? -val : val;
}