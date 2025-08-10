# Arduino-Based Rover with RF24 Control

This project is an Arduino-powered rover, driven by DC motors and controlled via joystick with wireless RF24 communication on separate Arduino. This rover is completely wireless and even features an automatic obstacle detection and avoidance system via a HC-SR04 ultrasonic sensor.

## Features
- Ultrasonic sensor for obstacle detection and avoidance
- Analog joystick for forward and backward control of rover
- Twin RF24 radio transceivers for wireless communication between rover and joystick Arduinos
- DIY h-bridge using circuit logic and transistors
- Power supplies for on-board voltage regulation of motors and RF24 transcievers

## Goals
- Build a dual-motor wireless rover with automatic obstacle avoidance
- Learn how to use complex signaling methods (RF24) in conjunction with DC motors, ultrasonic sensors, and other more basic circuitry components
- Explore voltage regulation and power 

## Challenges and Solutions
- In my original setup, I was using an L293D motor driver to control the two wheels separately, thus allowing my rover to turn either direction by only driving one motor at a time. However, in testing this component was short circuited and consequently fried.
    - After assessing the damage and isolating the issue to a single faulty connection, I replaced the chip with four transistors, each with a digital input from the arduino connected to its gate pin, to simulate the h-bridge found within the L293D. Although, a limited number of transistors on my end meant that I could no longer control the wheels separately, restricitng my rover to either forward or backward movement.
- Getting started with the new RF24 modules I had recently acquired, I had trouble adapting the starter code to my needs. Specifically, I needed to be sending both the x coordinate and y coordinate of my analog joystick between transceivers at once.
    - I get around the one variable limitation, I created a struct that would hold both the x and y coordinate and sent that between transceivers. By creating an identical struct on the receiving end, I was able to effectively communicate both of my variables at once.
- Half of the time when driving the motors I would encounter a problem where my RF24 transceiver on the car would suddenly stop working. A closer look showed that since my motors and my RF24 was connected to the same battery, driving my motors would put too much strain on the supply and the RF24 would not be powered with the 3.3v that it needs to function.
    - I used an external power regulator to turn my 9v battery into a steady, and more importantly, dependable 3.3v that would ensure the RF24 modules would not falter even when driving the motor or using my joystick.
- Further experimentation with the RF24 modules seemed to fail even though debugging attempts showed that both transceivers were on and connected correctly. Even still, no information was being received by the car.
    - Looking closer into the literature on these modules revealed that I was sending information way too quickly for it to be accurately received. Lengthening my interval between transmissions allowed me to send information, just with a little bit of delay (around 0.2 seconds). This fix allowed the rover to remain wireless while minimally encroaching on the control given to the user.

## Hardware Used
- Arduino Uno
- Arduino Nano
- Ultrasonic sensor (HC-SR04)
- RF24 transceivers
- NPN transistors
- Three 9v batteries
- External power regulator
- DC motors
- Analog Joystick

## Software
- Arduino IDE
- Libraries:
    - SPI
    - SR04
    - RF24
    - nRF24LO1
  
## How to Use
- Follow circuit diagram to build system as shown below:
    [.jpg file](./rf24-rover-photo.jpg)    
- Upload Arduino code as shown below:
    - [Car .ino file](./rf24-rover-car-code.ino)
    - [Joystick .ino file](./rf24-rover-car-code.ino)
- Connect all power supplies as directed
- Use joystick to drive motors forward and backward as desired

## Demo
- [.mp4 file](./rf24-rover-video.mp4)

## Testing
- Flawlessly stops before any collisions in front of rover
- Motors are strong enough to drive rover over carpets, tile, and hardwood

## Future Improvements
- Add PWM on gate pins to allow for greater speed control
- Acquire more transistors to allow for individual wheel control
  

