#include <Arduino.h>
#include "Wire.h"
#include "Button.h"
#include "ButtonsHandler.h"

void pressed() { Serial.println("Button pressed"); }

void pressedLong() { Serial.println("Button pressed long"); }

void combination() { Serial.println("Combination pressed"); }

Button buttonA(BUTTON_PIN_A, IN_PULLUP);
Button buttonB(BUTTON_PIN_B, IN_PULLUP);

Button *buttons[] = {&buttonA, &buttonB};
ButtonsHandler buttonsHandler(buttons, sizeof(buttons) / sizeof(buttons[0]));

void setup() {
    Serial.begin(115200);
    buttonA->setClick(pressed);
    buttonA->setClickLong(pressedLong);
    buttonsHandler.setClickSimultaneous({buttonA, buttonB}, combination);
}

void loop() {
    buttonsHandler.poll();
    delay(1);
}