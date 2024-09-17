////// currently built for use w/Circuit Playground Express

#include <Adafruit_Circuit_Playground.h>
#include "Rabbit_Res.hpp"

//// put your setup code here, to run once:
void setup() {

    Serial.begin(9600);
    delay(1000);
    // randomSeed(analogRead(0));  // works as expected w/o this
    
    CircuitPlayground.begin();
    myServo.attach(servoPin);
    pinMode(onboardLedPin, OUTPUT);  // just for onboard red LED
    pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
    pinMode(contPin, INPUT_PULLUP);

    // instantiate pinMode for twitch reset
    // pinMode(twitchRstBtnPin, INPUT_PULLDOWN);
}

//// put your main code here, to run repeatedly:
void loop() {
    digitalWrite(onboardLedPin, digitalRead(ledBtnPin));  // (quick functioning test) turn on D13 LED when D4 btn is pressed; will 1st read the status of the LED 

    // use digitalWrite to reset twitch ?? --> twitch = initTwitchVal; (prolly not)
    // digitalWrite(twitch = initTwitchVal, digitalRead(twitchRstBtnPin));

    if(!CircuitPlayground.slideSwitch()) return;



    curMillis = millis();
    discontinuous = digitalRead(contPin);  // HIGH & True == 1 b/c stored as a bit (1 or 0)
    // if(twitch && curMillis - timer > delayMS) !discontinuous ? contFunc(item) : discontFunc(item);  // if accidental [continuity] disconnect then reconnect, it will still fxn as long as death twitch state not complete/expired
    if(twitch && curMillis - timer > delayMS) !discontinuous ? contFunc() : discontFunc();  // if accidental [continuity] disconnect then reconnect, it will still fxn as long as death twitch state not complete/expired
}
