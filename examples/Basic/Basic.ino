#include <Arduino.h>
#include "ButtonsHandler.h"

#define BUTTON_PIN 27

void pressed() { Serial.println("Button pressed"); }

void pressedLong() { Serial.println("Button pressed long"); }

Button button(BUTTON_PIN, IN_PULLUP, pressed, pressedLong);
ButtonsHandler buttonsHandler({button});  // can put multiple buttons here {button1, button2...}

void setup() {
    Serial.begin(115200);
}

void loop() {
    buttonsHandler.poll();
}