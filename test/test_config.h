#ifndef DEBOUNCE_ME_TEST_CONFIG_H
#define DEBOUNCE_ME_TEST_CONFIG_H

#include "../src/ButtonsHandler.h"
#include "../src/Button.h"

#define BUTTON_PIN_A 2
#define BUTTON_PIN_B 21
#define BUTTON_PIN_C 22
#define BUTTON_PIN_D 23

#define LONG_PRESS_TIME 150
#define DOUBLE_PRESS_TIME 75
#define SIMULTANEOUS_PRESS_TIME 200

#define SMALL_DELAY 25

#define SHORT_PRESS_DELAY (LONG_PRESS_TIME / 2)
#define LONG_PRESS_DELAY (LONG_PRESS_TIME * 1.5)

#define SHORT_SIMULTANEOUS_PRESS_DELAY (SIMULTANEOUS_PRESS_TIME / 2)
#define LONG_SIMULTANEOUS_PRESS_DELAY (SIMULTANEOUS_PRESS_TIME * 1.5)

#define DELAY_BETWEEN_TESTS 200

#define DEBOUNCE_TIME 1

extern Button *btnA;
extern Button *btnB;
extern Button *btnC;
extern Button *btnD;

extern ButtonsHandler btnHlr;
#endif //DEBOUNCE_ME_TEST_CONFIG_H
