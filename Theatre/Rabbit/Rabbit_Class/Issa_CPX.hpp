#ifndef ISSA_CPX
#define ISSA_CPX

/*
NOTE:
- CPx refers to the Circuit Playground Express
*/

// board-controlled LED for quick functioning test
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
inline
void cpxSetup() {
    // for quick functioning test
    pinMode(boardLedPin, OUTPUT);  // for board-controlled LED
    pinMode(ledBtnPin, INPUT_PULLDOWN);  // makes default status of the D4 btn = 0 (as opposed to truthy)
    // board controlled switch (not between power & board)
    pinMode(switchPin, INPUT_PULLUP);  // resistor needed for CPx onboard switch
}

// use within the loop() function
inline
void cpxLoop() {
    // quick functioning test
    digitalWrite(boardLedPin, digitalRead(ledBtnPin));
    // for board-controlled switch (not between power & board)
    switchOn = digitalRead(switchPin);
}

#endif
