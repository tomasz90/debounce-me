#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include "Button.h"

#if defined(USE_LEGACY)
#define LEGACY 1
#else
#define LEGACY 0
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#endif

#if defined(ESP32) || defined(NRF52840_XXAA) || defined(USE_POLL_ONCE)
#define IS_FREE_RTOS_SUPPORTED 1
#else
#define IS_FREE_RTOS_SUPPORTED 0
#endif

class ButtonsHandler {
public:
#if !LEGACY
    ButtonsHandler(std::initializer_list<Button*> buttons);
#else
    ButtonsHandler(Button **buttons, uint8_t numButtons);
#endif

    void setDebounceTime(unsigned int time);
    void setClickSimultaneous(std::set<Button*> _buttons, std::function<void()> behavior);
    void setClickSimultaneousLong(std::set<Button*> _buttons, std::function<void()> behavior, unsigned int longPressTime = 1000);

    void poll();

#if IS_FREE_RTOS_SUPPORTED
    void pollOnce(int pollInterval = 1);

    void pollStop();
#endif

private:
#if !LEGACY
    std::vector<Button*> buttons;
#else
    static const uint8_t MAX_BUTTONS = 10;
    Button *buttons[MAX_BUTTONS]; // Fixed-size array
    uint8_t numButtons;
#endif
    std::set<Button*> simultaneousButtons;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviors;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviorsLong;
    std::map<std::set<Button*>, int> simultaneousLongPressTimes;

    bool wasSimultaneousPress = false;
    unsigned int debounceTime = 20;

    std::map<Button*, unsigned long> buttonLastStartPressed;
    std::map<Button*, unsigned long> buttonLastClicked;
    std::map<Button*, bool> buttonWasLongPressed;

#if IS_FREE_RTOS_SUPPORTED
    TimerHandle_t timer;
#endif

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
    void onDoublePress(Button *button);
    void registerPress(Button *button);
    void onWasReleased(Button *button);
};


#endif //BUTTON_HANDLER_H