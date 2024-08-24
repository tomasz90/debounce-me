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
    buttonsHandler.setSimultaneousBehavior({buttonA, buttonB}, combination);
    buttonsHandler.pollOnce(); // poll here once (need to use FreeRTOS)
}

void loop() {
    // alternatively: buttonsHandler.poll();
}