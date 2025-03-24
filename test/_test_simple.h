#include "test_config.h"
#include "test_utils.h"

void testOnPress() {
    pushButton(btnA);
    delay(SMALL_DELAY);

    releaseButton(btnA);
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(pressA);
}

void testOnPressLong() {
    pushButton(btnA);
    delay(LONG_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

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
    delay(SMALL_DELAY);

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
    delay(SMALL_DELAY);

    assertAllEqual0Except(doublePressA);
    resetActions();

    pushButton(btnA);
    delay(SMALL_DELAY);
    releaseButton(btnA);
    delay(DOUBLE_PRESS_TIME + SMALL_DELAY);

    assertAllEqual0Except(pressA);
}

void testOnPressLongMultiple() {
    pushButton(btnA);
    delay(3 * LONG_PRESS_DELAY);

    releaseButton(btnA);
    delay(SMALL_DELAY);

    assertAllEqual0Except(longPressA, 3);
}

