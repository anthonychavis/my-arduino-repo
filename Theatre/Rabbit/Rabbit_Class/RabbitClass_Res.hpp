#ifndef RABBIT_RES_H

#define RABBIT_RES_H

/*
NOTE:
    - Must find the servo's min/max pulse width (microseconds)
    - CPx refers to pins of the Circuit Playground Express
    - QT refers to the QT PY Pico
    - might have to change pin value for other boards
*/

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
//     #define issa_CPx false
// /*
// * for QT PY ESP32 Pico (until more specific, if possible);
// * change these pin values as needed for other boards
// */
//     // for servo
//     #define servoPin 26  // A0 == D26 - QT (unless using wifi?) ??; pwm/~;
//     // for mag connector/continuity
//     #define contPin 15  // A3 == D15 - QT  (unless using wifi?) ??; digital input

/*
* for RP2040 Prop-Maker Feather (until more specific, if possible);
* change these pin values as needed
*/
    // for servo
    #define servoPin pinNumber
    // for mag connector/continuity
    #define contPin 15  // 
#endif

/*
for servo
- assign the tested min/max pulse widths respectively
- could start with the values given in the servo spec sheet
- default was 540 & 2400 for Servo.h
*/
#ifdef MIN_PULSE_WIDTH
#undef MIN_PULSE_WIDTH
#endif
#ifdef MAX_PULSE_WIDTH
#undef MAX_PULSE_WIDTH
#endif
#define MIN_PULSE_WIDTH 532
#define MAX_PULSE_WIDTH 2120

// time
unsigned long curMillis, timer = 0;
uint16_t delayMS;

// for mag connector
bool discontinuous;


//// FUNCTIONS

// serial print servoObj position in microseconds
void printServoPos(Servo servoObj) {
    int microsec = servoObj.readMicroseconds();
    int ang = servoObj.read();
    Serial.println("---");
    Serial.print("Servo position in microseconds: ");
    Serial.println(microsec);
    Serial.print("Servo position in angle: ");
    Serial.println(ang);
    delay(2000);
};
// don't use while main code is active; serial print servoObj position in microseconds at specified angle
void printServoPos(Servo servoObj, int angle) {
    if(angle > 180 || angle < 0) {
        Serial.print("adhere to 0 <= angle <= 180; you entered: ");
        Serial.println(angle);
        delay(2000);
        return;
    }
    servoObj.write(angle);
    int microsec = servoObj.readMicroseconds();
    int ang = servoObj.read();
    Serial.println("---");
    Serial.print("Servo position in microseconds: ");
    Serial.println(microsec);
    Serial.print("Servo position in angle: ");
    Serial.println(ang);
    delay(2000);
};


//// CLASSES 

class Rabbit {
    Servo& servo;

    uint8_t minAng, maxAng, angRange, middleAng;
    uint8_t prevAng, newAng;
    const double vel = 60.0 / 250;  // degrees/millisecs

    // dead
    const uint16_t initSurvivability = 100;  // min = 0; max = 65536
    uint16_t survivability = initSurvivability;
    bool decapped = false, feetAtMaxAng;
    const float initAccel = 1.1;
    float accel = initAccel;

    // METHODS

    uint8_t setMidAng() {
        return angRange / 2 + minAng;  // truncated
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

    // Test higher lowAng & lower highAng first to check how much the gears amplify the angles !!
    // negative values may result in unexpected behavior - for now
    explicit Rabbit(bool feetTowardsHighAng, Servo& aServo, uint8_t lowAng = 10, uint8_t highAng = 170) :
        feetAtMaxAng(feetTowardsHighAng), servo(aServo), minAng(lowAng), maxAng(highAng)
    {
            angRange = maxAng - middleAng + 1;
            middleAng = setMidAng();
                servo.attach(servoPin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
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
        if(!decapped) {  
            delay(50);
            // imagery = should curl up before loosening
            newAng = feetAtMaxAng ? maxAng - angRange / 10 : minAng + angRange / 10;
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
            if(accel < middleAng) {
                if(middleAng < newAng && feetAtMaxAng) {
                    newAng -= accel;
                } else if(middleAng > newAng && !feetAtMaxAng) {
                    newAng += accel;
                } else {
                    newAng = middleAng;
                }
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

    uint8_t getMinAng() {
        return minAng;
    }
    
    uint8_t getMiddleAng() {
        return middleAng;
    }

    uint8_t getMaxAng() {
        return maxAng;
    }

    // FOR TESTING

    // // serial print servo position in microseconds
    // void printServoPos() {
    //     int microsec = servo.readMicroseconds();
    //     int ang = servo.read();
    //     Serial.println("---");
    //     Serial.print("Servo position in microseconds: ");
    //     Serial.println(microsec);
    //     Serial.print("Servo position in angle: ");
    //     Serial.println(ang);
    //     delay(2000);
    // }
    // // don't use while main code is active; serial print servo position in microseconds at specified angle
    // void printServoPos(int angle) {
    //     if(angle > 180 || angle < 0) {
    //         Serial.print("adhere to 0 <= angle <= 180; you entered: ");
    //         Serial.println(angle);
    //         delay(2000);
    //         return;
    //     }
    //     servo.write(angle);
    //     int microsec = servo.readMicroseconds();
    //     int ang = servo.read();
    //     Serial.println("---");
    //     Serial.print("Servo position in microseconds: ");
    //     Serial.println(microsec);
    //     Serial.print("Servo position in angle: ");
    //     Serial.println(ang);
    //     delay(2000);
    // }
};

#endif
