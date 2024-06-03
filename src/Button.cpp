#include "Button.h"

Button::Button(byte buttonPin,
               Mode buttonMode,
               std::function<void()> buttonOnPress,
               std::function<void()> buttonOnPressLong) :
        pin(buttonPin),
        mode(buttonMode),
        onPress(std::move(buttonOnPress)),
        onPressLong(std::move(buttonOnPressLong)) {

    pinMode(pin, mode);

    invertedState = mode == IN_PULLUP;
    isLongPressSupported = onPressLong != nullptr;

    state.lastRawState = RELEASED;
    state.currentState = RELEASED;
    state.lastState = RELEASED;
    state.lastChangeTime = 0;
}

bool Button::operator<(const Button &other) const {
    return false;
}

bool Button::operator==(const Button &other) const {
    return this->pin == other.pin;
}

