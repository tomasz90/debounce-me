#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include <ArxContainer.h>
#include "Button.h"

#if defined(INC_FREERTOS_H) || defined(USE_POLL_ONCE)
#define IS_FREE_RTOS_SUPPORTED 1
#else
#define IS_FREE_RTOS_SUPPORTED 0
#endif

class ButtonsHandler {
public:
    ButtonsHandler(std::initializer_list<Button*> buttons);

    void setClickSimultaneous(std::set<Button*> _buttons, std::function<void()> behavior);
    void setClickSimultaneousLong(std::set<Button*> _buttons, std::function<void()> behavior, unsigned int longPressTime = 1000);
    void setDebounceTime(unsigned int time);

    void poll();

#if IS_FREE_RTOS_SUPPORTED
    void pollOnce(int pollInterval = 1);
    void pollStop();
private:
    TimerHandle_t timer;
#endif

private:
    struct ButtonTemporaryProperties {
        unsigned long buttonLastStartPressed;
        unsigned long buttonLastClicked;
        bool buttonWasLongPressed;
    };

    struct ButtonSimultaneousProperties {
        std::function<void()> behavior;
        std::function<void()> behaviorLong;
        int longPressTime = -1;
    };

    unsigned int debounceTime = 20;
    bool wasSimultaneousPress = false;

    std::vector<Button*> buttons;
    std::map<Button*, ButtonTemporaryProperties> buttonTemporary;
    std::map<std::set<Button*>, ButtonSimultaneousProperties> simultaneousBehaviors;
    std::set<Button*> currentlyPressed;

    void pollState(Button *button) const;
    void resetState(Button *button) const;

    bool isPressed(Button *button) const;
    bool wasPressed(Button *button) const;
    bool wasReleased(Button *button) const;

    bool isLongPressed(Button *button);
    bool isSimultaneousLongPressed(Button *button);
    bool& wasLongPressed(Button *button);
    bool areMultipleButtonsPressed() const;

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