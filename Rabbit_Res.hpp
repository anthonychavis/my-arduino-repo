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

#include <Servo.h> // look more into this ?? #define MAX_SERVOS 0??

// VARS

// time
unsigned long curMillis, timer = 0;
uint8_t counter = 0;
uint16_t delayMS = 500;

// for servo
Servo myServo;
#define servoPin 10  // A3 == D10 - CPE; pwm/~
// test higher minAng & lower maxAng first to check how much the gears amplify the angles !!
const uint8_t minAng = 10;  // if 0, change initial prevAng
const uint8_t maxAng = 170;  // reduced b/c the servo clicks & makes other weird noises
const uint8_t angRange = maxAng - minAng + 1;  // inclusive
const uint8_t middleAng = angRange / 2;  // truncated
const uint8_t initTwitchVal = 15;  // can calc this rather than hard code !!
uint8_t twitch = initTwitchVal, absAngDiff, prevAng = 0, newAng;
int16_t angDiff;
const double vel = 60.0 / 210;  // degrees/millisecs

// for mag connector
bool discontinuous;
#define contPin 1  // A7 == D1 - CPE

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
    if(twitch != initTwitchVal) {
        twitch = initTwitchVal;
        counter = 0;
    }
    if (!prevAng) setupContFunc();  
    potato();
}

// used while NO continuity through mag connector (or other method of continuity); return to middle angle
inline
void discontFunc() {
    if(!counter) {  // what if greater than 3/4 ??
        newAng = middleAng * 7 / 4;  // change to 1/4 if oriented in other direction; imagery = should curl up before loosening
        // Serial.print("newAng when counter < 1: ");
        // Serial.println(newAng);
        angDiff = newAng - prevAng;
        absAngDiff = angDiff >= 0 ? angDiff : abs(angDiff);  // don't calc w/in abs()
        delayMS = absAngDiff / vel + 1;
        counter++;
    } else {
        if(middleAng < newAng) {
            newAng -= 8;
            if(newAng < middleAng) newAng = middleAng;
            // Serial.print("newAng during drop: ");
            // Serial.println(newAng);
            delayMS = (newAng - middleAng) / vel;
        }
    }

    myServo.write(newAng);
    timer = curMillis;
    twitch--;
}
