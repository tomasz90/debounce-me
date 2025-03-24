#include "test_config.h"
#include "test_utils.h"

void testOnPressSimultaneous() {
    pushButton(btnA);
    pushButton(btnB);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(simultaneousPressAB);
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

    assertAllEqual0Except(simultaneousPressABC);
}

void testOnPressSimultaneousNoneMatch() {
    pushButton(btnA);
    pushButton(btnD);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    releaseButton(btnD);
    delay(SMALL_DELAY);

    assertAllEqual0();
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

    assertAllEqual0Except(simultaneousPressAB);
}

void testOnPressSimultaneousLong() {
    pushButton(btnA);
    pushButton(btnB);
    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(simultaneousLongPressAB);
}

void testOnPressSimultaneous4() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(SIMULTANEOUS_LONG_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(simultaneousLongPressABC);
}

void testNotOnPressSimultaneous5() {
    pushButton(btnA);
    pushButton(btnB);
    delay(SIMULTANEOUS_SHORT_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(simultaneousPressAB);
}

void testNotOnPressSimultaneous6() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    delay(SIMULTANEOUS_SHORT_PRESS_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(simultaneousPressABC);
}

void testCombinations() {
    // TEST BUTTON A PRESS
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(pressA);
    resetActions();

    // TEST BUTTON B PRESS
    pushButton(btnB);
    delay(SMALL_DELAY);

    releaseButton(btnB);
    delay(SMALL_DELAY);

    assertAllEqual0Except(pressB);
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

    assertAllEqual0Except(simultaneousPressAC, 0);
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

    assertAllEqual0Except(simultaneousLongPressAB);
    resetActions();

    // TEST BUTTON C PRESS
    pushButton(btnC);
    delay(SMALL_DELAY);

    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(pressC);
    resetActions();

    // TEST BUTTON C LONG PRESS
    pushButton(btnC);
    delay(3 * LONG_PRESS_DELAY);

    releaseButton(btnC);
    delay(SMALL_DELAY);

    assertAllEqual0Except(longPressC, 3);
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

    assertAllEqual0Except(simultaneousLongPressABC);
    resetActions();
}

void testOnPressSimultaneousAll() {
    pushButton(btnA);
    pushButton(btnB);
    pushButton(btnC);
    pushButton(btnD);

    delay(SMALL_DELAY);

    releaseButton(btnA);
    releaseButton(btnB);
    releaseButton(btnC);
    releaseButton(btnD);

    delay(SMALL_DELAY);

    assertAllEqual0Except(simultaneousPressABCD);
}

