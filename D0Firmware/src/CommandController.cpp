#include <Arduino.h>
#include <stdlib.h>
#include "../includes/CommandController.h"
#include "../includes/Settings.h"
#include "../includes/Util.h"

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

void cResponseCommandOutOfRange(CommandController &controller) {
    controller.serialProcessorPtr->println(F("OOR"));
}

/*********************************************************************
 * Command Handler Callbacks
 ********************************************************************/
/*******************
 * cRuntimeTelemetry
 * 
 * Payload format: ALY, ALX, ARY, ARX, none,btn1,btn2,stat 
 *        Example: 7F9F,80DF,7F5F,801F,0000,0000,0000,0000
 * 
 * ******************/
void cRuntimeTelemetry(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    controller.commandState.driveSpeed = ((int16_t)(Util::htoi(params[0].dataPtr) - 0x8000)) >> 4;
    controller.commandState.turnSpeed = ((int16_t)(Util::htoi(params[1].dataPtr) - 0x8000)) >> 4;
    controller.commandState.flags.map = Util::htoi(params[7].dataPtr);
    controller.commandState.lastUpdateTick = millis();
    controller.commandState.valid = true;
    cResponseOK(controller);
    Serial.println("TEL ");
    Serial.print("flag ");Serial.print(controller.commandState.flags.map, HEX);Serial.print(" arm ");Serial.print(controller.commandState.flags.armed);
}


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

/*******************
 * cRuntimeTelemetry
 * 
 * Payload format: servoNum, minimum, center, maxiumum, deadband
 *        Example: 0, 150, 375, 600, 50
 * 
 * servoNum:
 *      0. leftDriveServoCal
 *      1. rightDriveServoCal;
 *      2. neckAngleServoCal;
 *      3. neckLeanServoCal;
 *      4. headTurnServoCal;
 *      5. headTiltServoCal;
 * ******************/
void cSettingsServoCalibration(CommandController &controller, command_table_t &cTable, command_t &command, Params &params) {
    _servo_calibration_t *servoCalibrationPtr = &controller.settingsPtr->config.leftDriveServoCal;
    uint8_t servoNum = Util::atoi(params[0].dataPtr);
    if(servoNum > 5) return (void)cResponseCommandOutOfRange(controller);

    servoCalibrationPtr += servoNum;
    servoCalibrationPtr->minimum = Util::atoi(params[1].dataPtr);
    servoCalibrationPtr->center = Util::atoi(params[2].dataPtr);
    servoCalibrationPtr->maximum = Util::atoi(params[3].dataPtr);
    servoCalibrationPtr->deadband = Util::atoi(params[4].dataPtr);
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

const command_table_t commandTable[COMMAND_TABLE_COUNT] = {
    // runtime
    {"rr", 0, 0, cRuntimeArm},
    {"rt", 8, 8, cRuntimeTelemetry},
    
    {"tt", 1, 3, testtest},
    // imu
    {"ic", 0, 0, cImuCalibrate},
    {"ip", 0, 0, cImuPrint},
    // settings
    {"sf", 0, 0, cSettingsFreeAnimation},
    {"sc", 5, 5, cSettingsServoCalibration},
    {"sp", 0, 0, cSettingsPrint},
    {"sr", 1, 3, cSettingsReset},
    {"ss", 1, 3, cSettingsSave}
};


void CommandController::initCommandState() {
    commandState.driveSpeed = COMMAND_RANGE_MID;
    commandState.turnSpeed = COMMAND_RANGE_MID;
    commandState.valid = false;
}

void CommandController::setup(SerialProcessor &serialProcessor, Settings &settings, IMU &imu, BatteryMonitor &batteryMonitor) {
    settingsPtr = &settings;
    imuPtr = &imu;
    batteryMonitorPtr = &batteryMonitor;
    serialProcessorPtr = &serialProcessor;

    initCommandState();

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

    if(commandState.valid && millis() - commandState.lastUpdateTick > TELEMETRY_TIMEOUT) {
        initCommandState();
    }
}