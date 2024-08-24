# Debounce Me

This debounce button library is designed to handle the common issue of switch bounce in mechanical buttons. When a mechanical button is pressed or released, it often creates multiple transitions (bounces) due to the mechanical contacts making and breaking multiple times before settling. This can result in false triggering of button presses and releases in your program.

There are already few libraries in PlatformIO, but none of them were perfect, or even work properly. I think this library is outstanding for its simplicity and efficiency.

Below is simple example how to use it. Notice that for only two hypothetical buttons, you can define as much as 6 behaviors (pressedA, pressedB, pressedLongA, pressedLongB, combinationAB, combinationLongAB), and for 3 buttons you can define 14 behaviors!!!
Use `pollOnce()` - to call only once in startup, or `poll()` - to call constantly inside a loop.

```c++
#include <Arduino.h>
#include "Button.h"
#include "ButtonsHandler.h"

#define BUTTON_PIN_A 27
#define BUTTON_PIN_B 34

void pressed() { Serial.println("Button pressed"); }

void pressedLong() { Serial.println("Button pressed long"); }

void combination() { Serial.println("Combination pressed"); }

Button* buttonA = new Button(BUTTON_PIN_A, IN_PULLUP);
Button* buttonB = new Button(BUTTON_PIN_B, IN_PULLUP);

ButtonsHandler buttonsHandler({buttonA, buttonB});

void setup() {
    Serial.begin(115200);
    buttonA->setBehavior(pressed, pressedLong);
    buttonsHandler.setSimultaneousBehavior({buttonA, buttonB}, combintation);
    
    // here pollOnce (need to use FreeRTOS) OR inside loop call buttonsHandler.poll()
    // both ways should work similarly
    buttonsHandler.pollOnce(); 
}

void loop() {
    buttonsHandler.poll(); // alternative to pollOnce
}
```
