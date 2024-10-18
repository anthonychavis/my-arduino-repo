/*
NOTE:
    - CPx refers to pins of the Circuit Playground Express
    - QT refers to the QT PY Pico
    - might have to change pin value for other boards
    - functions are not hoisted in this file
    - servo power input - 5v
*/

#ifndef RABBIT_RES_H

#define RABBIT_RES_H

#if defined(ARDUINO_ARCH_ESP32)
    #include <ESP32Servo.h>
#else
    #include <Servo.h>
#endif

//// VARS

#if defined(__SAMD21G18A__)  // CPx
    #define issa_CPx true
    // board-controlled LED for quick functionin test
    #define ledBtnPin 4  // btn A - CPx
    #define boardLedPin 13  // D13 - CPx
    // board controlled switch (not between power & board)
    bool switchOn;
    #define switchPin 7  // D7 - CPx
    // for servo
    #define servoPin 10  // A3 == D10 - CPx
    // for mag connector/continuity
    #define contPin 1  // A7 == D1 - CPx;
#else
    #define issa_CPx false
/*
* for QT PY ESP32 Pico (until more specific, if possible);
* change these pin values as needed for other boards
*/
    // for servo
    #define servoPin 26  // A0 == D26 - QT (unless using wifi?) ??; pwm/~;
    // for mag connector/continuity
    #define contPin 15  // A3 == D15 - QT  (unless using wifi?) ??; digital input
#endif

// time
unsigned long curMillis, timer = 0;
uint16_t delayMS;

// for servo - 5v [-- move to setup() in .ino !!]
Servo myServo;

// test higher minAng & lower maxAng first to check how much the gears amplify the angles !!
const uint8_t minAng = 10;  // increased b/c the servo clicks & makes other weird noises
const uint8_t maxAng = 170;  // reduced b/c the servo clicks & makes other weird noises
const uint8_t angRange = maxAng - minAng + 1;  // inclusive
const uint8_t middleAng = angRange / 2;  // truncated
uint8_t prevAng, newAng;
const double vel = 60.0 / 250;  // degrees/millisecs

// dead
const uint16_t initTwitchVal = 1000;  // change value if more time needed for accidental death; min = 0; max = 2^16 [need to code in the limit]; can calc this rather than hard code !!
uint16_t twitch = initTwitchVal;
bool decapped = false;
const float initAccel = 1.1;
float accel = initAccel;
// for mag connector
bool discontinuous;


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

// reset if accidental death; while twitch != 0
inline
void easterBunny() {
    twitch = initTwitchVal;
    decapped = false;
    accel = initAccel;
    prevAng = newAng;
}

// alive; used while continuity through mag connector (or other method of continuity)
inline
void contFunc() {
    if(twitch != initTwitchVal) {
        easterBunny();
    }
    newAng = random(minAng, maxAng);
    delayMS = angDiffFunc() / vel + 1;
    prevAng = newAng;
    newAngTimer();
}

// dead; used while NO continuity through mag connector (or other method of continuity); return to middle angle
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
    } else if(newAng == middleAng) {
        // this block extends the accidental death allowance - see initTwitchVal
        twitch--;
        delayMS = 300;
        timer = curMillis;
        return;
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
}

#endif
