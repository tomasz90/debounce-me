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

    void setClickSimultaneous(std::set<Button*> _buttons, std::function<void()> behavior);
    void setClickSimultaneousLong(std::set<Button*> _buttons, std::function<void()> behavior, unsigned int longPressTime = 1000);
#else
    ButtonsHandler(Button **buttons, uint8_t numButtons);

    void setClickSimultaneous(Button** buttons, uint8_t count, void (*behavior)());
    void setClickSimultaneousLong(Button** buttons, uint8_t count, void (*behavior)(), unsigned int longPressTime = 1000);
#endif

    void setDebounceTime(unsigned int time);

    void poll();

#if IS_FREE_RTOS_SUPPORTED
    void pollOnce(int pollInterval = 1);

    void pollStop();
#endif

private:
    unsigned int debounceTime = 20;
    bool wasSimultaneousPress = false;

#if !LEGACY
    std::vector<Button*> buttons;

    std::set<Button*> simultaneousButtons;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviors;
    std::map<std::set<Button*>, std::function<void()>> simultaneousBehaviorsLong;
    std::map<std::set<Button*>, int> simultaneousLongPressTimes;

    std::map<Button*, unsigned long> buttonLastStartPressed;
    std::map<Button*, unsigned long> buttonLastClicked;
    std::map<Button*, bool> buttonWasLongPressed;
#else
    static const uint8_t MAX_BUTTONS = 10;
    static const uint8_t MAX_GROUPS = 5;

    Button* buttons[MAX_BUTTONS];
    uint8_t numButtons;

    struct ButtonGroup {
        Button* buttons[MAX_BUTTONS];
        uint8_t count;
        void (*behavior)();
        unsigned int longPressTime;
    };

    ButtonGroup simultaneousGroups[MAX_GROUPS];
    ButtonGroup simultaneousLongGroups[MAX_GROUPS];
    uint8_t numSimultaneous = 0;
    uint8_t numSimultaneousLong = 0;

    unsigned long buttonLastStartPressed[MAX_BUTTONS];
    unsigned long buttonLastClicked[MAX_BUTTONS];
    bool buttonWasLongPressed[MAX_BUTTONS];
#endif

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

#if LEGACY
    uint8_t getButtonIndex(Button* button) const;
    bool checkGroupPressed(const ButtonGroup& group) const;
    bool checkGroupLongPressed(const ButtonGroup& group) const;
    bool checkNoOtherPressed(const ButtonGroup& group) const;
#endif
};


#endif //BUTTON_HANDLER_H