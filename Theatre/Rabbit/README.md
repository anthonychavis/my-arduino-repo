# Sacrificial Rabbit

A "tail" (_or...fin?_) of a rabbit (_or...shark?_) that wouldn't.[^1]

A rabbit, held with its spine vertical, struggles to free itself from an actor's grip for some time until it is decapitated.

## Table of Contents

- [Code][Rabbit_Class] (_link leaves this README page_)
- [Description](#description)
    - [Challenges](#challenges)
    - [Solutions](#solutions)
    - [Tech](#tech)
    - [Extra features](#extra-features)
- [Installation](#installation)
- [Usage](#usage)
<!-- - [Project Visuals](#project-visuals) -->
- [Acknowledgements](#acknowledgements)
- [Footnotes](#footnotes)

## Description

This program was developed for a prop in an October play.

### Challenges

1. Have the rabbit behave one way while alive & another way immediately after death.
    - _my first theatre tech idea, although I was initially thinking of using capacitors_
2. Have the servo horn finish at the middle position.

### Solutions

1. Created a `Rabbit` class
    - a public [`struggle()`][struggle] method for when it is "alive"
        - while continuity through a wire is sensed by the microcontroller
    - a public [`headless()`][headless] method for when it is "decapitated"
        - when continuity is no longer sensed
2. Within `headless()` is a conditional that handles the behavior immediately after decapitation is sensed along with some dramatization I added in
    - private [`initDecap()`][initDecap] method
        - one last struggle movement
        - a brief pause (_slightly extra time delay_)
        - change `decap` state to `true`
    - private [`decapAccelNewAng()`][decapAccelNewAng] method
        - starts moving slowly at first; then accelerates to the middle position

### Tech

- C++
- Arduino
    - _IDE 2.3.3_
- Circuit Playground Express
    - _& other boards_
- Servo motor
    - _9g_
    - _180 deg_

### Extra features

- my experience as an electrician taught me the importance of thinking ahead for potential challenges
    1. I included a _guard_ for accidental death (_disconnection/reconnection of the wire used for continuity_)
        - there is some time allowed for the rabbit to go back to its alive-movements if continuity is sensed again
        - see `Rabbit` class:
            - private [`decreaseSurvivability()`][decreaseSurvivability] method
                - invoked within `headless()`
            - private [`survivability`][survivability] field assigned the value of the private [`initSurvivability`][initSurvivability] field
                - decremented within `decreaseSurvivability()`
                - returned by getter [`revivable()`][revivable]
                    - used as a conditional within [`loop()`][loop] in the `.ino` file
            - private [`easterBunny()`][easterBunny] method
                - invoked within `struggle()`
    2. just in case the servo orientation had to be one way as opposed to the other when inserted into the rabbit model, I added code to easily allow it
        - see the [`Rabbit` constructor][Rabbit-constructor] argument `feetTowardsHighAng`
    3. to help protect the servo:
        - since not all servos are aligned to the same pulse widths (_microseconds_), I also included code to test for the servo's pulse width
            - the program is automatically set with macros in the `.ino` file to test this first
                - see [`TestingMyServo`][TestingMyServo]
            - the values to help test this are set by adjusting those assigned to [`lowestPulseWidth`][lowestPulseWidth] & [`highestPulseWidth`][highestPulseWidth]
        - a static [`create()`][create] method is used to run checks before allowing the instantiation of a `Rabbit` object
            - _I didn't want the creation of an object with unsuitable conditions or state_
- lastly, since I utilized features of the Circuit Playground Express during building & iterating on the program while unsure of what microcontroller would be used during the play, I include macros to compile some code only if the Circuit Playground Express is used
    - see [`issa_CPx`][issa_CPx] in the `.ino` file
    - most microcontrollers do not include the specific features that this code utilizes
        - _pinned onboard switch, button, & LED_
<!-- mention the edge-case bug here ?? -->

[Table of Contents](#table-of-contents)

## Installation

- _at the time of writing_

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (_external link_)
    - [Set Up for Circuit Playground Express](https://learn.adafruit.com/adafruit-circuit-playground-express/set-up-arduino-ide) (_external link_)

### Steps

#### Git Clone

If you decide to clone this entire repo:
1. open the command line to the directory that will hold the clone & clone the repo; for example:
    - `git clone https://github.com/anthonychavis/my-arduino-repo.git`
2. open the Arduino IDE -> _File_ -> _Open_:
    - go to `./my-arduino-repo/Theatre/Rabbit`
        - _found within the directory where the repo was cloned_
    - delete the `.ino` file there
    - go to `./Rabbit_Class`
    - select to open the `RabbitClass.ino` file
        - there will be a prompt to create a directory of the same name as this file
            - _a lesson learned for me_
        - the `.ino` file will move into that new directory
3. move the `.hpp` file from `./Rabbit_Class` into the new directory

#### Compile & Upload

Within the Arduino IDE:
1. select the correct board & port
2. recommend first "_verifying_" before "_uploading_"
    - click the _checkmark_ icon to test compilation
    - pay attention to the output
        - _an output console pane will appear in the IDE_
3. if compilation is successful, click the _upload_ icon

[Table of Contents](#table-of-contents)

## Usage

### Serial Monitor

- To view the _print_ messages, go to _Tools_ -> _Serial Monitor_
    - a _Serial Monitor_ tab will open next to the _Output_ tab
    - all programmed output messages will appear here
- After successfully uploading the program onto the microcontroller:
    - either unplug the USB cable from the board/computer & plug it back in, or press the _reset_ button on the board
        - the messages from `setup()` will appear
- While the program is running & connected to the IDE:
    - there is nothing printed to the _Serial Monitor_ while `!discontinuous`, or when `struggle()` can be invoked
    - make sure the **baud rate** in the IDE's Serial Monitor is set to 9600 <!-- add img for clarity -->
        - if the baud rate has to change for a specific board, it should match the value set in [`Serial.begin()`][serial-begin_docs] (_external link_) within the `setup()` function
            - [`Serial.begin()`][serial-begin_testing] when testing the servo
            - [`Serial.begin()`][serial-begin_not-testing] when not testing the servo

[Table of Contents](#table-of-contents)

<!-- ## Project Visuals

<details style='cursor:pointer;'>
<summary><h3>GIFs</h3></summary>

</details>

[Table of Contents](#table-of-contents) 
-->

## Acknowledgements

- this program is currently built to work with either the [`Servo.h`][Servoh] (_external link_) or [`ESP32Servo.h`][ESP32Servoh] (_external link_) servo library

[Table of Contents](#table-of-contents)

## Footnotes

[^1]: The fin & shark reference a joke that came about during show prep.


<!-- TOC -->
[Rabbit_Class]: https://github.com/anthonychavis/my-arduino-repo/tree/main/Theatre/Rabbit/Rabbit_Class "to the Rabbit_Class directory"

<!-- Description -->
[struggle]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L196 "to the struggle() definition"
[headless]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L180 "to the headless() definition"
[initDecap]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L112 "to the initDecap() definition"
[decapAccelNewAng]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L72 "to the decapAccelNewAng() definition"
[decreaseSurvivability]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L96 "to the decreaseSurvivability() definition"
[survivability]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L57 "to the survivability assignment"
[initSurvivability]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L56 "to the initSurvivability assignment"
[revivable]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L217 "to the revivable() definition"
[loop]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L185 "to the loop() function - when !(TestingMyServo)"
[easterBunny]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L104 "to the easterBunny() definition"
[Rabbit-constructor]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L140 "to the Rabbit constructor"
[TestingMyServo]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L74 "to the TestingMyServo assignment"
[lowestPulseWidth]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L31 "to the lowestPulseWidth assignment"
[highestPulseWidth]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L32 "to the highestPulseWidth assignment"
[create]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L156 "to the create() definition"
[issa_CPx]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L16 "to the issa_CPx declaration"

<!-- Usage -->
[serial-begin_docs]: https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/ "external link to the Serial.begin() documentation"
[serial-begin_testing]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L109 "to the Serial.begin() when TestingMyServo"
[serial-begin_not-testing]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L155 "to the Serial.begin() when !TestingMyServo"

<!-- Acknowledgements -->
[Servoh]: https://docs.arduino.cc/libraries/servo/ "external link to the Arduino Servo.h documentation"
[ESP32Servoh]: https://docs.arduino.cc/libraries/esp32servo/ "external link to the Arduino ESP32Servo.h documentation"
