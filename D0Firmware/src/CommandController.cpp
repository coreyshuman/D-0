#include <Arduino.h>
#include <stdlib.h>
#include "../includes/CommandController.h"
#include "../includes/Settings.h"

const char TAB = ("\t");
const char NEWLINE = ("\n");

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

typedef struct _command_table_t
{
    char commandCode[2];
    uint8_t paramsMin;
    uint8_t paramsMax;
    void (*commandHandler)(CommandController &controller, _command_table_t &cTable, command_t &command, Params &params);

} command_table_t;



void testtest(CommandController &controller, command_table_t &cTable, command_t &command, Params &params2) {
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

/*********************************************************************
 * Response Helpers
 ********************************************************************/
void cResponseOK(CommandController &controller) {
    controller.serialProcessorPtr->println(F("OK"));
}

void cResponseNotEnoughParams(CommandController &controller) {
    controller.serialProcessorPtr->println(F("NEP"));
}

void cResponseCommandNotFound(CommandController &controller) {
    controller.serialProcessorPtr->println(F("CNF"));
}

void cRuntimeFreeAnimation(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    //controller.commandState.flags.
}


/*********************************************************************
 * Command Handler Callbacks
 ********************************************************************/
void cRuntimeArm(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.commandState.flags.armed = true;
    cResponseOK(controller);
}

void cBatteryPrint(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.serialProcessorPtr->print("Bus Voltage:   "); controller.serialProcessorPtr->print(controller.batteryMonitorPtr->busVoltage); controller.serialProcessorPtr->println(" V");
    controller.serialProcessorPtr->print("Shunt Voltage: "); controller.serialProcessorPtr->print(controller.batteryMonitorPtr->shuntVoltage); controller.serialProcessorPtr->println(" mV");
    controller.serialProcessorPtr->print("Load Voltage:  "); controller.serialProcessorPtr->print(controller.batteryMonitorPtr->loadVoltage); controller.serialProcessorPtr->println(" V");
    controller.serialProcessorPtr->print("Current:       "); controller.serialProcessorPtr->print(controller.batteryMonitorPtr->current); controller.serialProcessorPtr->println(" mA");
    controller.serialProcessorPtr->print("Power:         "); controller.serialProcessorPtr->print(controller.batteryMonitorPtr->power); controller.serialProcessorPtr->println(" mW");
    controller.serialProcessorPtr->println();
    cResponseOK(controller);
}

void cImuCalibrate(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.imuPtr->calibrate();
    cResponseOK(controller);
}

void cImuPrint(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.serialProcessorPtr->print(F("ypr\t"));
    controller.serialProcessorPtr->print(controller.imuPtr->imuData.ypr.x * 180/M_PI);
    controller.serialProcessorPtr->print(TAB);
    controller.serialProcessorPtr->print(controller.imuPtr->imuData.ypr.y * 180/M_PI);
    controller.serialProcessorPtr->print(TAB);
    controller.serialProcessorPtr->print(controller.imuPtr->imuData.ypr.z * 180/M_PI);
    controller.serialProcessorPtr->println();
}

void cSettingsFreeAnimation(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {

}

void cSettingsServoCalibration(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {

}

void cSettingsPrint(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.settingsPtr->printConfig();
    cResponseOK(controller);
}

void cSettingsReset(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.settingsPtr->reset();
    cResponseOK(controller);
}

void cSettingsSave(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.settingsPtr->save();
    cResponseOK(controller);
}

const command_table_t commandTable[] = {
    // runtime
    {"rf", 0, 0, cRuntimeFreeAnimation},
    {"rr", 0, 0, cRuntimeArm},
    
    {"tt", 1, 3, testtest},
    // imu
    {"ic", 0, 0, cImuCalibrate},
    {"ip", 0, 0, cImuPrint},
    // settings
    {"sf", 0, 0, cSettingsFreeAnimation},
    {"sc", 0, 0, cSettingsServoCalibration},
    {"sp", 0, 0, cSettingsPrint},
    {"sr", 1, 3, cSettingsReset},
    {"ss", 1, 3, cSettingsSave}
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
        Serial.println(F("COMMAND OVERFLOW"));
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

        if(command.commandCode[0] != potentialCommand.commandCode[0]) continue;
        if(command.commandCode[1] != potentialCommand.commandCode[1]) continue;

        Params params = parseParams(command.payload, command.payloadLength, potentialCommand.paramsMax);
        if(params.count < potentialCommand.paramsMin) {
            cResponseNotEnoughParams(*this);
            return;
        }
        potentialCommand.commandHandler(*this, potentialCommand, command, params);
        return;
    }
    cResponseCommandNotFound(*this);
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