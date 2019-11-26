#include "../includes/Motors.h"
#include "../includes/Config.h"
#include "../includes/platform/Platform.h"


void Motors::setup() {
    //pwm.begin(); // don't call begin(), it will reset Wire
    pwm.reset();
    // In theory the internal oscillator is 25MHz but it really isn't
    // that precise. You can 'calibrate' by tweaking this number till
    // you get the frequency you're expecting!
    pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates
}

void Motors::loop(command_state_t &commandState, imu_data_t &imuData, config_t &config) {
    int i;
    uint16_t pwmVal;
    int16_t commandVal;
    if(commandState.flags.armed) {
        int16_t stabilizationSpeed = 0;
        int16_t turnSpeedFactor = commandState.turnSpeed / 2;

        // stabilization
        if(config.freeAnimation.flags.stabilizationEnabled) {
            stabilizationSpeed += (int16_t)(imuData.ypr.x * (config.freeAnimation.stabilizationP + 60)); //cts debug
            
        }

        // left drive motor
        commandVal = commandState.driveSpeed + turnSpeedFactor;
        if(commandVal > COMMAND_RANGE_MID - config.leftDriveServoCal.deadband &&
            commandVal < COMMAND_RANGE_MID + config.leftDriveServoCal.deadband) {
                commandVal = COMMAND_RANGE_MID;
        } 
        commandVal += stabilizationSpeed;
        pwmVal = map(commandVal, COMMAND_RANGE_MIN, COMMAND_RANGE_MAX, 
            config.leftDriveServoCal.minimum, config.leftDriveServoCal.maximum);
        pwm.setPWM(DRIVE_MOTOR_LEFT, 0, pwmVal);
        Serial.print("Drive ");
        Serial.print(commandVal);
        Serial.print(" PWM ");
        Serial.print(pwmVal);
        Serial.print("\n");
        

        // right drive motor
        commandVal = commandState.driveSpeed + turnSpeedFactor;
        if(commandVal > COMMAND_RANGE_MID - config.leftDriveServoCal.deadband &&
            commandVal < COMMAND_RANGE_MID + config.leftDriveServoCal.deadband) {
                pwm.setPin(DRIVE_MOTOR_RIGHT, 0);
        } else {
            pwmVal = map(commandVal, COMMAND_RANGE_MIN, COMMAND_RANGE_MAX, 
                config.leftDriveServoCal.minimum, config.leftDriveServoCal.maximum);
            pwm.setPWM(DRIVE_MOTOR_RIGHT, 0, pwmVal);
        }



    } else {
        for(i=0; i<15; i++) {
            pwm.setPin(i, 0);
        }
        
        
    }
}