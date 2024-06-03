#include <Arduino.h>
#include <vector>
#include <map>
#include "Button.h"

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

class ButtonsHandler {

public:
    explicit ButtonsHandler(std::initializer_list<Button> buttons);
    void setDebounceTime(unsigned int time);
    void setLongPressTime(unsigned int time);
    void poll();

private:
    std::vector<Button> buttons;
    unsigned int debounceTime = 20;
    unsigned int longPressTime = 1000;

    std::map<Button, unsigned long> buttonLastStartPressed;
    std::map<Button, bool> buttonWasLongPressed;

    bool pollState(Button &button) const;
    void resetState(Button &button) const;
    bool isPressed(Button &button) const;
    bool wasPressed(Button &button) const;
    bool wasReleased(Button &button) const;
    bool isLongPressed(Button &button) const;
    bool& wasLongPressed(Button &button);
};


#endif //BUTTON_HANDLER_H