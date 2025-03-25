#include <Arduino.h>
#include <Button.h>
#include <ButtonsHandler.h>

#define BUTTON_PIN_A 12
#define BUTTON_PIN_B 13
#define BUTTON_PIN_C 15

void clicked(String s) { Serial.println("Button clicked: " + s); }

Button *buttonA = new Button(BUTTON_PIN_A, IN_PULLUP);
Button *buttonB = new Button(BUTTON_PIN_B, IN_PULLUP);

ButtonsHandler buttonsHandler({buttonA, buttonB});

#ifndef UNITY_INCLUDE_CONFIG_H

void setup() {
    Serial.begin(BAUD_RATE);
    buttonA->setClick([] { clicked("A"); });
    buttonB->setClick([] { clicked("B"); });
    buttonA->setClickLong([] { clicked("A LONG"); });
    buttonsHandler.setClickSimultaneous({buttonA, buttonB}, [] { clicked("AB"); });
#ifdef INC_FREERTOS_H // remove macros according to your needs
    buttonsHandler.pollOnce(); // poll here once (need to use FreeRTOS)
#endif
}

void loop() {
#ifndef INC_FREERTOS_H
    buttonsHandler.poll();
    delay(1);
#endif
}

#endif
