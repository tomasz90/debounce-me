#ifndef DEBOUNCE_ME_TESTUTILS_H
#define DEBOUNCE_ME_TESTUTILS_H

#include <unity.h>
#include "Button.h"
#include <map>

enum Action {
    NONE,
    PRESS_A,
    LONG_PRESS_A,
    DOUBLE_PRESS_A,
    PRESS_B,
    LONG_PRESS_B,
    DOUBLE_PRESS_B,
    PRESS_C,
    LONG_PRESS_C,
    DOUBLE_PRESS_C,
    SIMULTANEOUS_PRESS_A_B,
    SIMULTANEOUS_LONG_PRESS_A_B,
    SIMULTANEOUS_PRESS_A_C,
    SIMULTANEOUS_LONG_PRESS_A_C,
    SIMULTANEOUS_PRESS_B_C,
    SIMULTANEOUS_LONG_PRESS_B_C,
    SIMULTANEOUS_PRESS_A_B_C,
    SIMULTANEOUS_LONG_PRESS_A_B_C
};

const char *getActionName(Action action) {
    std::map<Action, std::string> actionNames = {
            {NONE,                          "NONE"},
            {PRESS_A,                       "PRESS_A"},
            {LONG_PRESS_A,                  "LONG_PRESS_A"},
            {DOUBLE_PRESS_A,                "DOUBLE_PRESS_A"},
            {PRESS_B,                       "PRESS_B"},
            {LONG_PRESS_B,                  "LONG_PRESS_B"},
            {DOUBLE_PRESS_B,                "DOUBLE_PRESS_B"},
            {PRESS_C,                       "PRESS_C"},
            {LONG_PRESS_C,                  "LONG_PRESS_C"},
            {DOUBLE_PRESS_C,                "DOUBLE_PRESS_C"},
            {SIMULTANEOUS_PRESS_A_B,        "SIMULTANEOUS_PRESS_A_B"},
            {SIMULTANEOUS_LONG_PRESS_A_B,   "SIMULTANEOUS_LONG_PRESS_A_B"},
            {SIMULTANEOUS_PRESS_A_C,        "SIMULTANEOUS_PRESS_A_C"},
            {SIMULTANEOUS_LONG_PRESS_A_C,   "SIMULTANEOUS_LONG_PRESS_A_C"},
            {SIMULTANEOUS_PRESS_B_C,        "SIMULTANEOUS_PRESS_B_C"},
            {SIMULTANEOUS_LONG_PRESS_B_C,   "SIMULTANEOUS_LONG_PRESS_B_C"},
            {SIMULTANEOUS_PRESS_A_B_C,      "SIMULTANEOUS_PRESS_A_B_C"},
            {SIMULTANEOUS_LONG_PRESS_A_B_C, "SIMULTANEOUS_LONG_PRESS_A_B_C"}
    };

    return actionNames[action].c_str();
}

std::map<Action, int> mapOfActions;

void none() {} // do nothing

void pressA() { mapOfActions[PRESS_A]++; }

void longPressA() { mapOfActions[LONG_PRESS_A]++; }

void doublePressA() { mapOfActions[DOUBLE_PRESS_A]++; }

void pressB() { mapOfActions[PRESS_B]++; }

void longPressB() { mapOfActions[LONG_PRESS_B]++; }

void doublePressB() { mapOfActions[DOUBLE_PRESS_B]++; }

void pressC() { mapOfActions[PRESS_C]++; }

void longPressC() { mapOfActions[LONG_PRESS_C]++; }

void doublePressC() { mapOfActions[DOUBLE_PRESS_C]++; }

void simultaneousPressAB() { mapOfActions[SIMULTANEOUS_PRESS_A_B]++; }

void simultaneousLongPressAB() { mapOfActions[SIMULTANEOUS_LONG_PRESS_A_B]++; }

void simultaneousPressAC() { mapOfActions[SIMULTANEOUS_PRESS_A_C]++; }

void simultaneousLongPressAC() { mapOfActions[SIMULTANEOUS_LONG_PRESS_A_C]++; }

void simultaneousPressBC() { mapOfActions[SIMULTANEOUS_PRESS_B_C]++; }

void simultaneousLongPressBC() { mapOfActions[SIMULTANEOUS_LONG_PRESS_B_C]++; }

void simultaneousPressABC() { mapOfActions[SIMULTANEOUS_PRESS_A_B_C]++; }

void simultaneousLongPressABC() { mapOfActions[SIMULTANEOUS_LONG_PRESS_A_B_C]++; }

void resetActions() {
    mapOfActions[NONE] = 0;
    mapOfActions[PRESS_A] = 0;
    mapOfActions[LONG_PRESS_A] = 0;
    mapOfActions[DOUBLE_PRESS_A] = 0;
    mapOfActions[PRESS_B] = 0;
    mapOfActions[LONG_PRESS_B] = 0;
    mapOfActions[DOUBLE_PRESS_B] = 0;
    mapOfActions[PRESS_C] = 0;
    mapOfActions[LONG_PRESS_C] = 0;
    mapOfActions[DOUBLE_PRESS_C] = 0;
    mapOfActions[SIMULTANEOUS_PRESS_A_B] = 0;
    mapOfActions[SIMULTANEOUS_LONG_PRESS_A_B] = 0;
    mapOfActions[SIMULTANEOUS_PRESS_A_C] = 0;
    mapOfActions[SIMULTANEOUS_LONG_PRESS_A_C] = 0;
    mapOfActions[SIMULTANEOUS_PRESS_B_C] = 0;
    mapOfActions[SIMULTANEOUS_LONG_PRESS_B_C] = 0;
    mapOfActions[SIMULTANEOUS_PRESS_A_B_C] = 0;
    mapOfActions[SIMULTANEOUS_LONG_PRESS_A_B_C] = 0;
}

void pushButton(Button *button) {
    digitalWrite(button->pin, LOW);
}

void releaseButton(Button *button) {
    digitalWrite(button->pin, HIGH);
}

void assertAllEqual0Except(Action action, int times = 1) {
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

void assertAllEqual0() {
    assertAllEqual0Except(NONE, 0);
}

void loop() {
    // Required by Arduino
}

#endif //DEBOUNCE_ME_TESTUTILS_H
