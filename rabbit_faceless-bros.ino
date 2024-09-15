////// currently built for use w/Circuit Playground Express
    // planning to add:
        // and other changes for bigger movements (if servo capable)
        // look into random(); see bottom of file

#include <Adafruit_Circuit_Playground.h>

//// put your setup code here, to run once:
void setup() {
    // Serial.begin(9600);
    // delay(1000);
    CircuitPlayground.begin();
    myServo.attach(servoPin);
    pinMode(onboardLedPin, OUTPUT);  // just for onboard red LED
    pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
    pinMode(contPin, INPUT_PULLUP);
}

//// put your main code here, to run repeatedly:
void loop() {
    // (quick functioning test) turn on D13 LED when D4 btn is pressed; will 1st read the status of the LED 
    digitalWrite(onboardLedPin, digitalRead(ledBtnPin));
    if(!CircuitPlayground.slideSwitch()) return;
    curMillis = millis();
    discontinuous = digitalRead(contPin);  // HIGH & True == 1; (so probably) LOW & False == 0 --> so should be int, not bool? ????
    if(twitch && curMillis - timer > delayMS) !discontinuous ? contFunc(item) : discontFunc(item);  // if accidental [continuity] disconnect then reconnect, it will still fxn as long as death twitch state not complete/expired
}

// look into using random(min, max);
  // https://www.arduino.cc/reference/en/language/functions/random-numbers/random/