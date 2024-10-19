/*
NOTE:
    - CPx refers to pins of the Circuit Playground Express
    - QT refers to the QT PY Pico
    - might have to change pin value for other boards
*/

#if defined(ARDUINO_ARCH_ESP32)
    #include <ESP32Servo.h>
#else
    #include <Servo.h>
#endif

// #include "RabbitClass_Res.hpp"
#include "Rabbit_Res.hpp"

        // pass by reference ?? Passing the dependency by reference ensures that you're working with the same instance of the object throughout the lifetime of th object. ??
        // should these be instantiated here ??
            // how are they removed from the stack if here ??

// Servo object instantiated from a Servo library
Servo myServo;
/*
(explicit constructor call);
param1: rabbit feet are towards the higher angles?;
param2: Servo object;
param3: mimimum angle in range;
param4: maximum angle in range;
*/
Rabbit fluffyCute(true, myServo, 20, 160);
    // delete the objects ?? in destructor ??

//// put your setup code here, to run once:
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    // CPx; needs work:
    if(issa_CPx) {
        // if button-controlled led for quick functioning test
        pinMode(boardLedPin, OUTPUT);  // for board-controlled LED
        pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
        // board controlled switch (not between power & board)
        pinMode(switchPin, INPUT_PULLUP);  // resistor needed for CPx onboard switch; 
    }

    // for mag connector/continuity
    pinMode(contPin, INPUT_PULLUP);  // uses onboard resistor;

// HOW TO CREATE RABBIT FOR GLOBAL ACCESS, BUT ATTACH PIN HERE ?? !!

    delay(1000);
}

//// put your main code here, to run repeatedly:
void loop() {
    // printServoPos(myServo);  // comment out or remove when not troubleshooting
    // printServoPos(myServo, 70);  // don't use while main code is active; comment out or remove when not troubleshooting
    // fluffyCute.printServoPos();  // comment out or remove when not troubleshooting
    // fluffyCute.printServoPos(50);  // don't use while main code is active; comment out or remove when not troubleshooting

    // CPx; needs work:
    if(issa_CPx) {
        // (quick functioning test)
        digitalWrite(boardLedPin, digitalRead(ledBtnPin));
        // for board-controlled switch (not between power & board)
        switchOn = digitalRead(switchPin);
        if(!switchOn) {
            delay(1000);
            return;
        }
    }

    if(fluffyCute.servoAttached()) {
        curMillis = millis();
        discontinuous = digitalRead(contPin);  // HIGH & True == 1; (0V input read at pin) LOW & False == 0
        if(curMillis - timer > delayMS) {
            !discontinuous ? fluffyCute.struggle() : fluffyCute.headless();
        }
    } else {
        delay(500);
        return;
    }

    if(!fluffyCute.revivable()) {
        fluffyCute.servoDetach();
    }

    // fluffyCute.printServoPos();  // comment out or remove when not troubleshooting
}
