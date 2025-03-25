#include "test_config.h"
#include "test_utils.h"

void testOnPressSimultaneous() {
    pushButton(btnA);
    pushButton(btnB);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);

    assertAllEqual0Except(simultaneousPressAB);
}

void testOnPressSimultaneous2() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);

    assertAllEqual0Except(simultaneousPressABC);
}

void testOnPressSimultaneousNoneMatch() {
    pushButton(btnA);
    pushButton(btnD);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnD);

    assertAllEqual0();
}

void testOnPressSimultaneous3() {
    pushButton(btnA);
    delay(SHORT_PRESS_DELAY);

    pushButton(btnB);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnB);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnA);

    assertAllEqual0Except(simultaneousPressAB);
}

void testOnPressSimultaneousLong() {
    pushButton(btnA);
    pushButton(btnB);
    delay(LONG_SIMULTANEOUS_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);

    assertAllEqual0Except(simultaneousLongPressAB);
}

void testOnPressSimultaneous4() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(LONG_SIMULTANEOUS_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);

    assertAllEqual0Except(simultaneousLongPressABC);
}

void testNotOnPressSimultaneous5() {
    pushButton(btnA);
    pushButton(btnB);
    delay(SHORT_SIMULTANEOUS_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);

    assertAllEqual0Except(simultaneousPressAB);
}

void testNotOnPressSimultaneous6() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(SHORT_SIMULTANEOUS_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);

    assertAllEqual0Except(simultaneousPressABC);
}

void testCombinations() {
    // TEST BUTTON A PRESS
    pushButton(btnA);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnA);

    assertAllEqual0Except(pressA);
    resetActions();

    // TEST BUTTON B PRESS
    pushButton(btnB);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnB);

    assertAllEqual0Except(pressB);
    resetActions();

    // TEST BUTTONS A_C SIMULTANEOUS PRESS WHEN A_C IS NOT DEFINED
    pushButton(btnA);
    delay(SMALL_DELAY);
    pushButton(btnC);

    delay(SHORT_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnC);

    assertAllEqual0Except(simultaneousPressAC, 0);
    resetActions();

    // TEST BUTTONS A_B SIMULTANEOUS LONG PRESS
    pushButton(btnA);
    delay(SMALL_DELAY);
    pushButton(btnB);

    delay(3 * LONG_SIMULTANEOUS_PRESS_DELAY); // should not influence

    releaseButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnB);

    assertAllEqual0Except(simultaneousLongPressAB);
    resetActions();

    // TEST BUTTON C PRESS
    pushButton(btnC);
    delay(SMALL_DELAY);

    releaseButton(btnC);

    assertAllEqual0Except(pressC);
    resetActions();

    // TEST BUTTON C LONG PRESS
    pushButton(btnC);
    delay(3.5 * LONG_PRESS_TIME);

    releaseButton(btnC);

    assertAllEqual0Except(longPressC, 3);
    resetActions();

    // TEST BUTTONS A_B_C SIMULTANEOUS LONG PRESS
    pushButton(btnA);
    delay(SMALL_DELAY);
    pushButton(btnB);
    pushButton(btnC);

    delay(LONG_SIMULTANEOUS_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnC);
    releaseButton(btnB);

    assertAllEqual0Except(simultaneousLongPressABC);
    resetActions();
}

void testOnPressSimultaneousAll() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    pushButton(btnD);

    delay(SHORT_SIMULTANEOUS_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    releaseButton(btnD);

    assertAllEqual0Except(simultaneousPressABCD);
}

