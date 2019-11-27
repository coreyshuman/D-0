#include <Arduino.h>
#include <stdlib.h>
#include "../includes/CommandController.h"
#include "../includes/Settings.h"

typedef struct _command_table_t
{
    char commandCode[2];
    uint8_t paramsMin;
    uint8_t paramsMax;
    void (*commandHandler)(CommandController &controller, _command_table_t cTable, command_t command);

} command_table_t;

void commandArm(CommandController &controller, command_table_t cTable, command_t command) {
    Serial.println("Command Arm");
    Serial.print(" min ");Serial.print(cTable.paramsMin);Serial.print(" max ");Serial.print(cTable.paramsMax);
    Serial.print(" len ");Serial.print(command.payloadLength);
}

void printConfig(CommandController &controller, command_table_t cTable, command_t command) {
    Serial.println("Print Config");
    Serial.print(" min ");Serial.print(cTable.paramsMin);Serial.print(" max ");Serial.print(cTable.paramsMax);
    Serial.print(" len ");Serial.print(command.payloadLength);
}

typedef struct _param_t {
    char *dataPtr;
    uint8_t length;
} param_t;

class Params {
    public:
        uint8_t count;
        Params(uint8_t);
        ~Params();
        bool add(char *paramPtr, uint8_t paramLength);
        param_t &operator[](int);

    private:
        param_t *params;
        uint8_t paramMax;
};

param_t &Params::operator[](int index) {
    //if(index < paramMax) {
        return params[index];
    //}
    //return (param_t)nullptr;
}

bool Params::add(char *paramPtr, uint8_t paramLength) {
    if(count >= paramMax) return false;
    params[count].dataPtr = paramPtr;
    params[count].length = paramLength;
    count++;
    return true;
}

Params::Params(uint8_t max) {
    params = new param_t[max];
    paramMax = max;
    count = 0;
}

Params::~Params() {
    delete []params;
}


Params parseParams(char *payload, const uint8_t length, const uint8_t maxParams) {
    Params params(maxParams);
    int i;
    char *tmpParamPtr = &payload[0];
    uint8_t tmpParamLength = 0;

    for(i = 0; i < length; i++) {
        if(payload[i] == ',') {
            // zero pad for easy string extraction
            payload[i] = '\0';
            if(!params.add(tmpParamPtr, tmpParamLength)) {
                return params; // exceeded max params so exit
            }
            tmpParamPtr = &payload[i+1];
            tmpParamLength = 0;
        } else {
            tmpParamLength++;
        }
    }
    // add final param if exists
    if(tmpParamLength > 0) {
        params.add(tmpParamPtr, tmpParamLength);
    }

    return params;
}

void testtest(CommandController &controller, command_table_t cTable, command_t command) {
    int i;
    Serial.println("test");
    Serial.print(" min ");Serial.print(cTable.paramsMin);Serial.print(" max ");Serial.print(cTable.paramsMax);
    Serial.print(" len ");Serial.print(command.payloadLength);Serial.println();

    Params params = parseParams(command.payload, command.payloadLength, cTable.paramsMax);
    Serial.print("Params found: ");Serial.print(params.count);Serial.println();
    if(params.count < cTable.paramsMin) {
        Serial.println("Not enough parameters.");
    }

    for(i = 0; i<params.count; i++) {
        Serial.print(i);Serial.print(": ");
        Serial.println(params[i].dataPtr);
    }
}



const command_table_t commandTable[COMMAND_TABLE_COUNT] = {
    {"ar", 0, 0, commandArm},
    {"pc", 0, 0, printConfig},
    {"tt", 1, 3, testtest}
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
    int i = 0;
    commandBuffer[commandBufferWriteIndex].commandCode[0] = payload[0];
    commandBuffer[commandBufferWriteIndex].commandCode[1] = payload[1];
    // copy payload, don't include command code or checksum
    for(i = 0; i < length - 3 && i < COMMAND_PAYLOAD_LENGTH-1; i++) {
        commandBuffer[commandBufferWriteIndex].payload[i] = payload[i+2];
    }
    commandBuffer[commandBufferWriteIndex].payload[i] = '\0';
    commandBuffer[commandBufferWriteIndex].payloadLength = length - 3;
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

void CommandController::processCommand(command_t &command) {
    int i, j;
    command_table_t potentialCommand;
    for(i = 0; i < COMMAND_TABLE_COUNT; i++) {
        // copy command table in flash to local ram variable
        const uint8_t *commTablePtr = (uint8_t *)&commandTable[i];
        uint8_t *potCommPtr = (uint8_t *)&potentialCommand;
        for(j = 0; j < sizeof(command_table_t); j++) {
            *potCommPtr++ = *commTablePtr++;
        }
        //potentialCommand = commandTable[i];
        if(command.commandCode[0] != potentialCommand.commandCode[0]) continue;
        if(command.commandCode[1] != potentialCommand.commandCode[1]) continue;

        Serial.println("Command Found!");
        //(this->*commandTable[i].commandHandler)();
        potentialCommand.commandHandler(*this, potentialCommand, command);
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