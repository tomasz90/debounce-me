#ifndef DEBOUNCE_ME_TESTUTILS_H
#define DEBOUNCE_ME_TESTUTILS_H

enum Actions {
    PRESS_A,
    LONG_PRESS_A,
    PRESS_B,
    LONG_PRESS_B,
    PRESS_C,
    LONG_PRESS_C,
    SIMULTANEOUS_PRESS_A_B,
    SIMULTANEOUS_LONG_PRESS_A_B,
    SIMULTANEOUS_PRESS_A_C,
    SIMULTANEOUS_LONG_PRESS_A_C,
    SIMULTANEOUS_PRESS_B_C,
    SIMULTANEOUS_LONG_PRESS_B_C,
    SIMULTANEOUS_PRESS_A_B_C,
    SIMULTANEOUS_LONG_PRESS_A_B_C
};

std::map<Actions, int> mapOfActions;

void pressA() { mapOfActions[PRESS_A]++; }
void longPressA() { mapOfActions[LONG_PRESS_A]++; }
void pressB() { mapOfActions[PRESS_B]++; }
void longPressB() { mapOfActions[LONG_PRESS_B]++; }
void pressC() { mapOfActions[PRESS_C]++; }
void longPressC() { mapOfActions[LONG_PRESS_C]++; }
void simultaneousPressAB() { mapOfActions[SIMULTANEOUS_PRESS_A_B]++; }
void simultaneousLongPressAB() { mapOfActions[SIMULTANEOUS_LONG_PRESS_A_B]++; }
void simultaneousPressAC() { mapOfActions[SIMULTANEOUS_PRESS_A_C]++; }
void simultaneousLongPressAC() { mapOfActions[SIMULTANEOUS_LONG_PRESS_A_C]++; }
void simultaneousPressBC() { mapOfActions[SIMULTANEOUS_PRESS_B_C]++; }
void simultaneousLongPressBC() { mapOfActions[SIMULTANEOUS_LONG_PRESS_B_C]++; }
void simultaneousPressABC() { mapOfActions[SIMULTANEOUS_PRESS_A_B_C]++; }
void simultaneousLongPressABC() { mapOfActions[SIMULTANEOUS_LONG_PRESS_A_B_C]++; }

void resetActions() {
    mapOfActions[PRESS_A] = 0;
    mapOfActions[LONG_PRESS_A] = 0;
    mapOfActions[PRESS_B] = 0;
    mapOfActions[LONG_PRESS_B] = 0;
    mapOfActions[PRESS_C] = 0;
    mapOfActions[LONG_PRESS_C] = 0;
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

void assertAllEqual0Except(Actions action, int times = 1) {
    for (auto &pair : mapOfActions) {
        if (pair.first != action) {
            TEST_ASSERT_EQUAL(0, pair.second);
        }
    }
    TEST_ASSERT_EQUAL(times, mapOfActions[action]);
}

void loop() {
    // Required by Arduino
}

#endif //DEBOUNCE_ME_TESTUTILS_H
