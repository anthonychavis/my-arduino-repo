/*
NOTE:
    - CPx = Circuit Playground Express
*/

#include "Rabbit_Res.hpp"

//// put your setup code here, to run once:
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    // for quick board functioning test
    pinMode(onboardLedPin, OUTPUT);  // just for onboard red LED
    pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)

    // board controlled switch (not between power & board)
    pinMode(switchPin, INPUT_PULLUP);  // resistor needed for CPx onboard switch; 

    // continuity
    pinMode(contPin, INPUT_PULLUP);  // uses onboard resistor; 

    // servo
    myServo.attach(servoPin);
    if(myServo.attached()) {
        myServo.write(middleAng);
        prevAng = middleAng;
    }
}

//// put your main code here, to run repeatedly:
void loop() {
    // (quick functioning test)
    digitalWrite(onboardLedPin, digitalRead(ledBtnPin));

    // for board controlled switch (not between power & board)
    switchOn = digitalRead(switchPin);
    if(!switchOn) {
        delay(500);
        return;
    }

    if(myServo.attached()) {
        curMillis = millis();
        discontinuous = digitalRead(contPin);  // HIGH & True == 1
        if(twitch && curMillis - timer > delayMS) !discontinuous ? contFunc() : discontFunc();  // if accidental [continuity] disconnect then reconnect, it will still fxn as long as death twitch state not complete/expired
    } else {
        delay(500);
        return;
    }

    if(!twitch) myServo.detach();  // remove accidental movement of servo
}
