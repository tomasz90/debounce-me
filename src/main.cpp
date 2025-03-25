#include "Set.h"
#include "Button.h"

#if !defined(TEST_BUILD)
auto buttonA = new Button(1, IN_PULLUP);
auto buttonB = new Button(2, IN_PULLUP);
auto buttonC = new Button(3, IN_PULLUP);

std::set<Button *> buttons1 = {buttonA, buttonB, buttonC, buttonA};
std::set<Button *> buttons2 = {buttonB, buttonC, buttonA};
std::set<Button *> buttons3 = {buttonB, buttonC, buttonA};

void setup() {
    Serial.begin(115200);
    buttons1.insert(buttonA);
}

void loop() {
    Serial.print("The same: ");
    Serial.println(buttons1 == buttons2);
    delay(1000);

    Serial.print("Size: ");
    Serial.println(buttons1.size());
}

#endif