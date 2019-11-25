#include <Arduino.h>
#include "../includes/SerialProcessor.h"

void SerialProcessor::setup() {
    txReadIndex = rxReadIndex = txWriteIndex = rxWriteIndex = rxCount = rxCount = 0;
}

void SerialProcessor::loop(int test) {
    // quickly retrieve from Serial buffer
    // frees it up to receive more data
    while(Serial.available()) {
        rxBuffer[rxWriteIndex++] = Serial.read();
        if(rxWriteIndex >= BUFFER_LENGTH_RX) {
            rxWriteIndex = 0;
        }
        rxCount++;
    }

    if(rxCount > BUFFER_LENGTH_RX) {
        rxCount = BUFFER_LENGTH_RX;
        Serial.println("RX OVERFLOW");
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

void SerialProcessor::writeBytes(const byte *payload, uint8_t length) {
    while(length > 0) {
        txBuffer[txWriteIndex++] = *payload++;
        if(txWriteIndex >= BUFFER_LENGTH_TX) {
            txWriteIndex = 0;
        }
        txCount++;
        length--;
    }

    if(txCount > BUFFER_LENGTH_TX) {
        txCount = BUFFER_LENGTH_TX;
        Serial.println("TX OVERFLOW");
    }
}

void SerialProcessor::writeString(const char *payload) {
    char *tmpPointer = payload;
    uint8_t strLength = 0;

    while(*tmpPointer++ != '\0') {
        strLength++;
    }

    writeBytes(payload, strLength);
}

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
            if(parseCount >= 4) {
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
            if(calculateChecksum() == 0xFF || parseBuffer[parseCount-1] == 0x00) {  // cts debug
                receivedPayloadCallback();
                Serial.println("Verified");
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

void SerialProcessor::receivedPayloadCallback() {
    Serial.println("Received Payload");
}