#include "Button.h"

Button::Button(byte buttonPin,
               Mode buttonMode) :
        pin(buttonPin),
        mode(buttonMode) {

    pinMode(pin, mode);

    invertedState = mode == IN_PULLUP;

    state.lastRawState = RELEASED;
    state.currentState = RELEASED;
    state.lastState = RELEASED;
    state.lastChangeTime = 0;
}

void Button::setBehavior(std::function<void()> onPress,
                         std::function<void()> onPressLong,
                         bool isMultipleLongPressSupported) {
    this->onPress = std::move(onPress);
    this->onPressLong = std::move(onPressLong);
    this->isLongPressSupported = onPressLong != nullptr;
    this->isMultipleLongPressSupported = isMultipleLongPressSupported;
}

bool Button::operator<(const Button &other) const {
    return false;
}

bool Button::operator==(const Button &other) const {
    return this->pin == other.pin;
}

