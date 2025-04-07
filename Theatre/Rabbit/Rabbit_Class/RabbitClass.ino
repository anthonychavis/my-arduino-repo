/*
NOTE:
    - Must find the servo's min/max pulse width (microseconds) - see code below
    - CPX refers to the Circuit Playground Express - reconfirmed works with a 180 degree 9g servo
    - QT refers to the QT PY Pico
    - might have to change pin value for other boards
    - currently built for boards that support the C++ memory library
*/

#if defined(ARDUINO_ARCH_ESP32)
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

#if defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS)  // core
#include "Issa_CPX.hpp"  // defaults: servoPin 10; contPin 1
#else
// for servo
#define servoPin 1  // change as needed; A0 == D26 - QT (unless using wifi); pwm/~
// for mag connector/continuity
#define contPin 10  // change as needed; A3 == D15 - QT  (unless using wifi); digital input
#endif

/*
for servo
- assign the tested min/max (lowest/highest) pulse widths respectively
- could start with the values given in the servo spec sheet
- default was 540 & 2400 for Servo.h
*/
#define lowestPulseWidth 600
#define highestPulseWidth 2100
/*
- test servo min/max pulse width before running Rabbit
- set TestingMyServo to true when testing Servo; false otherwise
*/
#define TestingMyServo true

// Servo object instantiated from a Servo library
Servo myServo;


//// TESTING SERVO
#if TestingMyServo
#include "TestingServo.hpp"

//// setup code here, to run once
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    #ifdef ISSA_CPX
        cpxSetup();
    #endif

    myServo.attach(servoPin, lowestPulseWidth, highestPulseWidth);
    Serial.println("TestingMyServo; servoPin attachment initiated.");
    delay(1000);
}

//// main code here, to run repeatedly
void loop() {
    #ifdef ISSA_CPX
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
#include "TimeStruct.hpp"
#include "RabbitClass_Res.hpp"

// for mag connector/continuity
bool discontinuous = false;

TimeStruct fluffyTime;

std::unique_ptr<Rabbit> fluffyCute = nullptr;

//// setup code here, to run once
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    #ifdef ISSA_CPX
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

    if(fluffyCute) {
        Serial.println("The rabbit is born! - setup()");
    } else {
        Serial.println("fluffyCute doesn't exist");
    }
    delay(1000);
}

//// main code here, to run repeatedly
void loop() {
    #ifdef ISSA_CPX
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
