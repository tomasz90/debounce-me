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
    auto pinSecondState = mode == IN_PULLUP ? LOW : HIGH;
    pinMode(pinSecond, OUTPUT);
    digitalWrite(pinSecond, pinSecondState);
}

#if !LEGACY
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
#else
void Button::setClick(void (*behavior)()) {
    initialize();
    this->onPress = behavior;
}

void Button::setClickLong(void (*behavior)(),
                          unsigned int _longPressTime,
                          bool _isMultipleLongPressSupported) {
    initialize();
    this->onPressLong = behavior;
    this->longPressTime = _longPressTime;
    this->isMultipleLongPressSupported = _isMultipleLongPressSupported;
}

void Button::setClickDouble(void (*behavior)(), unsigned int _doublePressTime) {
    initialize();
    this->onPressDouble = behavior;
    this->doublePressTime = _doublePressTime;
}
#endif

bool Button::operator<(const Button &other) const {
    return false;
}

bool Button::operator==(const Button &other) const {
    return this->pin == other.pin;
}

