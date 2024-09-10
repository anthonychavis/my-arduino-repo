////// currently built for use w/Circuit Playground Express
    // still needs:
        // post-death duration
        // and other changes for bigger movements
        // look into random(); see bottom of file

#include <Servo.h> // look more into this ??
#include <Adafruit_Circuit_Playground.h>

int timer = 0;
unsigned long curMillis;
uint16_t delayMS = 400;

//// for servo
Servo myServo;
const uint8_t servoPin = 10;  // A7 == D1
uint8_t minAngle = 1;
uint8_t maxAngle = 174;  // reduced b/c the servo clicks & makes other weird noises
uint8_t item = 0;
// uint8_t twitch = 4;

//// for mag connector
bool discontinuous;
const uint8_t contPin = 1;

//// for onboard LED
const uint8_t ledBtnPin = 4;
const uint8_t onboardLedPin = 13;  // A3 == D10

//// put your setup code here, to run once:
void setup() {
    // Serial.begin(9600);
    delay(1000);
    CircuitPlayground.begin();
    myServo.attach(servoPin);
    pinMode(onboardLedPin, OUTPUT);  // just for onboard red LED
    pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to 1)
    pinMode(contPin, INPUT_PULLUP);
}

//// put your main code here, to run repeatedly:
void loop() {
    curMillis = millis();
    discontinuous = digitalRead(contPin);  // HIGH & True == 1; (so probably) LOW & False == 0 --> so should be int, not bool? ????
    if(CircuitPlayground.slideSwitch()) {
        if(curMillis - timer > delayMS){
        if(!discontinuous) {
            contFunc(item);
        } else {
            discontFunc(item);
        }
        }
    }
    // (quick functioning test) turn on D13 LED when D4 btn is pressed; will 1st read the status of the LED then 
    digitalWrite(13, digitalRead(4));
}

//// functions
// used while continuity through mag connector (or other method of continuity)
void contFunc(int itemState) {
    switch (itemState) {
        case 0:
            myServo.write(minAngle);
            timer = curMillis;
            item = 1;
            break;
        case 1:
            myServo.write(maxAngle);
            timer = curMillis;
            item = 0;
            break;
        default:
            item = 0;
            break;
    }
}

// used while NO continuity through mag connector (or other method of continuity)
void discontFunc(int itemState) {
    switch (itemState) {
        case 0:
            myServo.write(minAngle);
            timer = curMillis;
            item = 1;
            break;
        case 1:
            myServo.write(90);
            timer = curMillis;
            item = 2;
            break;
        case 2:
            myServo.write(maxAngle);
            timer = curMillis;
            item = 3;
            break;
        // case 3:
        default:
            myServo.write(90);
            timer = curMillis;
            item = 0;
            break;
    }
}

// look into using random(min, max);
  // https://www.arduino.cc/reference/en/language/functions/random-numbers/random/