// TEST SPEED W/CHRONO ON w/inline vs w/o inline

#include <Servo.h> // look more into this ??

// VARIABLES

// time
unsigned long curMillis, timer = 0;
uint16_t delayMS = 500;

// for servo
Servo myServo;
const uint8_t servoPin = 10;  // A3 == D10
const uint8_t minAngle = 1;
const uint8_t maxAngle = 170;  // reduced b/c the servo clicks & makes other weird noises
const uint8_t angleRange = maxAngle - minAngle;
const uint8_t middleAngle = angleRange / 2;
const uint8_t initTwitchVal = 8;
uint8_t twitch = initTwitchVal, item = 0;

// for mag connector
bool discontinuous;
const uint8_t contPin = 1;  // A7 == D1

// for onboard LED
const uint8_t ledBtnPin = 4;
const uint8_t onboardLedPin = 13;


//// FUNCTIONS
    // if accidental [continuity] disconnect then reconnect, it will still function as long as death twitch state not complete/expired
    
// used while continuity through mag connector (or other method of continuity)
inline
void contFunc(uint8_t itemState) {
    if(twitch != initTwitchVal) twitch = initTwitchVal;
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
inline
void discontFunc(uint8_t itemState) {
    twitch--;
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
inline
void contCaseBlock(uint8_t angle, uint8_t newItemVal) {
    myServo.write(angle);
    timer = curMillis;
    item = newItemVal;
}