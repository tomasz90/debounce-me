#ifndef DEBOUNCE_ME_TEST_CONFIG_H
#define DEBOUNCE_ME_TEST_CONFIG_H

#include "../src/ButtonsHandler.h"
#include "../src/Button.h"

#define BUTTON_PIN_A 2
#define BUTTON_PIN_B 21
#define BUTTON_PIN_C 22

#define SMALL_DELAY 20
#define LONG_PRESS_TIME 100
#define LONG_PRESS_DELAY (LONG_PRESS_TIME * 1.1)
#define DOUBLE_PRESS_TIME 50
#define SIMULTANEOUS_PRESS_TIME 200
#define SIMULTANEOUS_LONG_PRESS_DELAY (SIMULTANEOUS_PRESS_TIME * 1.1)

extern Button *btnA;
extern Button *btnB;
extern Button *btnC;

extern ButtonsHandler btnHlr;
#endif //DEBOUNCE_ME_TEST_CONFIG_H
