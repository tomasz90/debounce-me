#include "Button.h"
#include "Arduino.h"

Button::Button(byte buttonPin,
               Mode buttonMode) :
        pin(buttonPin),
        mode(buttonMode) {
    invertedState = mode == IN_PULLUP;

    state.lastRawState = RELEASED;
    state.currentState = RELEASED;
    state.lastState = RELEASED;
    state.lastChangeTime = 0;
}

Button::Button(byte buttonPinA, byte buttonPinB, Mode buttonAMode) : Button(buttonPinA, buttonAMode) {
    pinSecond = buttonPinB;
}

void Button::initialize() {
    if (initialized) return;
    initialized = true;
    pinMode(pin, mode);
    if (pinSecond == -1) return;
//    auto pinSecondState = mode == IN_PULLUP ? LOW : HIGH;
//    pinMode(pinSecond, OUTPUT);
//    digitalWrite(pinSecond, pinSecondState);
}

void Button::setClick(std::function<void()> behavior) {
    initialize();
    this->onPress = std::move(behavior);
}

void Button::setClickLong(std::function<void()> behavior,
                          unsigned int _longPressTime,
                          bool _isMultipleLongPressSupported) {
    initialize();
    this->onPressLong = std::move(behavior);
    this->longPressTime = _longPressTime;
    this->isMultipleLongPressSupported = _isMultipleLongPressSupported;
}

void Button::setClickDouble(std::function<void()> behavior, unsigned int _doublePressTime) {
    initialize();
    this->onPressDouble = std::move(behavior);
    this->doublePressTime = _doublePressTime;
}

bool Button::operator<(const Button &other) const {
    // Compare pins first, then other properties if pins are equal
    if (pin != other.pin) {
        return pin < other.pin;
    }
    if (pinSecond != other.pinSecond) {
        return pinSecond < other.pinSecond;
    }
    return mode < other.mode;
}

bool Button::operator==(const Button &other) const {
    return pin == other.pin && pinSecond == other.pinSecond;
}

