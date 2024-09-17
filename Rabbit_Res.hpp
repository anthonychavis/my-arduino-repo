/*
implement:
- go from const pins to #define pins
*/


/*
TEST SPEED W/CHRONO ON w/inline vs w/o inline
    - look into using arduino ide to print the time
    - https://www.arduino.cc/reference/en/language/functions/communication/print/
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

#include <Servo.h> // look more into this ??

// VARS

// time
unsigned long curMillis, timer = 0;
uint16_t delayMS = 500;

// for servo
Servo myServo;
const uint8_t servoPin = 10;  // A3 == D10
const uint8_t minAngle = 1;
const uint8_t maxAngle = 170;  // reduced b/c the servo clicks & makes other weird noises
// const uint8_t angleRange = maxAngle - minAngle;
// const uint8_t middleAngle = angleRange / 2;

const uint8_t initTwitchVal = 30;
uint8_t twitch = initTwitchVal, prevAngle = 0, angleDiff, newAngle;  // used for randomness & speed along w/delayMS

// const double maxVelocity = 60 / 250;  // degrees/millisecs for servo; check if smaller version of double (like uint8_t) !!
const double velocity = 5 / 250;  // degrees/millisecs


// for mag connector
bool discontinuous;
const uint8_t contPin = 1;  // A7 == D1


// for onboard
const uint8_t ledBtnPin = 4;
const uint8_t onboardLedPin = 13;
// const uint8_t twitchRstBtnPin = ;


//// FUNCTIONS
    
// used while continuity through mag connector (or other method of continuity)
// inline
// void contFunc(uint8_t itemState) {
inline
void contFunc() {
    if(twitch != initTwitchVal) twitch = initTwitchVal;

    if (!prevAngle) {
        prevAngle = minAngle;
        myServo.write(prevAngle);
        delay(100);
    }
    Serial.prinln(prevAngle);
    delay(50);
    newAngle = random(minAngle, maxAngle);
    Serial.println(newAngle);
    delay(50);
    angleDiff = newAngle - prevAngle;
    angleDiff = abs(angleDiff);  // DOES NOT WORK AS EXPECTED;
    Serial.printIn(angleDiff);
    delay(50);

    // delayMS = 500;
    // delayMS = angleDiff / maxVelocity;
    // delayMS = angleDiff / velocity;
    // delayMS += angleDiff / velocity;
    // delayMS = angleDiff / velocity + 500;
    Serial.println(delayMS);
    delay(50);

    myServo.write(newAngle);
    prevAngle = newAngle;
}

// used while NO continuity through mag connector (or other method of continuity)
// inline
// void discontFunc(uint8_t itemState) {
inline
void discontFunc() {
    if (!prevAngle) {
        prevAngle = minAngle;
        myServo.write(prevAngle);
        delay(100);
    }
    Serial.println(prevAngle);
    delay(50);
    newAngle = random(minAngle, maxAngle);
    Serial.println(newAngle);
    delay(50);
    angleDiff = newAngle - prevAngle;
    angleDiff = abs(angleDiff);  // DOESNT WORK AS EXPECTED; do not calc w/in this method - see Arduino docs
    Serial.printIn(angleDiff);
    delay(50);

    // delayMS = 500;
    // delayMS = angleDiff / maxVelocity;
    // delayMS = angleDiff / velocity;
    // delayMS += angleDiff / velocity;
    // delayMS = angleDiff / velocity + 500;
    Serial.println(delayMS);
    delay(50);

    myServo.write(newAngle);
    prevAngle = newAngle;
    twitch--;
}




// uint8_t item = 0;


    // switch (itemState) {
    //     case 0:
    //         contCaseBlock(minAngle, 1);
    //         break;
    //     case 1:
    //         contCaseBlock(middleAngle, 2);
    //         break;
    //     case 2:
    //         contCaseBlock(maxAngle, 3);
    //         break;
    //     default:
    //         contCaseBlock(middleAngle, 0);
    //         break;
    // }



    // Serial.println(newAngle);

    // switch (itemState) {
    //     case 0:
    //         contCaseBlock(minAngle, 1);
    //         break;
    //     case 1:
    //         contCaseBlock(maxAngle, 0);
    //         break;
    //     default:
    //         item = 0;
    //         break;
    // }


    // used w/in contFunc & discontFunc
// inline
// void contCaseBlock(uint8_t angle, uint8_t newItemVal) {
//     myServo.write(angle);
//     timer = curMillis;
//     item = newItemVal;
// }