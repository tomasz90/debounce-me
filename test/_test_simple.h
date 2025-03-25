#include "test_config.h"
#include "test_utils.h"

#ifndef ARDUINO_ARCH_AVR

void testOnPress() {
    pushButton(btnA);
    delay(SHORT_PRESS_DELAY);

    releaseButton(btnA);

    assertAllEqual0Except(pressA);
}

void testOnPressLong() {
    pushButton(btnA);
    delay(LONG_PRESS_DELAY);

    releaseButton(btnA);

    assertAllEqual0Except(longPressA);
}

void testOnPressDouble() {
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);

    assertAllEqual0Except(doublePressA);
}

void testOnPressDouble2() {
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);

    assertAllEqual0Except(doublePressA);
    resetActions();

    pushButton(btnA);
    delay(SMALL_DELAY);
    releaseButton(btnA);

    assertAllEqual0Except(pressA);
}

void testOnPressLongMultiple() {
    pushButton(btnA);
    delay(3.5 * LONG_PRESS_TIME);

    releaseButton(btnA);

    assertAllEqual0Except(longPressA, 3);
}
#endif

