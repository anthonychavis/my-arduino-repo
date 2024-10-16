// ready for test with other board & model
// NOTE: CPx refers to pins of the Circuit Playground Express; might have to change value for other boards

#include "Rabbit_Res.hpp"

//// put your setup code here, to run once:
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    // // if button-controlled led for quick functioning test
    // pinMode(boardLedPin, OUTPUT);  // for board-controlled LED
    // pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)

    // // board controlled switch (not between power & board)
    // pinMode(switchPin, INPUT_PULLUP);  // resistor needed for CPx onboard switch; 

    // for mag connector/continuity
    pinMode(contPin, INPUT_PULLUP);  // uses onboard resistor; 

    // servo
    myServo.attach(servoPin);
    myServo.write(middleAng);
    prevAng = middleAng;
}

//// put your main code here, to run repeatedly:
void loop() {
    // // (quick functioning test)
    // digitalWrite(boardLedPin, digitalRead(ledBtnPin));

    // // for board-controlled switch (not between power & board)
    // switchOn = digitalRead(switchPin);
    // if(!switchOn) {
    //     delay(500);
    //     return;
    // }

    if(myServo.attached()) {
        curMillis = millis();
        discontinuous = digitalRead(contPin);  // HIGH & True == 1
        if(curMillis - timer > delayMS) !discontinuous ? contFunc() : discontFunc();  // if accidental [continuity] disconnect then reconnect, it will still fxn as long as death twitch state not complete/expired
    } else {
        delay(500);
        return;
    }

    if(!twitch) myServo.detach();  // remove accidental jerky movement of servo
}
