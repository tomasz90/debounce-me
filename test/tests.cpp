#include <unity.h>
#include "../src/Button.h"
#include "../src/ButtonsHandler.h"
#include "testUtils.h"

#define BUTTON_PIN_A 2
#define BUTTON_PIN_B 21
#define BUTTON_PIN_C 22

#define LONG_PRESS_TIME 300
#define SIMULTANEOUS_PRESS_TIME 600
#define SMALL_DELAY 100
#define LONG_PRESS_DELAY 350
#define SIMULTANEOUS_PRESS_DELAY 650

Button *buttonA = new Button(BUTTON_PIN_A, IN_PULLUP);
Button *buttonB = new Button(BUTTON_PIN_B, IN_PULLUP);
Button *buttonC = new Button(BUTTON_PIN_C, IN_PULLUP);

ButtonsHandler buttonsHandler({buttonA, buttonB, buttonC});

void setUp(void) {
    resetActions();

    releaseButton(buttonA);
    releaseButton(buttonB);
    releaseButton(buttonC);
}

void tearDown(void) {
    // This function is run after EACH TEST
}

void testOnPress(void) {
    pushButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
}

void testOnPressLong(void) {
    pushButton(buttonA);
    delay(LONG_PRESS_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(LONG_PRESS_A);
}

void testOnPressLongMultiple(void) {
    pushButton(buttonA);
    delay(3 * LONG_PRESS_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(LONG_PRESS_A, 3);
}

void testOnPressSimultaneous(void) {
    pushButton(buttonA);
    pushButton(buttonB);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B);
}

void testOnPressSimultaneous2(void) {
    pushButton(buttonA);
    pushButton(buttonB);
    pushButton(buttonC);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    releaseButton(buttonB);
    releaseButton(buttonC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B_C);
}

void testOnPressSimultaneous3(void) {
    pushButton(buttonA);
    delay(SMALL_DELAY);

    pushButton(buttonB);
    delay(SMALL_DELAY);

    releaseButton(buttonB);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B);
}

void testOnPressSimultaneousLong(void) {
    pushButton(buttonA);
    pushButton(buttonB);
    delay(SIMULTANEOUS_PRESS_DELAY);

    releaseButton(buttonA);
    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B);
}

void testOnPressSimultaneousLong2(void) {
    pushButton(buttonA);
    pushButton(buttonB);
    pushButton(buttonC);
    delay(SIMULTANEOUS_PRESS_DELAY);

    releaseButton(buttonA);
    releaseButton(buttonB);
    releaseButton(buttonC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B_C);
}

void testNotOnPressSimultaneousLong(void) {
    pushButton(buttonA);
    pushButton(buttonB);
    delay(LONG_PRESS_DELAY);

    releaseButton(buttonA);
    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B);
}

void testNotOnPressSimultaneousLong2(void) {
    pushButton(buttonA);
    pushButton(buttonB);
    pushButton(buttonC);
    delay(LONG_PRESS_DELAY);

    releaseButton(buttonA);
    releaseButton(buttonB);
    releaseButton(buttonC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B_C);
}

void testCombinations(void) {
    // TEST BUTTON A PRESS
    pushButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
    resetActions();

    // TEST BUTTON B PRESS
    pushButton(buttonB);
    delay(SMALL_DELAY);

    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(PRESS_B);
    resetActions();

    // TEST BUTTONS A_B SIMULTANEOUS PRESS
    pushButton(buttonA);
    delay(SMALL_DELAY);
    pushButton(buttonB);

    delay(LONG_PRESS_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B);
    resetActions();

    // TEST BUTTONS A_B SIMULTANEOUS LONG PRESS
    pushButton(buttonA);
    delay(SMALL_DELAY);
    pushButton(buttonB);

    delay(3 * SIMULTANEOUS_PRESS_DELAY); // should not influence

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B);
    resetActions();

    // TEST BUTTON C LONG PRESS
    pushButton(buttonC);
    delay(3 * LONG_PRESS_DELAY);

    releaseButton(buttonC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(LONG_PRESS_C, 3);
    resetActions();

    // TEST BUTTONS A_B_C SIMULTANEOUS LONG PRESS
    pushButton(buttonA);
    delay(SMALL_DELAY);
    pushButton(buttonB);
    pushButton(buttonC);

    delay(SIMULTANEOUS_PRESS_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonC);
    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B_C);
    resetActions();
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
    buttonsHandler.setSimultaneousBehaviorLong({buttonA, buttonB}, simultaneousLongPressAB, SIMULTANEOUS_PRESS_TIME);
    buttonsHandler.setSimultaneousBehavior({buttonA, buttonC}, simultaneousPressAC);
    buttonsHandler.setSimultaneousBehaviorLong({buttonA, buttonC}, simultaneousLongPressAC, SIMULTANEOUS_PRESS_TIME);
    buttonsHandler.setSimultaneousBehavior({buttonB, buttonC}, simultaneousPressBC);
    buttonsHandler.setSimultaneousBehaviorLong({buttonB, buttonC}, simultaneousLongPressBC, SIMULTANEOUS_PRESS_TIME);
    buttonsHandler.setSimultaneousBehavior({buttonA, buttonB, buttonC}, simultaneousPressABC);
    buttonsHandler.setSimultaneousBehaviorLong({buttonA, buttonB, buttonC}, simultaneousLongPressABC,
                                               SIMULTANEOUS_PRESS_TIME);

    buttonsHandler.pollOnce();

    UNITY_BEGIN();

    RUN_TEST(testOnPress);
    RUN_TEST(testOnPressLong);
    RUN_TEST(testOnPressLongMultiple);
    RUN_TEST(testOnPressSimultaneous);
    RUN_TEST(testOnPressSimultaneous2);
    RUN_TEST(testOnPressSimultaneous3);
    RUN_TEST(testOnPressSimultaneousLong);
    RUN_TEST(testOnPressSimultaneousLong2);
    RUN_TEST(testNotOnPressSimultaneousLong);
    RUN_TEST(testNotOnPressSimultaneousLong2);
    RUN_TEST(testCombinations);

    UNITY_END();

}
