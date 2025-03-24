#include <unity.h>
#include "../src/Button.h"
#include "../src/ButtonsHandler.h"
#include "testUtils.h"

#define BUTTON_PIN_A 2
#define BUTTON_PIN_B 21
#define BUTTON_PIN_C 22

#define SMALL_DELAY 20
#define LONG_PRESS_TIME 100
#define LONG_PRESS_DELAY (LONG_PRESS_TIME * 1.1)
#define DOUBLE_PRESS_TIME 50
#define SIMULTANEOUS_PRESS_TIME 200
#define SIMULTANEOUS_LONG_PRESS_DELAY (SIMULTANEOUS_PRESS_TIME * 1.1)

#if !LEGACY
Button *buttonA = new Button(BUTTON_PIN_A, IN_PULLUP);
Button *buttonB = new Button(BUTTON_PIN_B, IN_PULLUP);
Button *buttonC = new Button(BUTTON_PIN_C, IN_PULLUP);

ButtonsHandler buttonsHandler({buttonA, buttonB, buttonC});
#else
Button buttonA(BUTTON_PIN_A, IN_PULLUP);
Button buttonB(BUTTON_PIN_B, IN_PULLUP);
Button buttonC(BUTTON_PIN_C, IN_PULLUP);

Button *buttons[] = {&buttonA, &buttonB, &buttonC};
ButtonsHandler buttonsHandler(buttons, sizeof(buttons) / sizeof(buttons[0]));
#endif

// TO RUN TESTS: `pio test -vvv`
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
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
}

void testOnPressLong(void) {
    pushButton(buttonA);
    delay(LONG_PRESS_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(LONG_PRESS_A);
}

void testOnPressDouble(void) {
    pushButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    pushButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(DOUBLE_PRESS_A);
}

void testOnPressDouble2(void) {
    pushButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    pushButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(DOUBLE_PRESS_A);
    resetActions();

    pushButton(buttonA);
    delay(SMALL_DELAY);
    releaseButton(buttonA);
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
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
    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

    releaseButton(buttonA);
    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B);
}

void testOnPressSimultaneousLong2(void) {
    pushButton(buttonA);
    pushButton(buttonB);
    pushButton(buttonC);
    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

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
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
    resetActions();

    // TEST BUTTON B PRESS
    pushButton(buttonB);
    delay(SMALL_DELAY);

    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(PRESS_B);
    resetActions();

    // TEST BUTTONS A_C SIMULTANEOUS PRESS WHEN A_C IS NOT DEFINED
    pushButton(buttonA);
    delay(SMALL_DELAY);
    pushButton(buttonC);

    delay(LONG_PRESS_DELAY);

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_C, 0);
    resetActions();

    // TEST BUTTONS A_B SIMULTANEOUS LONG PRESS
    pushButton(buttonA);
    delay(SMALL_DELAY);
    pushButton(buttonB);

    delay(3 * SIMULTANEOUS_LONG_PRESS_DELAY); // should not influence

    releaseButton(buttonA);
    delay(SMALL_DELAY);

    releaseButton(buttonB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B);
    resetActions();

    // TEST BUTTON C PRESS
    pushButton(buttonC);
    delay(SMALL_DELAY);

    releaseButton(buttonC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(PRESS_C);
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

    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

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

#if !LEGACY
    buttonA->setClick(pressA);
    buttonB->setClick(pressB);
    buttonC->setClick(pressC);

    buttonA->setClickLong(longPressA, LONG_PRESS_TIME, true);
    buttonB->setClickLong(longPressB, LONG_PRESS_TIME, true);
    buttonC->setClickLong(longPressC, LONG_PRESS_TIME, true);

    buttonA->setClickDouble(doublePressA, DOUBLE_PRESS_TIME);

    buttonsHandler.setClickSimultaneous({buttonA, buttonB}, simultaneousPressAB);
    buttonsHandler.setClickSimultaneousLong({buttonA, buttonB}, simultaneousLongPressAB, SIMULTANEOUS_PRESS_TIME);
    // Lets assume this combination is missing:
    // buttonsHandler.setClickSimultaneous({buttonA, buttonC}, simultaneousPressAC);
    buttonsHandler.setClickSimultaneousLong({buttonA, buttonC}, simultaneousLongPressAC, SIMULTANEOUS_PRESS_TIME);
    buttonsHandler.setClickSimultaneous({buttonB, buttonC}, simultaneousPressBC);
    buttonsHandler.setClickSimultaneousLong({buttonB, buttonC}, simultaneousLongPressBC, SIMULTANEOUS_PRESS_TIME);
    buttonsHandler.setClickSimultaneous({buttonA, buttonB, buttonC}, simultaneousPressABC);
    buttonsHandler.setClickSimultaneousLong({buttonA, buttonB, buttonC}, simultaneousLongPressABC,
                                            SIMULTANEOUS_PRESS_TIME);
#else
    buttonA.setClick(pressA);
    buttonB.setClick(pressB);
    buttonC.setClick(pressC);

    buttonA.setClickLong(longPressA, LONG_PRESS_TIME, true);
    buttonB.setClickLong(longPressB, LONG_PRESS_TIME, true);
    buttonC.setClickLong(longPressC, LONG_PRESS_TIME, true);

    buttonA.setClickDouble(doublePressA, DOUBLE_PRESS_TIME);

    Button *buttonsAB[] = {&buttonA, &buttonB};
    Button *buttonsAC[] = {&buttonA, &buttonC};
    Button *buttonsBC[] = {&buttonB, &buttonC};

    buttonsHandler.setClickSimultaneous(buttonsAB, 2, simultaneousPressAB);
    buttonsHandler.setClickSimultaneousLong(buttonsAB, 2, simultaneousLongPressAB, SIMULTANEOUS_PRESS_TIME);
    // Lets assume this combination is missing:
    // buttonsHandler.setClickSimultaneous({buttonA, buttonC}, simultaneousPressAC);
    buttonsHandler.setClickSimultaneousLong(buttonsAC, 2, simultaneousLongPressAC, SIMULTANEOUS_PRESS_TIME);
    buttonsHandler.setClickSimultaneous(buttonsBC, 2, simultaneousPressBC);
    buttonsHandler.setClickSimultaneousLong(buttonsBC, 2, simultaneousLongPressBC, SIMULTANEOUS_PRESS_TIME);
    buttonsHandler.setClickSimultaneous(buttons, 3, simultaneousPressABC);
    buttonsHandler.setClickSimultaneousLong(buttons, 3, simultaneousLongPressABC, SIMULTANEOUS_PRESS_TIME);
#endif

    buttonsHandler.pollOnce();

    // this is right place, it overwrites previously sets in method setClick...
    pinMode(BUTTON_PIN_A, OUTPUT);
    pinMode(BUTTON_PIN_B, OUTPUT);
    pinMode(BUTTON_PIN_C, OUTPUT);

    UNITY_BEGIN();

    RUN_TEST(testOnPress);
    RUN_TEST(testOnPressLong);
    RUN_TEST(testOnPressDouble);
    RUN_TEST(testOnPressDouble2);
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
