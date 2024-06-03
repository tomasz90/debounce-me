#include <Arduino.h>
#include "ButtonsHandler.h"

void pressed() { Serial.println("Button pressed"); }

void pressedLong() { Serial.println("Button pressed long"); }

Button button(27, IN_PULLUP, pressed, pressedLong);
ButtonsHandler buttonsHandler({button}); // can put multiple buttons here

void setup() {
    Serial.begin(115200);
}

void loop() {
    buttonsHandler.poll();
}