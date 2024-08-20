#include "ButtonsHandler.h"

ButtonsHandler::ButtonsHandler(std::initializer_list<Button*> buttons) : buttons(buttons) {
    std::for_each(this->buttons.begin(), this->buttons.end(), [this](Button *button) {
        buttonLastStartPressed[button] = 0;
        buttonWasLongPressed[button] = false;
    });
}

void ButtonsHandler::setDebounceTime(unsigned int time) {
    debounceTime = time;
}

void ButtonsHandler::pollState(Button *button) const {
    auto &s = button->state;
    bool currentRawState = button->invertedState == !digitalRead(button->pin);
    if (s.lastRawState != currentRawState) {
        bool moreThanDebounceTime = (unsigned long) (millis() - s.lastChangeTime) >= debounceTime;
        if (moreThanDebounceTime) {
            s.currentState = currentRawState;
            s.lastChangeTime = millis();
        }
    }
    s.lastRawState = currentRawState;
}

void ButtonsHandler::resetState(Button *button) const {
    button->state.lastState = button->state.currentState;
}

bool ButtonsHandler::isPressed(Button *button) const {
    return button->state.lastState == PRESSED && button->state.currentState == PRESSED;
}

bool ButtonsHandler::wasPressed(Button *button) const {
    return button->state.lastState == PRESSED && button->state.currentState == RELEASED;
}

bool ButtonsHandler::wasReleased(Button *button) const {
    return button->state.lastState == RELEASED && button->state.currentState == PRESSED;
}

bool ButtonsHandler::isLongPressed(Button *button) const {
    auto lastStartPressed = buttonLastStartPressed.at(button);
    return millis() - lastStartPressed >= button->longPressTime && lastStartPressed != 0;
}

bool &ButtonsHandler::wasLongPressed(Button *button) {
    return buttonWasLongPressed.at(button);
}

void ButtonsHandler::poll() {
    std::for_each(buttons.begin(), buttons.end(), [this](Button *button) {
        pollState(button);
    });

    std::for_each(buttons.begin(), buttons.end(), [this](Button *button) {
        bool isLongPress = isLongPressed(button) && button->isLongPressSupported;
        bool &wasLongPress = wasLongPressed(button);

        if (isPressed(button) && isLongPress && !wasBothPress) {
            if(simultaneousButtons.size() < 2) {
                button->onPressLong();
                wasLongPress = true;
            } else {
                wasBothPress = true;
                auto behavior = simultaneousBehaviorsLong[simultaneousButtons];
                if(behavior) {
                    behavior();
                }
            }
            buttonLastStartPressed[button] = button->isMultipleLongPressSupported ? millis() : 0;
            simultaneousButtons.clear();
        } else if (wasPressed(button) && !isLongPress && !wasLongPress && !wasBothPress) {
            if(simultaneousButtons.size() < 2) {
                button->onPress();
            } else {
                wasBothPress = true;
                auto behavior = simultaneousBehaviors[simultaneousButtons];
                if(behavior) {
                    behavior();
                }
            }
            buttonLastStartPressed[button] = 0;
            simultaneousButtons.clear();
        } else if (wasReleased(button)) {
            buttonLastStartPressed[button] = millis();
            simultaneousButtons.insert(button);
            wasLongPress = false;
            wasBothPress = false;
        }
    });

    std::for_each(buttons.begin(), buttons.end(), [this](Button *button) {
        resetState(button);
    });
}
