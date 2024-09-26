/*
implement:
- smoother end
    - how much would have to change to implement 9.8m/s^2 ??
- improve/use twitch state for accidental dis-/reconnect
- refactor
*/


/*
TEST SPEED w/inline vs w/o inline
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

#include <Servo.h> // look more into this ?? #define MAX_SERVOS 0 ??

// VARS

// time
unsigned long curMillis, timer = 0;
bool decapped = false;
uint16_t delayMS;

// for servo
Servo myServo;
#define servoPin 10  // A3 == D10 - CPE; pwm/~
// test higher minAng & lower maxAng first to check how much the gears amplify the angles !!
const uint8_t minAng = 10;  // if 0, change initial prevAng
const uint8_t maxAng = 170;  // reduced b/c the servo clicks & makes other weird noises
const uint8_t angRange = maxAng - minAng + 1;  // inclusive
const uint8_t middleAng = angRange / 2;  // truncated
const uint8_t initTwitchVal = 10;  // can calc this rather than hard code !!
uint8_t twitch = initTwitchVal, absAngDiff, prevAng = 0, newAng;
int16_t angDiff;
const double vel = 60.0 / 250;  // degrees/millisecs
const float initAccel = 1.1;
float accel = initAccel;

// for mag connector
bool discontinuous;
#define contPin 1  // A7 == D1 - CPE; (digital input)

// for onboard
#define ledBtnPin 4  // btn A - CPE
#define onboardLedPin 13  // - CPE
// #define bBtnPin 5  // btn B - CPE

//// FUNCTIONS

// 
inline
void potato() {
    newAng = random(minAng, maxAng);
    myServo.write(newAng);
    angDiff = newAng - prevAng;
    absAngDiff = angDiff >= 0 ? angDiff : abs(angDiff);  // don't calc w/in abs()
    delayMS = absAngDiff / vel + 1;
    prevAng = newAng;
    timer = curMillis;
}

// used at beginning of program & for testing
inline
void setupContFunc() {
    prevAng = minAng;
    myServo.write(prevAng);
    delay(200);
}
    
// used while continuity through mag connector (or other method of continuity)
inline
void contFunc() {
    if(twitch != initTwitchVal) {  // fix twitch
        twitch = initTwitchVal;
        decapped = false;
        accel = initAccel;
        prevAng = newAng;
    }
    if (!prevAng) setupContFunc();
    potato();
}

// used while NO continuity through mag connector (or other method of continuity); return to middle angle
inline
void discontFunc() {
    // rev depending on motor orientation w/model
    if(!decapped) {  // could add code for if position greater than 3/4 at start of this block !!
        delay(50);
            // Serial.print("prevAng @decap: ");
            // Serial.println(prevAng);
        newAng = middleAng * 7 / 4;  // change to 1/4 if oriented in other direction; imagery = should curl up before loosening
            // Serial.print("newAng when decapped < 1: ");
            // Serial.println(newAng);
        angDiff = newAng - prevAng;
        absAngDiff = angDiff >= 0 ? angDiff : abs(angDiff);  // don't calc w/in abs()
        delayMS = absAngDiff / vel + 251;
        decapped = true;
    } else {
        if(middleAng < newAng && accel < middleAng) {
            // if(accel < middleAng) {
            newAng -= accel;
            accel *= accel;
        } else {
            newAng = middleAng;
        }
        // }
        delayMS = 65;
            // Serial.print("newAng during drop: ");
            // Serial.println(newAng);
            // Serial.println(accel);
    }
    myServo.write(newAng);
    timer = curMillis;
    twitch--;  // fix this !!
}
