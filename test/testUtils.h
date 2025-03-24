#ifndef DEBOUNCE_ME_TESTUTILS_H
#define DEBOUNCE_ME_TESTUTILS_H

#include <unity.h>
#include "Button.h"
#include <map>
#include <functional>

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

const char *getActionName(void (*action)()) {
    std::map<void (*)(), std::string> actionNames = {
            {none,                          "NONE"},
            {pressA,                       "PRESS_A"},
            {longPressA,                  "LONG_PRESS_A"},
            {doublePressA,                "DOUBLE_PRESS_A"},
            {pressB,                       "PRESS_B"},
            {longPressB,                  "LONG_PRESS_B"},
            {doublePressB,                "DOUBLE_PRESS_B"},
            {pressC,                       "PRESS_C"},
            {longPressC,                  "LONG_PRESS_C"},
            {doublePressC,                "DOUBLE_PRESS_C"},
            {simultaneousPressAB,        "SIMULTANEOUS_PRESS_A_B"},
            {simultaneousLongPressAB,   "SIMULTANEOUS_LONG_PRESS_A_B"},
            {simultaneousPressAC,        "SIMULTANEOUS_PRESS_A_C"},
            {simultaneousLongPressAC,   "SIMULTANEOUS_LONG_PRESS_A_C"},
            {simultaneousPressBC,        "SIMULTANEOUS_PRESS_B_C"},
            {simultaneousLongPressBC,   "SIMULTANEOUS_LONG_PRESS_B_C"},
            {simultaneousPressABC,      "SIMULTANEOUS_PRESS_A_B_C"},
            {simultaneousLongPressABC, "SIMULTANEOUS_LONG_PRESS_A_B_C"}
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
}

void pushButton(Button *button) {
    digitalWrite(button->pin, LOW);
}

void releaseButton(Button *button) {
    digitalWrite(button->pin, HIGH);
}

void assertAllEqual0Except(void (*action)(), int times = 1) {
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

#endif //DEBOUNCE_ME_TESTUTILS_H
