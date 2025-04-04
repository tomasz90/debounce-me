#include "_test_simple.h"
#include "_test_simultaneous.h"

#ifndef ARDUINO_ARCH_AVR

Button *btnA = new Button(BUTTON_PIN_A, IN_PULLUP);
Button *btnB = new Button(BUTTON_PIN_B, IN_PULLUP);
Button *btnC = new Button(BUTTON_PIN_C, IN_PULLUP);
Button *btnD = new Button(BUTTON_PIN_D, IN_PULLUP);

ButtonsHandler btnHlr({btnA, btnB, btnC, btnD});

void setup() {
    Serial.begin(BAUD_RATE);

    btnA->setClick(pressA);
    btnB->setClick(pressB);
    btnC->setClick(pressC);
    btnD->setClick([] {});

    btnA->setClickLong(longPressA, LONG_PRESS_TIME, true);
    btnB->setClickLong(longPressB, LONG_PRESS_TIME, true);
    btnC->setClickLong(longPressC, LONG_PRESS_TIME, true);

    btnA->setClickDouble(doublePressA, DOUBLE_PRESS_TIME);

    // this is right place, it overwrites previously sets in method setClick...
    pinMode(BUTTON_PIN_A, OUTPUT);
    pinMode(BUTTON_PIN_B, OUTPUT);
    pinMode(BUTTON_PIN_C, OUTPUT);
    pinMode(BUTTON_PIN_D, OUTPUT);

    btnHlr.setClickSimultaneous({btnA, btnB}, simultaneousPressAB);
    btnHlr.setClickSimultaneousLong({btnA, btnB}, simultaneousLongPressAB, SIMULTANEOUS_LONG_PRESS_TIME);
    // Let's assume this combination is missing:
    // buttonsHandler.setClickSimultaneous({buttonA, buttonC}, simultaneousPressAC);
    btnHlr.setClickSimultaneous({btnB, btnC}, simultaneousPressBC);
    btnHlr.setClickSimultaneous({btnA, btnB, btnC}, simultaneousPressABC);
    btnHlr.setClickSimultaneous({btnA, btnB, btnC, btnD}, simultaneousPressABCD);

    btnHlr.setClickSimultaneousLong({btnA, btnC}, simultaneousLongPressAC, SIMULTANEOUS_LONG_PRESS_TIME);
    btnHlr.setClickSimultaneousLong({btnB, btnC}, simultaneousLongPressBC, SIMULTANEOUS_LONG_PRESS_TIME);
    btnHlr.setClickSimultaneousLong({btnA, btnB, btnC}, simultaneousLongPressABC, SIMULTANEOUS_LONG_PRESS_TIME);

    btnHlr.setDebounceTime(DEBOUNCE_TIME);
    btnHlr.pollOnce(POLL_INTERVAL);

    UNITY_BEGIN();

    RUN_TEST(testOnPress);
    RUN_TEST(testOnPressLong);
    RUN_TEST(testOnPressDouble);
    RUN_TEST(testOnPressDouble2);
    RUN_TEST(testOnPressLongMultiple);
    RUN_TEST(testOnPressSimultaneous);
    RUN_TEST(testOnPressSimultaneous2);
    RUN_TEST(testOnPressSimultaneousNoneMatch);
    RUN_TEST(testOnPressSimultaneous3);
    RUN_TEST(testOnPressSimultaneousLong);
    RUN_TEST(testOnPressSimultaneous4);
    RUN_TEST(testNotOnPressSimultaneous5);
    RUN_TEST(testNotOnPressSimultaneous6);
    RUN_TEST(testCombinations);
    RUN_TEST(testOnPressSimultaneousAll);

    UNITY_END();

}

// TO RUN TESTS: `pio test -vvv`
void setUp() {
    resetActions();

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    releaseButton(btnD);
}

void tearDown() {
    delay(DELAY_BETWEEN_TESTS);
}
#endif