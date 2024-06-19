#include <Arduino.h>
#include "Button.h"
#include "ButtonsHandler.h"

#define BUTTON_PIN 27

void pressed() { Serial.println("Button pressed"); }

void pressedLong() { Serial.println("Button pressed long"); }

Button* button = new Button(BUTTON_PIN, IN_PULLUP);
ButtonsHandler buttonsHandler({button});  // can put multiple buttons here {button1, button2...}

void setup() {
    Serial.begin(115200);
    button->setBehavior(pressed, pressedLong);
}

void loop() {
    buttonsHandler.poll();
}