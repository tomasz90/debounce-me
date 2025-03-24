#ifndef DEBOUNCE_ME_TEST_CONFIG_H
#define DEBOUNCE_ME_TEST_CONFIG_H

#include "../src/ButtonsHandler.h"
#include "../src/Button.h"

#define BUTTON_PIN_A 2
#define BUTTON_PIN_B 21
#define BUTTON_PIN_C 22
#define BUTTON_PIN_D 23

#define LONG_PRESS_TIME 100
#define DOUBLE_PRESS_TIME 50
#define SIMULTANEOUS_PRESS_TIME 200

#define SMALL_DELAY 20

#define SHORT_PRESS_DELAY (LONG_PRESS_TIME - SMALL_DELAY)
#define LONG_PRESS_DELAY (LONG_PRESS_TIME + SMALL_DELAY)

#define SHORT_SIMULTANEOUS_PRESS_DELAY (SIMULTANEOUS_PRESS_TIME - SMALL_DELAY)
#define LONG_SIMULTANEOUS_PRESS_DELAY (SIMULTANEOUS_PRESS_TIME + SMALL_DELAY)

#define DELAY_BETWEEN_TESTS 150

#define DEBOUNCE_TIME 2

extern Button *btnA;
extern Button *btnB;
extern Button *btnC;
extern Button *btnD;

extern ButtonsHandler btnHlr;
#endif //DEBOUNCE_ME_TEST_CONFIG_H
