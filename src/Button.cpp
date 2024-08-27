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

void Button::setClick(std::function<void()> onPress) {
    this->onPress = std::move(onPress);
}

void Button::setClickLong(std::function<void()> onPressLong,
                      unsigned int longPressTime,
                      bool isMultipleLongPressSupported) {
    this->onPressLong = std::move(onPressLong);
    this->longPressTime = longPressTime;
    this->isMultipleLongPressSupported = isMultipleLongPressSupported;
}

void Button::setClickDouble(std::function<void()> onPressDouble,
                    unsigned int doublePressTime) {
    this->onPressDouble = std::move(onPressDouble);
    this->doublePressTime = doublePressTime;
}

void Button::setLongPressTime(unsigned int time) {
    longPressTime = time;
}

bool Button::operator<(const Button &other) const {
    return false;
}

bool Button::operator==(const Button &other) const {
    return this->pin == other.pin;
}

