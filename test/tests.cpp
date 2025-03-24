#include "../src/ButtonsHandler.h"
#include "testUtils.h"
#include "helper.h"

#define BUTTON_PIN_A 2
#define BUTTON_PIN_B 21
#define BUTTON_PIN_C 22

#define SMALL_DELAY 20
#define LONG_PRESS_TIME 100
#define LONG_PRESS_DELAY (LONG_PRESS_TIME * 1.1)
#define DOUBLE_PRESS_TIME 50
#define SIMULTANEOUS_PRESS_TIME 200
#define SIMULTANEOUS_LONG_PRESS_DELAY (SIMULTANEOUS_PRESS_TIME * 1.1)

Button *btnA = new Button(BUTTON_PIN_A, IN_PULLUP);
Button *btnB = new Button(BUTTON_PIN_B, IN_PULLUP);
Button *btnC = new Button(BUTTON_PIN_C, IN_PULLUP);

ButtonsHandler btnHlr({btnA, btnB, btnC});

void setup() {
    Serial.begin(115200);

    btnA->setClick(pressA);
    btnB->setClick(pressB);
    btnC->setClick(pressC);

    btnA->setClickLong(longPressA, LONG_PRESS_TIME, true);
    btnB->setClickLong(longPressB, LONG_PRESS_TIME, true);
    btnC->setClickLong(longPressC, LONG_PRESS_TIME, true);

    btnA->setClickDouble(doublePressA, DOUBLE_PRESS_TIME);

    // this is right place, it overwrites previously sets in method setClick...
    pinMode(BUTTON_PIN_A, OUTPUT);
    pinMode(BUTTON_PIN_B, OUTPUT);
    pinMode(BUTTON_PIN_C, OUTPUT);

    btnHlr.setClickSimultaneous({btnA, btnB}, simultaneousPressAB);
    btnHlr.setClickSimultaneousLong({btnA, btnB}, simultaneousLongPressAB, SIMULTANEOUS_PRESS_TIME);
    // Lets assume this combination is missing:
    // buttonsHandler.setClickSimultaneous({buttonA, buttonC}, simultaneousPressAC);
    btnHlr.setClickSimultaneousLong({btnA, btnC}, simultaneousLongPressAC, SIMULTANEOUS_PRESS_TIME);
    btnHlr.setClickSimultaneous({btnB, btnC}, simultaneousPressBC);
    btnHlr.setClickSimultaneousLong({btnB, btnC}, simultaneousLongPressBC, SIMULTANEOUS_PRESS_TIME);
    btnHlr.setClickSimultaneous({btnA, btnB, btnC}, simultaneousPressABC);
    btnHlr.setClickSimultaneousLong({btnA, btnB, btnC}, simultaneousLongPressABC, SIMULTANEOUS_PRESS_TIME);

    btnHlr.pollOnce();

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

// TO RUN TESTS: `pio test -vvv`
void setUp() {
    resetActions();

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
}

void tearDown() {
    // This function is run after EACH TEST
}

void testOnPress() {
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
}

void testOnPressLong() {
    pushButton(btnA);
    delay(LONG_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(LONG_PRESS_A);
}

void testOnPressDouble() {
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(DOUBLE_PRESS_A);
}

void testOnPressDouble2() {
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(DOUBLE_PRESS_A);
    resetActions();

    pushButton(btnA);
    delay(SMALL_DELAY);
    releaseButton(btnA);
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
}

void testOnPressLongMultiple() {
    pushButton(btnA);
    delay(3 * LONG_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(LONG_PRESS_A, 3);
}

void testOnPressSimultaneous() {
    pushButton(btnA);
    pushButton(btnB);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B);
}

void testOnPressSimultaneous2() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B_C);
}

void testOnPressSimultaneous3() {
    pushButton(btnA);
    delay(SMALL_DELAY);

    pushButton(btnB);
    delay(SMALL_DELAY);

    releaseButton(btnB);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B);
}

void testOnPressSimultaneousLong() {
    pushButton(btnA);
    pushButton(btnB);
    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B);
}

void testOnPressSimultaneousLong2() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B_C);
}

void testNotOnPressSimultaneousLong() {
    pushButton(btnA);
    pushButton(btnB);
    delay(LONG_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B);
}

void testNotOnPressSimultaneousLong2() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(LONG_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_B_C);
}

void testCombinations() {
    // TEST BUTTON A PRESS
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(PRESS_A);
    resetActions();

    // TEST BUTTON B PRESS
    pushButton(btnB);
    delay(SMALL_DELAY);

    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(PRESS_B);
    resetActions();

    // TEST BUTTONS A_C SIMULTANEOUS PRESS WHEN A_C IS NOT DEFINED
    pushButton(btnA);
    delay(SMALL_DELAY);
    pushButton(btnC);

    delay(LONG_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_PRESS_A_C, 0);
    resetActions();

    // TEST BUTTONS A_B SIMULTANEOUS LONG PRESS
    pushButton(btnA);
    delay(SMALL_DELAY);
    pushButton(btnB);

    delay(3 * SIMULTANEOUS_LONG_PRESS_DELAY); // should not influence

    releaseButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B);
    resetActions();

    // TEST BUTTON C PRESS
    pushButton(btnC);
    delay(SMALL_DELAY);

    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(PRESS_C);
    resetActions();

    // TEST BUTTON C LONG PRESS
    pushButton(btnC);
    delay(3 * LONG_PRESS_DELAY);

    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(LONG_PRESS_C, 3);
    resetActions();

    // TEST BUTTONS A_B_C SIMULTANEOUS LONG PRESS
    pushButton(btnA);
    delay(SMALL_DELAY);
    pushButton(btnB);
    pushButton(btnC);

    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnC);
    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(SIMULTANEOUS_LONG_PRESS_A_B_C);
    resetActions();
}
