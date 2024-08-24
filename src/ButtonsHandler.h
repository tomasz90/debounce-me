#include <Arduino.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "Button.h"

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

class ButtonsHandler {
public:
    ButtonsHandler(std::initializer_list<Button*> buttons);

    void setDebounceTime(unsigned int time);
    void setSimultaneousBehavior(std::set<Button*> _buttons, std::function<void()> behavior);
    void setSimultaneousBehaviorLong(std::set<Button*> _buttons, std::function<void()> behavior);

    void pollOnce(int pollInterval = 1);
    void poll();

private:
    std::vector<Button*> buttons;

    std::set<Button*> simultaneousButtons;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviors;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviorsLong;

    bool wasSimultaneousPress = false;
    unsigned int debounceTime = 20;

    std::map<Button*, unsigned long> buttonLastStartPressed;
    std::map<Button*, bool> buttonWasLongPressed;

    void pollState(Button *button) const;
    void resetState(Button *button) const;

    bool isPressed(Button *button) const;
    bool wasPressed(Button *button) const;
    bool wasReleased(Button *button) const;
    bool isLongPressed(Button *button) const;
    bool& wasLongPressed(Button *button);

    bool isOneButtonPressed() const;
    void simultaneousOnPress();
    void simultaneousOnPressLong();
};


#endif //BUTTON_HANDLER_H