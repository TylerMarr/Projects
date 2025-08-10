#include "SR04.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define Forward 5
#define Backward 6
#define trig_pin 2
#define echo_pin 3

SR04 sr04 = SR04(echo_pin,trig_pin);
long a;

struct analogSignals {
  int xDirection = 0;
  int yDirection = 0;
};

bool canGo;

RF24 radio(7, 8); //CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup() {
  Serial.begin(9600);
  pinMode(Forward, OUTPUT);
  digitalWrite(Forward, LOW);
  pinMode(Backward, OUTPUT);
  digitalWrite(Backward, LOW);
  
  canGo = true;
  
  radio.begin();
  radio.openReadingPipe(0, addresses[1]); // 00002, opposite of transceiver
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void forward() {
  digitalWrite(Backward, LOW);
  digitalWrite(Forward, HIGH);
}

void backward() {
  digitalWrite(Forward, LOW);
  digitalWrite(Backward, HIGH);
}
void halt() {
  digitalWrite(Forward, LOW);
  digitalWrite(Backward, LOW);
}

void loop() {
  delay(200);
  if(radio.available()) {
    while(radio.available()) {
      analogSignals package;
      package.xDirection = 0;
      package.yDirection = 0;
      radio.read(&package, sizeof(analogSignals));
      Serial.println(package.xDirection);
      Serial.println(package.yDirection);
      a = sr04.Distance();
      Serial.println(a);
      if(a < 8) {
        canGo = false;
      }
      else {
        canGo = true;
      }
      if(package.xDirection > 600 && canGo == true) { //FORWARD
        forward();
      }
      else if(package.xDirection < 300) { //BACKWARD
        backward();
      }
      else {
        halt();
      }
      //interpret signals
    }
    delay(5);
  }
  else {
    halt();
  }
}
