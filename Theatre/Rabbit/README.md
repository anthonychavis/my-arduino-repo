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
            - I also didn't want the creation of an object with unsuitable conditions or state
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

- if you decide to clone this entire repo:
    - open the command line to the directory that will hold the clone & clone the repo; for example:
        - `git clone https://github.com/anthonychavis/my-arduino-repo.git`
- open the Arduino IDE -> File -> Open:
    - go to `./my-arduino-repo/Theatre/Rabbit`
    - delete the `.ino` file there
    - go to `./Rabbit_Class`
    - select to open the `RabbitClass.ino` file
        - there will be a prompt to create a directory of the same name as this file
            - _a lesson learned for me_
        - the `.ino` file will move into that new directory
- move the `.hpp` file from `./Rabbit_Class` into the new directory
- select the correct board & port
- recommend first "_verifying_" before "_uploading_"
    - click the _checkmark_ icon to test compilation
    - pay attention to the output
- if compilation is successful, click the _upload_ icon

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

[Rabbit_Class]: https://github.com/anthonychavis/my-arduino-repo/tree/main/Theatre/Rabbit/Rabbit_Class
[struggle]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L196
[headless]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L180
[initDecap]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L112
[decapAccelNewAng]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L72
[decreaseSurvivability]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L96
[survivability]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L57
[initSurvivability]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L56
[revivable]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L217
[loop]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L185
[easterBunny]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L104
[Rabbit-constructor]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L140
[TestingMyServo]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L74
[lowestPulseWidth]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L31
[highestPulseWidth]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L32
[create]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass_Res.hpp#L156
[issa_CPx]: https://github.com/anthonychavis/my-arduino-repo/blob/main/Theatre/Rabbit/Rabbit_Class/RabbitClass.ino#L16
[Servoh]: https://docs.arduino.cc/libraries/servo/
[ESP32Servoh]: https://docs.arduino.cc/libraries/esp32servo/