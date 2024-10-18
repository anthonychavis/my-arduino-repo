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

// #include <optional>

//// VARS

// this conditional needs much improvement !!
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

// for mag connector
bool discontinuous;


//// CLASSES 

class Rabbit {
    Servo& servo;

    // alive
    uint8_t minAng, maxAng;
    uint8_t middleAng, prevAng, newAng;
    const double vel = 60.0 / 250;  // degrees/millisecs

    // dead
    const uint16_t initSurvivability = 100;  // min = 0; max = 65536
    uint16_t survivability = initSurvivability;
    bool decapped = false, feetAtBigAng;
    const float initAccel = 1.1;
    float accel = initAccel;

    // METHODS

    uint8_t getMidAng() {
        return (maxAng - minAng + 1) / 2;  // truncated
    }  // might need a neutralAng depending on how it fxns w/model

    // return angle difference
    uint8_t angDiffFunc() {
        int16_t angDiff = newAng - prevAng;
        return angDiff >= 0 ? angDiff : abs(angDiff);  // don't calc w/in abs()
    }

    // set new timer val & write new angle
    void newAngTimer() {
        servo.write(newAng);
        timer = curMillis;
    }

    // reset if accidental death
    void easterBunny() {
        survivability = initSurvivability;
        decapped = false;
        accel = initAccel;
        prevAng = newAng;
    }

public:
        // Passing the dependency by reference ensures that you're working with the same instance of the object throughout the lifetime of the object. ??
            // destructor not needed in this case b/c this class does not "own" the dependency; aka it was instantiated elsewhere
                // how to remove from stack if instantiated in global scope ??
    // use "explicit" to prevent implicit conversions ??
    
    // test higher minAng & lower maxAng first to check how much the gears amplify the angles !!
    explicit Rabbit(bool feetTowardsHighAng, Servo& aServo, uint8_t lowAng = 10, uint8_t highAng = 170) : feetAtBigAng(feetTowardsHighAng), servo(aServo), minAng(lowAng), maxAng(highAng) {
            middleAng = getMidAng();
                servo.attach(servoPin);
                servo.write(middleAng);
            prevAng = middleAng;
    }

    // static std::optional<Rabbit> create(Servo& aServo, bool feetTowardsHighAng, uint8_t lowAng = 10, uint8_t highAng = 170) {
    //     if(lowAng is <= 0 || highAng >= 180 || highAng < lowAng) {
    //         return std::nullopt;
    //     }
            // print values !!
    // }

    // alive; used while continuity through mag connector (or other method of continuity)
    void struggle() {
        if(survivability != initSurvivability) {
            easterBunny();
        }
        newAng = random(minAng, maxAng);
        delayMS = angDiffFunc() / vel + 1;
        prevAng = newAng;
        newAngTimer();
    }

    // dead; used while NO continuity through mag connector (or other method of continuity); ends at middleAng
    void headless() {
        // rev depending on motor orientation w/model
            // use feetAtBigAng for automatic switch
        if(!decapped) {  
            // could add code for if position greater than 3/4 at start of this block !!
            delay(50);
            newAng = middleAng * 7 / 4;  // change to 1/4 if oriented in other direction; imagery = should curl up before loosening
            // newAng = middleAng * (feetAtBigAng ? 7 : 1) / 4;  // check if this syntax works !!
                Serial.print("newAng when decapped < 1: ");
                Serial.println(newAng);
            delayMS = angDiffFunc() / vel + 251;
            decapped = true;
        } else if(newAng == middleAng) {
            // this block extends the accidental death allowance - see initSurvivability
            survivability--;
            delayMS = 300;
            timer = curMillis;
            return;
        } else {
            // use feetAtBigAng to alter this !!
            if(middleAng < newAng && accel < middleAng) {
                newAng -= accel;
                accel *= accel;
            } else {
                newAng = middleAng;
            }
            delayMS = 65;
                Serial.print("newAng during drop: ");
                Serial.println(newAng);
                Serial.println(accel);
        }
        newAngTimer();
    }

    // GETTERS

    bool servoAttached() {
        return servo.attached();
    }

    uint16_t revivable() {
        // need to add check in case type is changed again !!
        return survivability;
    }

    void servoDetach() {
        servo.detach();
    }
};

#endif
