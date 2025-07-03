//security-alarm-system built by Tyler Marrazzo

#include <LiquidCrystal.h>
#include "IRremote.h"
#include "SR04.h"

#define TRIG_PIN 5
#define ECHO_PIN 4

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN); //ultrasonic sensor setup
long a; //distance read by ultrasonic sensor
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //LCD setup

int buzzer = 13; //active buzzer pin
unsigned long lastBuzzerToggle = 0; //last time the buzzer toggled
bool buzzerState = false; //current state of the buzzer
const unsigned long buzzerInterval = 200; //interval for buzzer toggle in milliseconds

int receiver = 6; //IR pin
IRrecv irrecv(receiver); //create instance of 'irrecv'

int volUp = 3; //pin to increase volume
bool volUpState; //to configure BJT's
int volDown = 2; //pin to decrease volume
bool volDownState; //to configure BJT's

unsigned long timeSinceTrig; //for calculation of elapsed time since alarm was triggered
unsigned long elapsedTime; //same as above

String correctPassword = "0123"; //example password to be set by user
String currentPassword = ""; //placeholder for inputted password

String state; //keep track of where the alarm is right now

void translateIR() { //takes action based on IR code received
  switch (irrecv.decodedIRData.decodedRawData) //map the IR code to the remote key
  {
    case 0xBA45FF00: //arm the alarm
    Serial.println("POWER"); 
    if(state != "triggered") { //if alarm is triggered, we must silence it before we do anything else
      state = "armed"; 
      lcd.clear(); 
      lcd.print("ARMED"); //display state
    }
    break; 
    
    case 0xB847FF00: //disarm the alarm
    Serial.println("FUNC/STOP"); 
    if(state != "triggered") { //same logic as armed
      state = "disarmed"; 
      lcd.clear(); 
      lcd.print("DISARMED"); 
    } 
    break; 
    
    case 0xBF40FF00: //silence the alarm
    Serial.println("PAUSE"); 
    if(state == "silenced") { //if already silenced, unsilence
      state = "triggered"; //timer is preserved as if it was triggered the entire time we were in silence state
      lcd.clear();
      lcd.print("TRIGGERED");
      
    } else if(currentPassword == correctPassword) { //attempted password is correct
      state = "silenced"; 
      lcd.clear(); 
      lcd.print("SILENCED");
      currentPassword = ""; //clear attempted password
    }
    else {
      currentPassword = ""; //if incorrect, still clear the password
    } 
    break; 
    
    case 0xB946FF00: //turn the volume up by one
    Serial.println("VOL+"); 
    if(volDownState == true) { //if volDown is already active, then turning volUp should go back to default volume
      digitalWrite(volDown, HIGH); //go from min to mid
      volDownState = false;
      Serial.println("mid");
    } else {
      digitalWrite(volUp, HIGH); //go from mid to max
      volUpState = true;
      Serial.println("max");
    }
    break;
    
    case 0xEA15FF00: //turn the volume down by one
    Serial.println("VOL-"); 
    if(volUpState == true) { //same logic as VOL+
      digitalWrite(volUp, LOW); //go from max to mid
      volUpState = false;
      Serial.println("mid");
    } else {
          digitalWrite(volDown, LOW); //go from mid to min
          volDownState = true;
          Serial.println("min");
    }
    break;
    
    case 0xE916FF00: 
    Serial.println("0");
    currentPassword += "0"; //add key to attempted password
    break;
    
    case 0xF30CFF00: 
    Serial.println("1");
    currentPassword += "1"; //same logic as above so below
    break;
    
    case 0xE718FF00: 
    Serial.println("2"); 
    currentPassword += "2";   
    break;
    
    case 0xA15EFF00: 
    Serial.println("3");    
    currentPassword += "3";   

    break;
    
    case 0xF708FF00: 
    Serial.println("4");    
    currentPassword += "4";
    break;
    
    case 0xE31CFF00: 
    Serial.println("5");  
    currentPassword += "5";  
    break;
    
    case 0xA55AFF00: 
    Serial.println("6");
    currentPassword += "6";    
    break;
    
    case 0xBD42FF00: 
    Serial.println("7");   
    currentPassword += "7"; 
    break;
    
    case 0xAD52FF00: 
    Serial.println("8");  
    currentPassword += "8";  
    break;
    
    case 0xB54AFF00: 
    Serial.println("9");   
    currentPassword += "9"; 
    break;
    
    default:
      Serial.println(" other button   ");
  } //end case
  delay(100); //do not get immediate repeat
} //END translateIR

void setup() {
  pinMode(buzzer, OUTPUT); //initialize the buzzer pin as an output
  digitalWrite(buzzer, LOW);
  
  pinMode(volUp, OUTPUT); //volUp is an output
  digitalWrite(volUp, LOW); //activating this BJT would connect buzzer to ground, so max volume
  volUpState = false;
  pinMode(volDown, OUTPUT); //volDown is an output
  digitalWrite(volDown, HIGH); //this BJT should be on by default and switched off when we want to increase volume, so this must start HIGH
  volDownState = false;
  
  timeSinceTrig = 0;
  elapsedTime = 0;
  
  lcd.begin(16, 2); //sixteen columns, two rows
  lcd.print("DISARMED"); //default to disarmed
  
  Serial.begin(9600);
  
  irrecv.enableIRIn(); //start the receiver
  
  state = "disarmed"; //alarm starts disarmed
}
void loop() {
  a=sr04.Distance(); //measure distance from ultrasonic sensor
  if(a < 3 && state == "armed") { //alarm triggered!
    lcd.clear();
    lcd.print("TRIGGERED "); //add a space for time elapsed since 
    state = "triggered";
    timeSinceTrig = millis(); //get current time
  }
  if(state == "triggered") {
    elapsedTime = (millis() - timeSinceTrig) / 1000; //calculate time since triggered in seconds
    lcd.setCursor(0,1);
    lcd.print(elapsedTime);
    unsigned long currentMillis = millis();
    if (currentMillis - lastBuzzerToggle >= buzzerInterval) { //non-blocking buzzer and LED flashing
      lastBuzzerToggle = currentMillis;
      buzzerState = !buzzerState; //alternate buzzer state
      digitalWrite(buzzer, buzzerState ? HIGH : LOW);
    }
    lcd.setCursor(0, 0); //allow for new states to be printed in case alarm is silenced
  } else {
    digitalWrite(buzzer, LOW); //ensure buzzer is off in non-triggered states
  }
  if(irrecv.decode()) { //have we received an IR signal?
    translateIR(); //send to translate whatever signal we got
    irrecv.resume(); //receive the next value
  }
}
