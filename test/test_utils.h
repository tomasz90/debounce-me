#pragma once

#ifndef DEBOUNCE_ME_TEST_UTILS_H
#define DEBOUNCE_ME_TEST_UTILS_H

#ifndef ARDUINO_ARCH_AVR

#include <unity.h>
#include <ArxContainer.h>
#include "Button.h"
#include "test_config.h"

std::map<void (*)(), int> mapOfActions;

void none() {} // do nothing
void pressA() { mapOfActions[pressA]++; }
void longPressA() { mapOfActions[longPressA]++; }
void doublePressA() { mapOfActions[doublePressA]++; }
void pressB() { mapOfActions[pressB]++; }
void longPressB() { mapOfActions[longPressB]++; }
void doublePressB() { mapOfActions[doublePressB]++; }
void pressC() { mapOfActions[pressC]++; }
void longPressC() { mapOfActions[longPressC]++; }
void doublePressC() { mapOfActions[doublePressC]++; }
void simultaneousPressAB() { mapOfActions[simultaneousPressAB]++; }
void simultaneousLongPressAB() { mapOfActions[simultaneousLongPressAB]++; }
void simultaneousPressAC() { mapOfActions[simultaneousPressAC]++; }
void simultaneousLongPressAC() { mapOfActions[simultaneousLongPressAC]++; }
void simultaneousPressBC() { mapOfActions[simultaneousPressBC]++; }
void simultaneousLongPressBC() { mapOfActions[simultaneousLongPressBC]++; }
void simultaneousPressABC() { mapOfActions[simultaneousPressABC]++; }
void simultaneousLongPressABC() { mapOfActions[simultaneousLongPressABC]++; }
void simultaneousPressABCD() { mapOfActions[simultaneousPressABCD]++; }

const char *getActionName(void (*action)()) {
    std::map<void (*)(), String> actionNames = {
            {none,                     "none"},
            {pressA,                   "pressA"},
            {longPressA,               "longPressA"},
            {doublePressA,             "doublePressA"},
            {pressB,                   "pressB"},
            {longPressB,               "longPressB"},
            {doublePressB,             "doublePressB"},
            {pressC,                   "pressC"},
            {longPressC,               "longPressC"},
            {doublePressC,             "doublePressC"},
            {simultaneousPressAB,      "simultaneousPressAB"},
            {simultaneousLongPressAB,  "simultaneousLongPressAB"},
            {simultaneousPressAC,      "simultaneousPressAC"},
            {simultaneousLongPressAC,  "simultaneousLongPressAC"},
            {simultaneousPressBC,      "simultaneousPressBC"},
            {simultaneousLongPressBC,  "simultaneousLongPressBC"},
            {simultaneousPressABC,     "simultaneousPressABC"},
            {simultaneousLongPressABC, "simultaneousLongPressABC"},
            {simultaneousPressABCD,    "simultaneousPressABCD"},
    };

    return actionNames[action].c_str();
}

void resetActions() {
    mapOfActions[none] = 0;
    mapOfActions[pressA] = 0;
    mapOfActions[longPressA] = 0;
    mapOfActions[doublePressA] = 0;
    mapOfActions[pressB] = 0;
    mapOfActions[longPressB] = 0;
    mapOfActions[doublePressB] = 0;
    mapOfActions[pressC] = 0;
    mapOfActions[longPressC] = 0;
    mapOfActions[doublePressC] = 0;
    mapOfActions[simultaneousPressAB] = 0;
    mapOfActions[simultaneousLongPressAB] = 0;
    mapOfActions[simultaneousPressAC] = 0;
    mapOfActions[simultaneousLongPressAC] = 0;
    mapOfActions[simultaneousPressBC] = 0;
    mapOfActions[simultaneousLongPressBC] = 0;
    mapOfActions[simultaneousPressABC] = 0;
    mapOfActions[simultaneousLongPressABC] = 0;
    mapOfActions[simultaneousPressABCD] = 0;
}

void pushButton(Button *button) {
    digitalWrite(button->pin, LOW);
}

void releaseButton(Button *button) {
    digitalWrite(button->pin, HIGH);
}

void assertAllEqual0Except(void (*action)(), int times = 1) {
    delay(DELAY_BETWEEN_TESTS);
    for (auto &pair: mapOfActions) {
        if (pair.first != action) {
            if (0 != pair.second) {
                Serial.print("WHEN CHECKING: ");
                Serial.print(getActionName(action));
                Serial.print(", ");
                auto actionName = getActionName(pair.first);
                Serial.print(actionName);
                Serial.print(" should be: 0, but was: ");
                Serial.print(pair.second);
                Serial.print(" ->>>>>>>   ");
            }
            TEST_ASSERT_EQUAL(0, pair.second);
        }
    }
    if (times != mapOfActions[action]) {
        Serial.print("WHEN CHECKING: ");
        Serial.print(getActionName(action));
        Serial.printf(", should overall be actions: %d, but was: %d  ->>>>>>>   ", times, mapOfActions[action]);
    }
    TEST_ASSERT_EQUAL(times, mapOfActions[action]);
}

void assertAllEqual0() { assertAllEqual0Except(none, 0); }

void loop() { } /* Required by Arduino */

#endif
#endif //DEBOUNCE_ME_TEST_UTILS_H
