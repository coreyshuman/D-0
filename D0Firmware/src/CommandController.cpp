#include <Arduino.h>
#include <stdlib.h>
#include "../includes/CommandController.h"
#include "../includes/Settings.h"

void commandArm() {
    Serial.println("Command Arm");
}

void printConfig() {
    Serial.println("Print Config");
}

#define COMMAND_TABLE_COUNT 2
const command_table_t commandTable[] = {
    {"ar", 0, 0, commandArm},
    {"pc", 0, 0, printConfig}
};



void CommandController::setup(SerialProcessor &serialProcessor, Settings &settings, IMU &imu, BatteryMonitor &batteryMonitor) {
    settingsPtr = &settings;
    imuPtr = &imu;
    batteryMonitorPtr = &batteryMonitor;
    serialProcessorPtr = &serialProcessor;

    commandState.driveSpeed = COMMAND_RANGE_MID;
    commandState.turnSpeed = COMMAND_RANGE_MID;

    commandBufferReadIndex = commandBufferWriteIndex = commandBufferCount = 0;
}

void CommandController::addCommand(char *payload, uint8_t length) {
    Serial.println("Got Processed Command");
    int i = 0;
    commandBuffer[commandBufferWriteIndex].commandCode[0] = payload[0];
    commandBuffer[commandBufferWriteIndex].commandCode[1] = payload[1];
    for(i = 0; i < length - 2, i < COMMAND_PAYLOAD_LENGTH; i++) {
        commandBuffer[commandBufferWriteIndex].payload[i] = payload[i+2];
    }
    commandBuffer[commandBufferWriteIndex].payloadLength = length - 2;
    commandBufferCount++;
    commandBufferWriteIndex++;

    if(commandBufferCount > COMMAND_BUFFER_COUNT) {
        commandBufferCount = COMMAND_BUFFER_COUNT;
        Serial.println("COMMAND OVERFLOW");
    }

    if(commandBufferWriteIndex >= COMMAND_BUFFER_COUNT) {
        commandBufferWriteIndex = 0;
    }
}

void CommandController::processCommand(command_t command) {
    int i;
    command_table_t potentialCommand;
    for(i = 0; i < COMMAND_TABLE_COUNT; i++) {
        potentialCommand = commandTable[i];
        if(command.commandCode[0] != potentialCommand.commandCode[0]) continue;
        if(command.commandCode[1] != potentialCommand.commandCode[1]) continue;

        Serial.println("Command Found!");
        potentialCommand.commandHandler();
        return;
    }
    Serial.println("Command not found.");
}

void CommandController::loop(int test) {
    while(commandBufferCount > 0) {
        processCommand(commandBuffer[commandBufferReadIndex++]);
        if(commandBufferReadIndex >= COMMAND_BUFFER_COUNT) {
            commandBufferReadIndex = 0;
        }
        commandBufferCount--;
    }
    /*
    if (Serial.available() > 0) {
        // read the incoming byte:
        char c = Serial.read();

        switch(c) {
            case 'a':
                commandState.flags.armed = !commandState.flags.armed;
                Serial.println(commandState.flags.armed ? F("Armed") : F("Not Armed"));
                break;
            case 'b': 
                settingsPtr->config.leftDriveServoCal.minimum = Serial.read() - 0x30;
                break;

            case 'i':
                Serial.print("ypr\t");
                Serial.print(imuPtr->imuData.ypr.x * 180/M_PI);
                Serial.print("\t");
                Serial.print(imuPtr->imuData.ypr.y * 180/M_PI);
                Serial.print("\t");
                Serial.println(imuPtr->imuData.ypr.z * 180/M_PI);
                break;
            case 's': 
                settingsPtr->save();
                break; 
            case 'd':
                settingsPtr->reset();   
            case 'c': 
                Serial.println("Calibrating...");
                settingsPtr->config.imuOffsets = imuPtr->calibrate(); 
                settingsPtr->save();
                Serial.println("Done.");
                break;
            case 'p': settingsPtr->printConfig(); break;
            case 'w':
                serialProcessorPtr->writeString("This is a test string.");
                break;
            case 'v':
                Serial.print("Bus Voltage:   "); Serial.print(batteryMonitorPtr->busVoltage); Serial.println(" V");
                Serial.print("Shunt Voltage: "); Serial.print(batteryMonitorPtr->shuntVoltage); Serial.println(" mV");
                Serial.print("Load Voltage:  "); Serial.print(batteryMonitorPtr->loadVoltage); Serial.println(" V");
                Serial.print("Current:       "); Serial.print(batteryMonitorPtr->current); Serial.println(" mA");
                Serial.print("Power:         "); Serial.print(batteryMonitorPtr->power); Serial.println(" mW");
                Serial.println("");
                break;
        }
        
    }
    */
}