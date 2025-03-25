#include "Set.h"
#include "Button.h"

#if !defined(TEST_BUILD)
auto buttonA = new Button(1, IN_PULLUP);
auto buttonB = new Button(2, IN_PULLUP);
auto buttonC = new Button(3, IN_PULLUP);
auto buttonD = new Button(4, IN_PULLUP);
auto buttonE = new Button(5, IN_PULLUP);
auto buttonF = new Button(6, IN_PULLUP);

std::set<Button *> buttons = {buttonA, buttonB, buttonC, buttonD, buttonE, buttonF};

void setup() {
    Serial.begin(115200);
}

void loop() {
    for (auto button : buttons) {
        Serial.println("PIN: "+ String(button->pin));
    }
    Serial.println("Size: " + String(buttons.size()));
    delay(1000);
}

#endif