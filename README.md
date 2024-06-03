# Debounce Me

This debounce button library is designed to handle the common issue of switch bounce in mechanical buttons. When a mechanical button is pressed or released, it often creates multiple transitions (bounces) due to the mechanical contacts making and breaking multiple times before settling. This can result in false triggering of button presses and releases in your program.

There are already few libraries in PlatformIO, but none of them were perfect, or even work properly. I think this library is outstanding for its simplicity and efficiency.

Below is simple example how to use it:

```c++
#include <Arduino.h>
#include "ButtonsHandler.h"

#define BUTTON_PIN 27

void pressed() { Serial.println("Button pressed"); }

void pressedLong() { Serial.println("Button pressed long"); }

Button button(BUTTON_PIN, IN_PULLUP, pressed, pressedLong);
ButtonsHandler buttonsHandler({button}); // can put multiple buttons here {button1, button2...}

void setup() {
    Serial.begin(115200);
}

void loop() {
    buttonsHandler.poll();
}
```
