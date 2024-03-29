#include <Arduino.h>
#include "../includes/SerialProcessor.h"

void SerialProcessor::setup() {
    txReadIndex = rxReadIndex = txWriteIndex = rxWriteIndex = rxCount = rxCount = 0;
}

void SerialProcessor::loop(int test) {
    // quickly retrieve from Serial buffer
    // frees it up to receive more data
    while(Serial.available()) {
        if(rxCount >= BUFFER_LENGTH_RX) {
            Serial.println("RX OVERFLOW");
            break;
        }
        rxBuffer[rxWriteIndex++] = Serial.read();
        if(rxWriteIndex >= BUFFER_LENGTH_RX) {
            rxWriteIndex = 0;
        }
        rxCount++;
    }

    

    // send data from transmit buffer
    while(txCount > 0) {
        Serial.write(txBuffer[txReadIndex++]);
        if(txReadIndex >= BUFFER_LENGTH_TX) {
            txReadIndex = 0;
        }
        txCount--;
    }


    // now parse our received data
    while(rxCount > 0) {
        parseReceived(rxBuffer[rxReadIndex++]);
        if(rxReadIndex >= BUFFER_LENGTH_RX) {
            rxReadIndex = 0;
        }
        rxCount--;
    }
}

size_t SerialProcessor::write(uint8_t data) {
    if(txCount >= BUFFER_LENGTH_TX) {
        Serial.println("TX OVERFLOW");
        return 0;
    }
    txBuffer[txWriteIndex++] = data;
    if(txWriteIndex >= BUFFER_LENGTH_TX) {
        txWriteIndex = 0;
    }
    txCount++;
    
    return 1;
}

size_t SerialProcessor::write(const uint8_t *payload, size_t length) {
    while(length --) {
        if(write(*payload++) == 0) break;
    }
}
/*
void SerialProcessor::writeString(const char *payload) {
    char *tmpPointer = payload;
    uint8_t strLength = 0;

    while(*tmpPointer++ != '\0') {
        strLength++;
    }

    write(payload, strLength);
}
*/
void SerialProcessor::parseReceived(char data) {

    // handle special characters
    switch (data)
    {
        case '$': //start characters
            spState = SPGotStartChar;
            parseCount = 0;
            break;
        case '\n':
        case '\r':
        case '*': //cts debug
            if(parseCount >= 3) {
                spState = SPVerifyPayload;
            } else {
                spState = SPWaitForStartChar;
            }
            break;
        default: break;
    }

    // build received data
    switch(spState) {
        case SPWaitForStartChar: break;
        case SPGotStartChar:
            spState = SPGetContent;
            break;
        case SPGetContent:
            parseBuffer[parseCount++] = data;
            break;
        case SPVerifyPayload:
            if(calculateChecksum() == 0xFF || parseBuffer[parseCount-1] == '&') {  // cts debug
                if(processedCallback != nullptr) {
                    processedCallback(parseBuffer, parseCount);
                }
                Serial.println("RxCmd");
            } else {
                Serial.println("INVALID");
            }
            spState = SPWaitForStartChar;
            parseCount = 0;
            break;
    }
}

uint8_t SerialProcessor::calculateChecksum() {
    uint8_t i = 0;
    uint8_t checksum = 0;
    while(i < parseCount) {
        checksum += (uint8_t)parseBuffer[i++];
    }
    return checksum;
}

void SerialProcessor::registerProcessedCallback(PROCESSED_CALLBACK_FUNCTION(func,,)) {
    processedCallback = func;
    Serial.println("Registered Callback");
}