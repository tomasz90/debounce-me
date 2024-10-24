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

Button::Button(byte buttonPinA, byte buttonPinB, Mode buttonAMode) : Button(buttonPinA, buttonAMode) {
    pinMode(buttonPinB, OUTPUT);
    digitalWrite(buttonPinB, buttonAMode == IN_PULLUP ? LOW : HIGH);
}

void Button::setClick(std::function<void()> behavior) {
    this->onPress = std::move(behavior);
}

void Button::setClickLong(std::function<void()> behavior,
                          unsigned int _longPressTime,
                          bool _isMultipleLongPressSupported) {
    this->onPressLong = std::move(behavior);
    this->longPressTime = _longPressTime;
    this->isMultipleLongPressSupported = _isMultipleLongPressSupported;
}

void Button::setClickDouble(std::function<void()> behavior, unsigned int _doublePressTime) {
    this->onPressDouble = std::move(behavior);
    this->doublePressTime = _doublePressTime;
}

bool Button::operator<(const Button &other) const {
    return false;
}

bool Button::operator==(const Button &other) const {
    return this->pin == other.pin;
}

