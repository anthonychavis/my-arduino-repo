/*
NOTE:
    - CPx refers to pins of the Circuit Playground Express
        - might have to change value for other boards
    - functions are not hoisted in this file
*/

/*
TEST SPEED w/inline vs w/o inline

#define digitalPinToInterrupt(P) ??
- why interrupt ??
- https://reference.arduino.cc/reference/en/language/functions/external-interrupts/digitalpintointerrupt/
*/

#include <Servo.h> // look more into this ?? #define MAX_SERVOS 0 ?? 

// VARS

// time
unsigned long curMillis, timer = 0;
uint16_t delayMS;

// for servo
// Servo library disables analogWrite() on pins 9 & 10 !!
Servo myServo;
#define servoPin 10  // A3 == D10 - CPx; pwm/~
// test higher minAng & lower maxAng first to check how much the gears amplify the angles !!
const uint8_t minAng = 10;  // increased b/c the servo clicks & makes other weird noises
const uint8_t maxAng = 170;  // reduced b/c the servo clicks & makes other weird noises
const uint8_t angRange = maxAng - minAng + 1;  // inclusive
const uint8_t middleAng = angRange / 2;  // truncated
const uint8_t initTwitchVal = 10;  // can calc this rather than hard code !!
uint8_t twitch = initTwitchVal, prevAng, newAng;
bool decapped = false;
const double vel = 60.0 / 250;  // degrees/millisecs
const float initAccel = 1.1;
float accel = initAccel;

// for mag connector
bool discontinuous;
#define contPin 1  // A7 == D1 - CPx; (digital input)

// board-controlled LED for quick functionin test
#define ledBtnPin 4  // btn A - CPx
#define boardLedPin 13  // D13 - CPx

// board controlled switch (not between power & board)
bool switchOn;
#define switchPin 7  // D7 - CPx


//// FUNCTIONS

// return angle difference
inline
uint8_t angDiffFunc() {
    int16_t angDiff = newAng - prevAng;
    return angDiff >= 0 ? angDiff : abs(angDiff);  // don't calc w/in abs()
}

// set new timer val & write new angle
inline
void newAngTimer() {
    myServo.write(newAng);
    timer = curMillis;
}

// reset if accidental death
inline
void easterBunny() {
    twitch = initTwitchVal;
    decapped = false;
    accel = initAccel;
    prevAng = newAng;
}

// used while continuity through mag connector (or other method of continuity)
inline
void contFunc() {
    if(twitch != initTwitchVal) easterBunny();
    newAng = random(minAng, maxAng);
    delayMS = angDiffFunc() / vel + 1;
    prevAng = newAng;
    newAngTimer();
}

// used while NO continuity through mag connector (or other method of continuity); return to middle angle
inline
void discontFunc() {
    // rev depending on motor orientation w/model
    if(!decapped) {  // could add code for if position greater than 3/4 at start of this block !!
        delay(50);
        newAng = middleAng * 7 / 4;  // change to 1/4 if oriented in other direction; imagery = should curl up before loosening
            // Serial.print("newAng when decapped < 1: ");
            // Serial.println(newAng);
        delayMS = angDiffFunc() / vel + 251;
        decapped = true;
    } else {
        if(middleAng < newAng && accel < middleAng) {
            newAng -= accel;
            accel *= accel;
        } else {
            newAng = middleAng;
        }
        delayMS = 65;
            // Serial.print("newAng during drop: ");
            // Serial.println(newAng);
            // Serial.println(accel);
    }
    newAngTimer();
    twitch--;
}
