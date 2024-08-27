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
    void setSimultaneousClick(std::set<Button*> _buttons, std::function<void()> behavior);
    void setSimultaneousClickLong(std::set<Button*> _buttons, std::function<void()> behavior, unsigned int longPressTime = 1000);

    void poll();
    void pollOnce(int pollInterval = 1);
    void stopPolling();

private:
    std::vector<Button*> buttons;

    std::set<Button*> simultaneousButtons;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviors;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviorsLong;
    std::map<std::set<Button*>, int> simultaneousLongPressTimes;

    bool wasSimultaneousPress = false;
    unsigned int debounceTime = 20;

    std::map<Button*, unsigned long> buttonLastStartPressed;
    std::map<Button*, unsigned long> buttonLastClicked;
    std::map<Button*, bool> buttonWasLongPressed;

    TimerHandle_t timer;

    void pollState(Button *button) const;
    void resetState(Button *button) const;

    bool isPressed(Button *button) const;
    bool wasPressed(Button *button) const;
    bool wasReleased(Button *button) const;

    bool isLongPressed(Button *button) const;
    bool isSimultaneousLongPressed(Button *button);
    bool& wasLongPressed(Button *button);
    bool isOneButtonPressed() const;

    void processButtonState(Button *button);

    void onSimultaneousPressLong();
    void onPressLong(Button *button);
    void onSimultaneousPress(Button *button);
    void onPress(Button *button);
    void onPress2(Button *button);
    void onWasReleased(Button *button);
};


#endif //BUTTON_HANDLER_H