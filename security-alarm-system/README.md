# Arduino-Based Security Alarm System

This project is an Arduino-powered security alarm system that detects thieves and scoundrels alike using an ultrasonic sensor and provides user interaction through an infrared (IR) remote, LCD feedback, and an active buzzer. The system also features adjustable alarm volume and smart password-protected alarm silencing.

## Features
- Ultrasonic sensor for tripping the alarm
- IR remote for arming, disarming, silencing, and general user interface
- LCD display for live feedback on alarm status
- Password protection for silencing the alarm
- Adjustable buzzer volume

## Goals
- Build a functional and intuitive alarm system for home security
- Learn how to combine a multitude of circuit components, such as Ultrasonic sensor, LCD display, buzzer, and IR receiver
- Develop problem-solving skills in overcoming both hardware limitations and software challenges

## Hardware Used
- Arduino Uno
- Ultrasonic sensor (HC-SR04)
- IR receiver module and remote
- Active Buzzer
- 16x2 LCD display
- LED
- Potentiometer
- Resistors

## Software
- Arduino IDE
- Libraries:
    - LiquidCrystal
    - IRremote
    - SR04

## Code Overview
- loop() handles sensor, receiving IR commands, and alarm state updates
- translateIR() handles interpretation of IR commands and state transitions
- setup() initializes outputs and system status

## How to Use
- Follow circuit diagram to build system as shown below:
    - [.brd file](./security-alarm-system-diagram.brd)
    - [.png file](./security-alarm-system-screenshot.png)    
- Upload Arduino code as shown below
- Use IR remote to arm, disarm, or silence (with password)
- Put thief within 3 cm of Ultrasonic sensor to trigger (distance can be varied but effectiveness is not guarenteed)
- Change volume as desired

## Testing
- Ultrasonic sensor is accurate to 3 cm of detection
- IR receiver detects every button press flawlessly with no repeats or missed inputs
- Password is absolutely required to silence alarm in any way, save disconnecting the system from power

## Future Improvements
- Add WiFi or bluetooth connectivity for IoT functionality
- Create mobile app for remote control and monitoring
  
