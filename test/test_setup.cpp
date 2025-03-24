#include "_test_simple.h"
#include "_test_simultaneous.h"

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
    // Let's assume this combination is missing:
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

void tearDown() { /* This function is run after EACH TEST */ }