#include <Adafruit_Circuit_Playground.h>  // remove this !!
#include "Rabbit_Res.hpp"

//// put your setup code here, to run once:
void setup() {
    // testing
    Serial.begin(9600);
    delay(1000);

    // servo
    myServo.attach(servoPin);
    myServo.write(middleAng);

    // continuity
    pinMode(contPin, INPUT_PULLUP);

    // board
    CircuitPlayground.begin();  // - CPE
    pinMode(onboardLedPin, OUTPUT);  // just for onboard red LED
    pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
}

//// put your main code here, to run repeatedly:
void loop() {
    digitalWrite(onboardLedPin, digitalRead(ledBtnPin));  // (quick functioning test) turn on D13 LED when D4 btn is pressed; will 1st read the status of the LED 

    if(!CircuitPlayground.slideSwitch()) return;  // - CPE

    curMillis = millis();
    discontinuous = digitalRead(contPin);  // HIGH & True == 1
    if(twitch && curMillis - timer > delayMS) !discontinuous ? contFunc() : discontFunc();  // if accidental [continuity] disconnect then reconnect, it will still fxn as long as death twitch state not complete/expired
}
