/*
implement:
- go from const pins to #define pins
- end neutral
*/


/*
TEST SPEED W/CHRONO ON w/inline vs w/o inline
*/

/*
abs(x);
    - do not calculate w/in the method
    - https://www.arduino.cc/reference/en/language/functions/math/abs/
*/

/*
random(max);
random(min, max);
    - if need a sequence of nums to vary on subsequent executions of a sketch, use randomSeed() w/fairly random input, such as analogRead(), on an unconnected pin
    - https://www.arduino.cc/reference/en/language/functions/random-numbers/random/
    - https://www.arduino.cc/reference/en/language/functions/random-numbers/randomseed/
*/

#include <Servo.h> // look more into this ?? #define MAX_SERVOS 0??

// VARS

// time
unsigned long curMillis, timer = 0;
// unsigned long counter = 0;
uint16_t delayMS = 500;

// for servo
Servo myServo;
const uint8_t servoPin = 10;  // A3 == D10
const uint8_t minAng = 5;  // if 0, change initial prevAng
const uint8_t maxAng = 165;  // reduced b/c the servo clicks & makes other weird noises
const uint8_t angRange = maxAng - minAng + 1;  // inclusive
const uint8_t middleAng = angRange / 2;  // truncated

const uint8_t initTwitchVal = 30;
uint8_t twitch = initTwitchVal, absAngDiff, prevAng = 0, newAng;
int16_t angDiff;
const double vel = 60.0 / 250;  // degrees/millisecs

// for mag connector
bool discontinuous;
const uint8_t contPin = 1;  // A7 == D1 - CPE

// for onboard
const uint8_t ledBtnPin = 4;  // btn A - CPE
const uint8_t onboardLedPin = 13;  // - CPE
// const uint8_t bBtnPin = 5; // btn B - CPE

//// FUNCTIONS

// 
inline
void potato() {
    newAng = random(minAng, maxAng);
    // newAng = counter % 2 ? minAng : maxAng;  // used for quick test
    angDiff = newAng - prevAng;
    absAngDiff = angDiff >= 0 ? angDiff : abs(angDiff);  // don't calc w/in abs()
    delayMS = absAngDiff / vel + 1;
    myServo.write(newAng);
    prevAng = newAng;
    timer = curMillis;
}

// used at beginning of program & for testing
inline
void setupContFunc() {
    prevAng = minAng;
    myServo.write(prevAng);
    delay(100);
}
    
// used while continuity through mag connector (or other method of continuity)
inline
void contFunc() {
    if(twitch != initTwitchVal) twitch = initTwitchVal;
    if (!prevAng) setupContFunc();  
    potato();
}

// used while NO continuity through mag connector (or other method of continuity)
inline
void discontFunc() {
    // if (!prevAng) setupContFunc();  // used here while testing
        // Serial.println(twitch);
    potato();

    twitch--;
    // counter++;
}
