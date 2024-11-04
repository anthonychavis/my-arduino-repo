/*
NOTE:
    - Must find the servo's min/max pulse width (microseconds)
    - CPx refers to the Circuit Playground Express - confirmed works w/9g servo
    - QT refers to the QT PY Pico
    - might have to change pin value for other boards
*/

#if defined(ARDUINO_ARCH_ESP32)
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

#if defined(__SAMD21G18A__)
#define issa_CPx
#endif

/*
for servo
- assign the tested min/max (lowest/highest) pulse widths respectively
- could start with the values given in the servo spec sheet
- default was 540 & 2400 for Servo.h
*/
#ifdef lowestPulseWidth
#undef lowestPulseWidth
#endif
#ifdef highestPulseWidth
#undef highestPulseWidth
#endif
#define lowestPulseWidth 532
#define highestPulseWidth 2120


#ifdef issa_CPx
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

    // use within the setup() function
    void cpxSetup() {
        // for quick functioning test
        pinMode(boardLedPin, OUTPUT);  // for board-controlled LED
        pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
        // board controlled switch (not between power & board)
        pinMode(switchPin, INPUT_PULLUP);  // resistor needed for CPx onboard switch
    }

    // use within the loop() function
    void cpxLoop() {
        // quick functioning test
        digitalWrite(boardLedPin, digitalRead(ledBtnPin));
        // for board-controlled switch (not between power & board)
        switchOn = digitalRead(switchPin);
    }
#else
    // for servo
    #define servoPin 1  // change as needed; A0 == D26 - QT (unless using wifi); pwm/~
    // for mag connector/continuity
    #define contPin 10  // change as needed; A3 == D15 - QT  (unless using wifi); digital input
#endif

/*
- test servo min/max pulse width before running Rabbit
- set TestingMyServo to true when testing Servo; false otherwise
*/
#define TestingMyServo true

// Servo object instantiated from a Servo library
Servo myServo;


//// TESTING SERVO 
#if TestingMyServo
// serial print servoObj position in microseconds
void printServoPos(Servo servoObj) {
    int microsec = servoObj.readMicroseconds();
    int ang = servoObj.read();
    Serial.println("---");
    Serial.print("Servo position in microseconds: "); Serial.println(microsec);
    Serial.print("Servo position in degrees: "); Serial.println(ang);
    delay(2000);
};
// don't use while main program is active; serial print servoObj position in microseconds at specified angle
void printServoPos(Servo servoObj, int angle) {
    if(angle > 180 || angle < 0) {
        Serial.print("adhere to 0 <= angle <= 180; you entered: "); Serial.println(angle);
        delay(2000);
        return;
    }
    servoObj.write(angle);
    int microsec = servoObj.readMicroseconds();
    int ang = servoObj.read();
    Serial.println("---");
    Serial.print("Servo position in microseconds: "); Serial.println(microsec);
    Serial.print("Servo position in degrees: "); Serial.println(ang);
    delay(2000);
};

void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    #ifdef issa_CPx
        cpxSetup();
    #endif

    myServo.attach(servoPin, lowestPulseWidth, highestPulseWidth);
    Serial.println("TestingMyServo; servoPin attachment initiated.");

    delay(1000);
}

void loop() {
    #ifdef issa_CPx
        cpxLoop();
        if(!switchOn) {
            delay(1000);
            return;
        }
    #endif

    // used to help find the min pulse width; adjust lowestPulseWidth value at above macro definition
    printServoPos(myServo, 0);
    // used to help find the max pulse width; adjust highestPulseWidth value at above macro definition
    printServoPos(myServo, 180);
}
#endif
//// end TESTING SERVO


//// NOT TESTING SERVO 
#if !(TestingMyServo)
#include "RabbitClass_Res.hpp"
// #include "Rabbit_Res.hpp"

// for mag connector/continuity
bool discontinuous;

// time
TimeStruct fluffyTime;

std::unique_ptr<Rabbit> fluffyCute = nullptr;

void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    #ifdef issa_CPx
        cpxSetup();
    #endif

    // for mag connector/continuity
    pinMode(contPin, INPUT_PULLUP);  // uses onboard resistor;

    /*
    param1: rabbit feet are towards the higher angles?;
    param2: Servo object;
    param3: TimeStruct var;
    param4: mimimum angle in range;
    param5: maximum angle in range;
    */
    fluffyCute = Rabbit::create(true, myServo, fluffyTime, 0, 180);
    // fluffyCute = Rabbit::create(true, myServo, fluffyTime, 150, 180);  // for testing
    // fluffyCute = Rabbit::create(true, myServo, fluffyTime, 0, 30);  // for testing

    if(fluffyCute) {
        Serial.println("The rabbit is born! - setup()");
    } else {
        Serial.println("fluffyCute doesn't exist");
    }

    delay(1000);
}

void loop() {
    #ifdef issa_CPx
        cpxLoop();
        if(!switchOn) {
            delay(1000);
            return;
        }
    #endif

    if(!fluffyCute) {
        Serial.println("The soul of the creature you seek cannot be detected.");
        delay(1000);
        return;
    }

    fluffyTime.curMillis = millis();
    if(fluffyTime.curMillis >= fluffyTime.delayMS) {
        discontinuous = digitalRead(contPin);  // HIGH & True == 1;
        !discontinuous ? fluffyCute->struggle() : fluffyCute->headless();
    }

    if(!fluffyCute->revivable()) {
        fluffyCute = nullptr;
    }
}
#endif
//// end NOT TESTING SERVO
