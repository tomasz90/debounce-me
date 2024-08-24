#include "ButtonsHandler.h"

ButtonsHandler::ButtonsHandler(std::initializer_list<Button *> buttons) : buttons(buttons) {
    std::for_each(this->buttons.begin(), this->buttons.end(), [this](Button *button) {
        buttonLastStartPressed[button] = 0;
        buttonWasLongPressed[button] = false;
    });
}

void ButtonsHandler::setDebounceTime(unsigned int time) {
    debounceTime = time;
}

void ButtonsHandler::setSimultaneousBehavior(std::set<Button *> _buttons, std::function<void()> behavior) {
    simultaneousBehaviors[_buttons] = std::move(behavior);
}

void ButtonsHandler::setSimultaneousBehaviorLong(std::set<Button *> _buttons, std::function<void()> behavior,
                                                 unsigned int longPressTime) {
    simultaneousBehaviorsLong[_buttons] = std::move(behavior);
    simultaneousLongPressTimes[_buttons] = longPressTime;
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
    if (!button->isLongPressSupported) return false;
    auto lastStartPressed = buttonLastStartPressed.at(button);
    auto matchTime = millis() - lastStartPressed >= button->longPressTime && lastStartPressed != 0;
    return matchTime && isOneButtonPressed();
}

bool ButtonsHandler::isSimultaneousLongPressed(Button *button) {
    auto lastStartPressed = buttonLastStartPressed.at(button);
    if (!simultaneousLongPressTimes[simultaneousButtons]) return false;
    auto matchTime = millis() - lastStartPressed >= simultaneousLongPressTimes[simultaneousButtons];
    return matchTime && !isOneButtonPressed();
}

bool &ButtonsHandler::wasLongPressed(Button *button) {
    return buttonWasLongPressed.at(button);
}

bool ButtonsHandler::isOneButtonPressed() const {
    return simultaneousButtons.size() < 2;  // this is required to be able to run multiple OnPressLong
}

// this function can be used to poll inside `startup()`, `loop()` is not required.
// use either this or `poll()`
#if defined(ESP32) || \
    defined(NRF52840_XXAA) || \
    defined(RASPBERRY_PI_PICO) || \
    defined(RASPBERRY_PI_PICO_W) || \
    defined(USE_POLL_ONCE)
void ButtonsHandler::pollOnce(int pollInterval) {
    auto task = [](TimerHandle_t xTimer) {
        auto _this = static_cast<ButtonsHandler *>(pvTimerGetTimerID(xTimer));
        _this->poll();
    };
    auto timer = xTimerCreate(NULL, pdMS_TO_TICKS(pollInterval), true, this, task);
    xTimerStart(timer, 0);
}
#endif

// this function can be used to poll inside `loop()`
void ButtonsHandler::poll() {
    for (auto &button: buttons) pollState(button);
    for (auto &button: buttons) processButtonState(button);
    for (auto &button: buttons) resetState(button);
}

void ButtonsHandler::processButtonState(Button *button) {
    bool isLongPress = isLongPressed(button);
    bool isSimultaneousLongPress = isSimultaneousLongPressed(button);
    bool &wasLongPress = wasLongPressed(button);

    if (isPressed(button) && !wasSimultaneousPress && isSimultaneousLongPress) {
        onSimultaneousPressLong();
    } else if (isPressed(button) && !wasSimultaneousPress && isLongPress) {
        onPressLong(button);
    } else if (wasPressed(button) && !wasSimultaneousPress && !isLongPress && !wasLongPress && isOneButtonPressed()) {
        onPress(button);
    } else if (wasPressed(button) && !wasSimultaneousPress && !isLongPress && !wasLongPress) {
        onSimultaneousPress(button);
    } else if (wasReleased(button)) {
        onWasReleased(button);
    }
}

void ButtonsHandler::onSimultaneousPressLong() {
    auto behavior = simultaneousBehaviorsLong[simultaneousButtons];
    if (behavior) behavior();
    simultaneousButtons.clear();
    wasSimultaneousPress = true;
}

void ButtonsHandler::onPressLong(Button *button) {
    button->onPressLong();
    simultaneousButtons.clear();
    wasLongPressed(button) = true;
    buttonLastStartPressed[button] = button->isMultipleLongPressSupported ? millis() : 0;
}

void ButtonsHandler::onSimultaneousPress(Button *button) {
    auto behavior = simultaneousBehaviors[simultaneousButtons];
    if (behavior) behavior();
    simultaneousButtons.clear();
    wasSimultaneousPress = true;
    buttonLastStartPressed[button] = 0;
}

void ButtonsHandler::onPress(Button *button) {
    button->onPress();
    simultaneousButtons.clear();
    buttonLastStartPressed[button] = 0;
}

void ButtonsHandler::onWasReleased(Button *button) {
    simultaneousButtons.insert(button);
    wasSimultaneousPress = false;
    wasLongPressed(button) = false;
    buttonLastStartPressed[button] = millis();
}