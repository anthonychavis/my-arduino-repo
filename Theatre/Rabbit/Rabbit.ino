/*
NOTE:
    - CPx refers to pins of the Circuit Playground Express
    - QT = QT PY ESP32 Pico
    - might have to change value for other boards
*/


#include "Rabbit_Res.hpp"

//// put your setup code here, to run once:
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    // CPx
    #if defined(__SAMD21G18A__)
        // if button-controlled led for quick functioning test
        pinMode(boardLedPin, OUTPUT);  // for board-controlled LED
        pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
        // board controlled switch (not between power & board)
        pinMode(switchPin, INPUT_PULLUP);  // resistor needed for CPx onboard switch; 
    #endif

    // for mag connector/continuity
    pinMode(contPin, INPUT_PULLUP);  // uses onboard resistor; 

    // servo
    myServo.attach(servoPin);
    myServo.write(middleAng);
    prevAng = middleAng;
    delay(1000);

                // #if defined(ARDUINO_SAMD_ZERO)
                //   Serial.println("bingo");
                // #endif

    // #if defined(ARDUINO_ARCH_ESP32)
    //     Serial.println("ESP32");
    // #elif defined(ATSAMD21G18A)
    //     Serial.println("probably be CPX");
    // #elif defined(__ATSAMD21G18A__)
    //     Serial.println("probably be CPX");
    // #elif defined(CP_Boards)
    //     Serial.println("probably be CPX");
    // // #elif defined(ARDUINO_ARCH_SAMD)
    // //     Serial.println("might be CPX");
    // #elif defined(__SAMD21G18A__)
    //     Serial.println("totally be CPX");
    // #else
    //     Serial.println("not ESP32");
    // #endif
}

//// put your main code here, to run repeatedly:
void loop() {
    // CPx
    #if defined(__SAMD21G18A__)
        // (quick functioning test)
        digitalWrite(boardLedPin, digitalRead(ledBtnPin));
        // for board-controlled switch (not between power & board)
        switchOn = digitalRead(switchPin);
        if(!switchOn) {
            delay(500);
            return;
        }
    #endif
    // digitalWrite(PIN_NEOPIXEL, digitalRead(ledBtnPin));  // QT

    if(myServo.attached()) {
        curMillis = millis();
        discontinuous = digitalRead(contPin);  // HIGH & True == 1
        if(curMillis - timer > delayMS) !discontinuous ? contFunc() : discontFunc();
    } else {
        delay(500);
        return;
    }

    if(!twitch) myServo.detach();  // remove accidental jerky movement of servo
}
