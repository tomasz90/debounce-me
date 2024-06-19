#include "ButtonsHandler.h"
#include <algorithm>

ButtonsHandler::ButtonsHandler(std::initializer_list<Button> buttons) : buttons(buttons) {
    std::for_each(this->buttons.begin(), this->buttons.end(), [this](Button &button) {
        buttonLastStartPressed[button] = 0;
        buttonWasLongPressed[button] = false;
    });
}

void ButtonsHandler::setDebounceTime(unsigned int time) {
    debounceTime = time;
}

void ButtonsHandler::setLongPressTime(unsigned int time) {
    longPressTime = time;
}

bool ButtonsHandler::pollState(Button &button) const {
    auto &s = button.state;
    bool stateChanged = false;
    bool currentRawState = button.invertedState == !digitalRead(button.pin);
    if (s.lastRawState != currentRawState) {
        bool moreThanDebounceTime = (unsigned long) (millis() - s.lastChangeTime) >= debounceTime;
        if (moreThanDebounceTime) {
            s.currentState = currentRawState;
            s.lastChangeTime = millis();
            stateChanged = true;
        }
    }
    s.lastRawState = currentRawState;
    return stateChanged;
}

void ButtonsHandler::resetState(Button &button) const {
    button.state.lastState = button.state.currentState;
}

bool ButtonsHandler::isPressed(Button &button) const {
    return button.state.lastState == PRESSED && button.state.currentState == PRESSED;
}

bool ButtonsHandler::wasPressed(Button &button) const {
    return button.state.lastState == PRESSED && button.state.currentState == RELEASED;
}

bool ButtonsHandler::wasReleased(Button &button) const {
    return button.state.lastState == RELEASED && button.state.currentState == PRESSED;
}

bool ButtonsHandler::isLongPressed(Button &button) const {
    auto lastStartPressed = buttonLastStartPressed.at(button);
    return millis() - lastStartPressed >= longPressTime && lastStartPressed != 0;
}

bool& ButtonsHandler::wasLongPressed(Button &button) {
    return buttonWasLongPressed.at(button);
}

void ButtonsHandler::poll() {
    std::for_each(buttons.begin(), buttons.end(), [this](Button &button) {
        pollState(button);
    });

    std::for_each(buttons.begin(), buttons.end(), [this](Button &button) {
        bool isLongPress = isLongPressed(button);
        bool& wasLongPress = wasLongPressed(button);

        if (wasLongPress) {
            if (wasPressed(button)) wasLongPress = false;
        } else if (wasReleased(button)) {
            buttonLastStartPressed[button] = millis();
        } else if (wasPressed(button) && (!isLongPress || !button.isLongPressSupported)) {
            buttonLastStartPressed[button] = 0;
            button.onPress();
        } else if (isPressed(button) && isLongPress && button.isLongPressSupported) {
            buttonLastStartPressed[button] = 0;
            button.onPressLong();
            wasLongPress = true;
        }
    });

    std::for_each(buttons.begin(), buttons.end(), [this](Button &button) {
        resetState(button);
    });
}
