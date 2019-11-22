# D-0 Firmware
Arudino Uno firmware for a D-0 droid!

## Features
- Audio
- IMU stabilization

## Project Setup
### Using Arduino
- Use Arduino editor to open `D0Firmware` directory.
- Select board type: `Arduino Uno`
- Select correct serial port for you Arduino Uno.

### Using Visual Studio Code
- Install `Arduino` extension by Microsoft
- Run `Arduino: Initialize`
- Update `.vscode/c_cpp_properties.json` `includePath` section with missing lines (using your machine's paths) per:
```
"includePath": [
    "${workspaceFolder}/**",
    "d:\\Program Files (x86)\\Arduino\\tools\\**",
    "d:\\Program Files (x86)\\Arduino\\hardware\\arduino\\avr\\**",
    "d:\\Program Files (x86)\\Arduino\\hardware\\tools\\avr\\**",
    "C:\\Users\\Corey\\OneDrive\\Documents\\Arduino\\libraries\\**"
],
```
- Run `Arduino: Board Config` and select `Arduino Uno`
- Run `Arduino: Select Serial Port` and select the port of your Arduino Uno.

## Required Libraries
- I2Cdev
- MPU6050
- Adafruit VS1053 FilePlayer
- Adafruit PWM Servo Driver

## Building the Project
### Arduino
- Click `Verify` button
### Visual Studio Code
- Run `Arduino: Verify`

## Installing Firmware
### Arduino
- Click `Upload` button
### Visual Studio Code
- Run `Arduino: Upload`

## Running Tests
Uses the Catch2 framework: https://github.com/catchorg/Catch2 (single file include included)
- Requires g++ and make. I used Linux Subsystem for Windows and ran the `make` command from Ubuntu. If already on Linux or MacOS you should be good to go, otherwise lookup how to install c++ buildtools for you architecture.
- Execute `run_tests.sh`
