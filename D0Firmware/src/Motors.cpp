#include "../includes/Motors.h"
#include "../includes/Config.h"
#include "../includes/platform/Platform.h"

void Motors::setup() {
    pwm = Adafruit_PWMServoDriver();
    pwm.begin();
    // In theory the internal oscillator is 25MHz but it really isn't
    // that precise. You can 'calibrate' by tweaking this number till
    // you get the frequency you're expecting!
    pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates

}

void Motors::loop(int test) {

}