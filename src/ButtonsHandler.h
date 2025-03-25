#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include "Button.h"
#include <vector>
#include <map>
//#include <set>
#include <Set.h>
#include <algorithm>

#if defined(ESP32) || defined(NRF52840_XXAA) || defined(USE_POLL_ONCE)
#define IS_FREE_RTOS_SUPPORTED 1
#else
#define IS_FREE_RTOS_SUPPORTED 0
#endif

class ButtonsHandler {
public:
    ButtonsHandler(std::initializer_list<Button *> buttons);

    void setClickSimultaneous(std::set<Button *> _buttons, std::function<void()> behavior);

    void setClickSimultaneousLong(std::set<Button *> _buttons, std::function<void()> behavior,
                                  unsigned int longPressTime = 1000);

    void setDebounceTime(unsigned int time);

    void poll();

#if IS_FREE_RTOS_SUPPORTED

    void pollOnce(int pollInterval = 1);

    void pollStop();

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
        unsigned int longPressTime = -1;
    };

    struct BtnState {
        bool _isPressed;
        bool isSimultaneousLongPress;
        bool isLongPress;
        bool _wasPressed;
        bool _areMultipleButtonsPressed;
        bool isDoublePressSupported;
        bool isRegisteredPress;
        bool isElapsedTime;
        bool _wasReleased;

        bool operator==(const BtnState &other) const {
            return _isPressed == other._isPressed &&
                   isSimultaneousLongPress == other.isSimultaneousLongPress &&
                   isLongPress == other.isLongPress &&
                   _wasPressed == other._wasPressed &&
                   _areMultipleButtonsPressed == other._areMultipleButtonsPressed &&
                   isDoublePressSupported == other.isDoublePressSupported &&
                   isRegisteredPress == other.isRegisteredPress &&
                   isElapsedTime == other.isElapsedTime &&
                   _wasReleased == other._wasReleased;
        }

        bool operator!=(const BtnState &other) const {
            return !(*this == other);
        }

        void log(int btnNumber) const {
            Serial.println("-------------------------------------------------------");
            Serial.println(String(btnNumber) + "._isPressed:                 " + String(_isPressed));
            Serial.println(String(btnNumber) + ".isSimultaneousLongPress:    " + String(isSimultaneousLongPress));
            Serial.println(String(btnNumber) + ".isLongPress:                " + String(isLongPress));
            Serial.println(String(btnNumber) + "._wasPressed:                " + String(_wasPressed));
            Serial.println(String(btnNumber) + "._areMultipleButtonsPressed: " + String(_areMultipleButtonsPressed));
            Serial.println(String(btnNumber) + ".isDoublePressSupported:     " + String(isDoublePressSupported));
            Serial.println(String(btnNumber) + ".isRegisteredPress:          " + String(isRegisteredPress));
            Serial.println(String(btnNumber) + ".isElapsedTime:              " + String(isElapsedTime));
            Serial.println(String(btnNumber) + "._wasReleased:               " + String(_wasReleased));
        }
    };

    unsigned int debounceTime = 20;
    bool wasSimultaneousPress = false;

    std::map<Button *, BtnState> btnStates;
    std::map<Button *, BtnState> lastBtnStates;
    std::vector<Button *> buttons;
    std::map<Button *, ButtonTemporaryProperties> buttonTemporary;
    std::map<std::set<Button *>, ButtonSimultaneousProperties> simultaneousBehaviors;
    std::set<Button *> currentlyPressed;

#if IS_FREE_RTOS_SUPPORTED
    TimerHandle_t timer;
#endif

    void pollState(Button *button) const;

    void resetState(Button *button) const;

    bool isPressed(Button *button) const;

    bool wasPressed(Button *button) const;

    bool wasReleased(Button *button) const;

    bool isLongPressed(Button *button);

    bool isSimultaneousLongPressed(Button *button);

    bool &wasLongPressed(Button *button);

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