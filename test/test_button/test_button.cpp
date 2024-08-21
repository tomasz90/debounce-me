#include <unity.h>
#include "Button.h"
#include "ButtonsHandler.h"

#define BUTTON_PIN_A 2
#define BUTTON_PIN_B 21
#define BUTTON_PIN_C 22

#define LONG_PRESS_TIME 300
#define SMALL_DELAY 100
#define LONG_PRESS_DELAY 350

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

Button* buttonA = new Button(BUTTON_PIN_A, IN_PULLUP);
Button* buttonB = new Button(BUTTON_PIN_B, IN_PULLUP);
Button* buttonC = new Button(BUTTON_PIN_C, IN_PULLUP);

ButtonsHandler buttonsHandler({buttonA, buttonB, buttonC});

void pushButton(Button *button) {
    digitalWrite(button->pin, LOW);
}

void releaseButton(Button *button) {
    digitalWrite(button->pin, HIGH);
}

void setUp(void) {
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

    releaseButton(buttonA);
    releaseButton(buttonB);
    releaseButton(buttonC);
}

void tearDown(void) {
    // This function is run after EACH TEST
}

void assertEqual0Except(Actions action) {
    for (auto &pair : mapOfActions) {
        if (pair.first != action) {
            TEST_ASSERT_EQUAL(0, pair.second);
        }
    }
}

void testOnPress(void) {
    pushButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    TEST_ASSERT_EQUAL(1, mapOfActions[PRESS_A]);
    assertEqual0Except(PRESS_A);
}

void testOnPressLong(void) {
    pushButton(buttonA);
    delay(LONG_PRESS_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    TEST_ASSERT_EQUAL(1, mapOfActions[LONG_PRESS_A]);
    assertEqual0Except(LONG_PRESS_A);
}

void setup() {
    Serial.begin(115200);

    pinMode(BUTTON_PIN_A, OUTPUT);
    pinMode(BUTTON_PIN_B, OUTPUT);
    pinMode(BUTTON_PIN_C, OUTPUT);

    buttonA->setBehavior(pressA, longPressA, LONG_PRESS_TIME, true);
    buttonB->setBehavior(pressB, longPressB, LONG_PRESS_TIME, true);
    buttonC->setBehavior(pressC, longPressC, LONG_PRESS_TIME, true);

    buttonsHandler.setSimultaneousBehavior({buttonA, buttonB}, simultaneousPressAB);
    buttonsHandler.setSimultaneousBehaviorLong({buttonA, buttonB}, simultaneousLongPressAB);
    buttonsHandler.setSimultaneousBehavior({buttonA, buttonC}, simultaneousPressAC);
    buttonsHandler.setSimultaneousBehaviorLong({buttonA, buttonC}, simultaneousLongPressAC);
    buttonsHandler.setSimultaneousBehavior({buttonB, buttonC}, simultaneousPressBC);
    buttonsHandler.setSimultaneousBehaviorLong({buttonB, buttonC}, simultaneousLongPressBC);
    buttonsHandler.setSimultaneousBehavior({buttonA, buttonB, buttonC}, simultaneousPressABC);
    buttonsHandler.setSimultaneousBehaviorLong({buttonA, buttonB, buttonC}, simultaneousLongPressABC);

    auto polled = [](TimerHandle_t xTimer) { buttonsHandler.poll();};
    auto xTimer = xTimerCreate(NULL, pdMS_TO_TICKS(1), true, NULL, polled);
    xTimerStart(xTimer, 0);

    UNITY_BEGIN();

    RUN_TEST(testOnPress);
    RUN_TEST(testOnPressLong);

    UNITY_END();

    xTimerStop(xTimer, 0);
    xTimerDelete(xTimer, 0);
}

void loop() {
    // This function is required by Arduino, but you can leave it empty if you don't need it.
}
