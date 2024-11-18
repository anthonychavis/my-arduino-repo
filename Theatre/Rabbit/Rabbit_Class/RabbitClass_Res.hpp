#ifndef RABBIT_RES_H
#define RABBIT_RES_H

/*
NOTE:
    - Must find the servo's min/max pulse width (microseconds)
    - CPx refers to the Circuit Playground Express - confirmed still works
    - QT refers to the QT PY Pico
    - might have to change pin value for other boards
*/

#include <memory>

//// VARS

#ifndef servoPin
#define servoPin
#endif

#ifndef contPin
#define contPin
#endif

#ifndef lowestPulseWidth
#define lowestPulseWidth
#endif

#ifndef highestPulseWidth
#define highestPulseWidth
#endif

// time
struct TimeStruct {
    unsigned long curMillis = 0;
    unsigned long delayMS = 0;
};

// // servo angle range limits
// struct AngRangeLims {
//     uint8_t minAng = 0;
//     uint8_t maxAng = 180;
// };


//// CLASSES 

class Rabbit {
    Servo& servo;
    TimeStruct& rabTime;

    int16_t minAng = -1, maxAng = -1, angRange = -1, middleAng = -1;
    int16_t prevAng = -1, newAng = -1;
    const double vel = 60.0 / 250;  // degrees/millisecs

    // dead
    const uint16_t initSurvivability = 10;  // min = 0; max = 65536
    uint16_t survivability = initSurvivability;
    bool decapped = false, feetAtMaxAng;
    const float initAccel = 1.1;
    float accel = initAccel;

    // METHODS

    // return angle difference
    uint8_t angDiffFunc() {
        int16_t angDiff = newAng - prevAng;
        return angDiff >= 0 ? angDiff : abs(angDiff);  // don't calc w/in abs()
    }

    // could further improve for maxAng, minAng, & initAccel variance !!
    // for headless(); accel to middleAng
    void decapAccelNewAng() {
        if(accel > middleAng) {
            newAng = middleAng;
            return;
        }

        if(feetAtMaxAng) {
            newAng -= accel;
            if(middleAng > newAng) {
                newAng = middleAng;
                return;
            }
        } else {
            newAng += accel + 1;
            if(middleAng < newAng) {
                newAng = middleAng;
                return;
            }
        }

        accel *= accel;
    }

    // for headless()
    void decreaseSurvivability() {
        // extends the accidental death allowance - see initSurvivability
        survivability--;
        rabTime.delayMS = 300 + rabTime.curMillis;
        Serial.print("survivability level: ");Serial.println(survivability);
    }

    // reset if accidental death
    void easterBunny() {
        survivability = initSurvivability;
        decapped = false;
        accel = initAccel;
        prevAng = newAng;
    }

    // for headless()
    void initDecap() {
        newAng = feetAtMaxAng ? maxAng - angRange / 10 : minAng + angRange / 10;
        rabTime.delayMS = angDiffFunc() / vel + 251;
        decapped = true;
    }

    // set new delayMS val & write new angle
    void newAngDelay() {
        servo.write(newAng);
        rabTime.delayMS += rabTime.curMillis;
    }

    // for troubleshooting
    void printAllAngs() {
        Serial.print("minAng: ");Serial.println(minAng);
        Serial.print("maxAng: ");Serial.println(maxAng);
        Serial.print("angRange: ");Serial.println(angRange);
        Serial.print("middleAng: ");Serial.println(middleAng);
        Serial.print("prevAng: ");Serial.println(prevAng);
        if(newAng < 0) return;
        Serial.print("newAng: ");Serial.println(newAng);
    }

    int16_t setMidAng() {
        if(angRange < 0) return -1;
        return angRange >> 1 + minAng;  // truncated
    }  // might need a neutralAng depending on how it fxns w/model

    explicit Rabbit(const bool feetTowardsHighAng, Servo& aServo, TimeStruct& timeData, const uint8_t lowAng = 10, const uint8_t highAng = 170) :
        feetAtMaxAng(feetTowardsHighAng), servo(aServo), rabTime(timeData), minAng(lowAng), maxAng(highAng)
    {
            angRange = maxAng - minAng + 1;
            middleAng = setMidAng();
            servo.attach(servoPin, lowestPulseWidth, highestPulseWidth);
            servo.write(middleAng);
            prevAng = middleAng;
    }

public:
    ~Rabbit() {
        servo.detach();
        Serial.println("Servo detachment initiated");
    }

    static std::unique_ptr<Rabbit> create(const bool feetTowardsHighAng, Servo& aServo, TimeStruct& timeData, const int16_t lowAng = 10, const int16_t highAng = 170) {
        // improve conditional to check spacing !!
        if(lowAng < 0 || highAng > 180 || highAng < lowAng) {
            Serial.println("CANNOT create this rabbit - create()");
            Serial.println("check lowAng & highAng; adhere to 0 <= lowAng < highAng <= 180");
            return nullptr;
        }
        
        auto rabbit = std::unique_ptr<Rabbit>(new Rabbit(feetTowardsHighAng, aServo, timeData, lowAng, highAng));

        // in case i move things around too quickly again
        if(rabbit->middleAng < 0) {
            Serial.println("CANNOT create this rabbit - create()");
            Serial.println("Check that the angRange is initiated prior to the initiation of middleAng.");
            rabbit->printAllAngs();
            return nullptr;
        }

        Serial.println("The rabbit is born! - create()");
        rabbit->printAllAngs();
        return rabbit;
    }

    // dead; used while NO continuity through mag connector (or other method of continuity); position ends at middleAng
    void headless() {
        // imagery = [should curl up]; a final movement (as if signal from brain already sent to muscles) w/dramatic pause before loosening
        if(!decapped) {
            delay(50);
            initDecap();
        } else if(newAng == middleAng) {
            decreaseSurvivability();
            return;
        } else {
            decapAccelNewAng();
            rabTime.delayMS = 65;
        }
        newAngDelay();
    }

    // alive; used while continuity through mag connector (or other method of continuity)
    void struggle() {
        if(survivability != initSurvivability) {
            easterBunny();
        }
        newAng = random(minAng, maxAng);
        rabTime.delayMS = angDiffFunc() / vel + 1;
        prevAng = newAng;
        newAngDelay();
    }

    // GETTERS

    // fix how it repeats the values from 1st execution w/in loop() !!
    void getAllAngs() {
        printAllAngs();
    }

    int16_t getMiddleAng() {
        return middleAng;
    }

    uint16_t revivable() {
        // need to add check in case type is changed again !!
        return survivability;
    }

    bool servoAttached() {
        return servo.attached();
    }

    // FOR TESTING

    // // serial print servo position in microseconds
    // void printServoPos() {
    //     int microsec = servo.readMicroseconds();
    //     int ang = servo.read();
    //     Serial.println("---");
    //     Serial.print("Servo position in microseconds: ");
    //     Serial.println(microsec);
    //     Serial.print("Servo position in degrees: ");
    //     Serial.println(ang);
    //     delay(2000);
    // }
    // ensure no other function controls the servo before testing w/this function; serial print servo position in microseconds at specified angle
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
    //     Serial.print("Servo position in degrees: ");
    //     Serial.println(ang);
    //     delay(2000);
    // }
};

#endif
