////// currently built for use w/Circuit Playground Express
    // planning to add:
        // post-death duration
        // and other changes for bigger movements (if servo capable)
        // look into random(); see bottom of file

#include <Servo.h> // look more into this ??
#include <Adafruit_Circuit_Playground.h>

int timer = 0;
unsigned long curMillis;
uint16_t delayMS = 400;

//// for servo
Servo myServo;
const uint8_t servoPin = 10;  // A3 == D10
const uint8_t minAngle = 1;
const uint8_t maxAngle = 174;  // reduced b/c the servo clicks & makes other weird noises
const uint8_t angleRange = maxAngle - minAngle;
const uint8_t middleAngle = angleRange / 2;
uint8_t item = 0;
// uint8_t twitch = 4;

//// for mag connector
bool discontinuous;
const uint8_t contPin = 1;  // A7 == D1

//// for onboard LED
const uint8_t ledBtnPin = 4;
const uint8_t onboardLedPin = 13;

//// put your setup code here, to run once:
void setup() {
    // Serial.begin(9600);
    // delay(1000);
    CircuitPlayground.begin();
    myServo.attach(servoPin);
    pinMode(onboardLedPin, OUTPUT);  // just for onboard red LED
    pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
    pinMode(contPin, INPUT_PULLUP);
}

//// put your main code here, to run repeatedly:
void loop() {
    curMillis = millis();
    discontinuous = digitalRead(contPin);  // HIGH & True == 1; (so probably) LOW & False == 0 --> so should be int, not bool? ????
    if(CircuitPlayground.slideSwitch()) return;
    if(curMillis - timer > delayMS){
        // if accidental [continuity] disconnect then reconnect, it will still fxn as long as death twitch state not complete/expired; twitch state not yet set in this draft
        !discontinuous ? contFunc(item) : discontFunc(item);
    }
    // (quick functioning test) turn on D13 LED when D4 btn is pressed; will 1st read the status of the LED 
    digitalWrite(onboardLedPin, digitalRead(ledBtnPin));
}

//// functions
    // if accidental [continuity] disconnect then reconnect, it will still function as long as death twitch state not complete/expired; twitch state not yet set in this draft
    
// used while continuity through mag connector (or other method of continuity)
void contFunc(uint8_t itemState) {
     switch (itemState) {
        case 0:
            contCaseBlock(minAngle, 1);
            break;
        case 1:
            contCaseBlock(middleAngle, 2);
            break;
        case 2:
            contCaseBlock(maxAngle, 3);
            break;
        default:
            contCaseBlock(middleAngle, 0);
            break;
    }
}

// used while NO continuity through mag connector (or other method of continuity)
void discontFunc(uint8_t itemState) {
    switch (itemState) {
        case 0:
            contCaseBlock(minAngle, 1);
            break;
        case 1:
            contCaseBlock(maxAngle, 0);
            break;
        default:
            item = 0;
            break;
    }
}

// used w/in contFunc & discontFunc
void contCaseBlock(uint8_t angle, uint8_t newItemVal) {
    myServo.write(angle);
    timer = curMillis;
    item = newItemVal;
}

// look into using random(min, max);
  // https://www.arduino.cc/reference/en/language/functions/random-numbers/random/