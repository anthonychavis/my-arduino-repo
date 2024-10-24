/*
NOTE:
    - Must find the servo's min/max pulse width (microseconds)
    - CPx refers to pins of the Circuit Playground Express
    - QT refers to the QT PY Pico
    - might have to change pin value for other boards
*/

#if defined(ARDUINO_ARCH_ESP32)
    #include <ESP32Servo.h>
#else
    #include <Servo.h>
#endif

#include "RabbitClass_Res.hpp"
// #include "Rabbit_Res.hpp"

        // pass by reference ?? Passing the dependency by reference ensures that you're working with the same instance of the object throughout the lifetime of th object. ??
        // should these be instantiated here ??
            // how are they removed from the stack if here ??

/*
- test servo min/max pulse width before running Rabbit
- set TestingMyServo to true when testing Servo; false otherwise
*/
#define TestingMyServo true

// Servo object instantiated from a Servo library
Servo myServo;

/*
(explicit constructor call);
param1: rabbit feet are towards the higher angles?;
param2: Servo object;
param3: mimimum angle in range;
param4: maximum angle in range;
*/
Rabbit fluffyCute(true, myServo, 0, 180);

//// put your setup code here, to run once:
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    #ifdef issa_CPx
        // for quick functioning test
        pinMode(boardLedPin, OUTPUT);  // for board-controlled LED
        pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
        // board controlled switch (not between power & board)
        pinMode(switchPin, INPUT_PULLUP);  // resistor needed for CPx onboard switch; 
    #endif

    // for mag connector/continuity
    pinMode(contPin, INPUT_PULLUP);  // uses onboard resistor;

    delay(1000);
}

//// put your main code here, to run repeatedly:
void loop() {
    #ifdef issa_CPx
        // (quick functioning test)
        digitalWrite(boardLedPin, digitalRead(ledBtnPin));
        // for board-controlled switch (not between power & board)
        switchOn = digitalRead(switchPin);
        if(!switchOn) {
            delay(1000);
            return;
        }
    #endif

    if(TestingMyServo) {
        // used to help find the min pulse width; adjust MIN_PULSE_WIDTH value in .hpp file
        printServoPos(myServo, 0);
        // used to help find the max pulse width; adjust MAX_PULSE_WIDTH value in .hpp file
        printServoPos(myServo, 180);
        return;
    }

    if(!fluffyCute.servoAttached()) {
        delay(500);
        return;
    }

    curMillis = millis();
    if(curMillis - timer > delayMS) {
        discontinuous = digitalRead(contPin);  // HIGH & True == 1;
        !discontinuous ? fluffyCute.struggle() : fluffyCute.headless();
    }

    if(!fluffyCute.revivable()) {
        fluffyCute.servoDetach();
    }
}
