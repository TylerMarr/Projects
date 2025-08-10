#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); //CE, CSN
const byte addresses[][6] = {"00001", "00002"};

struct analogSignals {
  int xDirection;
  int yDirection;
};

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  Serial.begin(9600);
  radio.stopListening();
}

void loop() {
  static unsigned long timer = 0;
   unsigned long interval = 200;
   if (millis() - timer >= interval)
   {
      timer = millis();
      analogSignals package;
      package.xDirection = analogRead(A0);
      package.yDirection = analogRead(A1);
      radio.write(&package, sizeof(analogSignals));
   }
}
