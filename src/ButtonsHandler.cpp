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

void ButtonsHandler::setSimultaneousBehaviorLong(std::set<Button *> _buttons, std::function<void()> behavior, unsigned int longPressTime) {
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
    if(!button->isLongPressSupported) return false;
    auto lastStartPressed = buttonLastStartPressed.at(button);
    return millis() - lastStartPressed >= button->longPressTime && lastStartPressed != 0;
}

bool ButtonsHandler::isSimultaneousLongPressed(Button *button) {
    auto lastStartPressed = buttonLastStartPressed.at(button);
    if(!simultaneousLongPressTimes[simultaneousButtons]) return false;
    return millis() - lastStartPressed >= simultaneousLongPressTimes[simultaneousButtons];
}

bool &ButtonsHandler::wasLongPressed(Button *button) {
    return buttonWasLongPressed.at(button);
}

bool ButtonsHandler::isOneButtonPressed() const {
    return simultaneousButtons.size() < 2;  // this is required to be able to run multiple OnPressLong
}

void ButtonsHandler::simultaneousOnPress() {
    auto behavior = simultaneousBehaviors[simultaneousButtons];
    if (behavior) behavior();
    wasSimultaneousPress = true;
}

void ButtonsHandler::simultaneousOnPressLong() {
    auto behavior = simultaneousBehaviorsLong[simultaneousButtons];
    if (behavior) behavior();
    wasSimultaneousPress = true;
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
    for (auto &button : buttons) pollState(button);
    for (auto &button : buttons) processButtonState(button);
    for (auto &button : buttons) resetState(button);
}

void ButtonsHandler::processButtonState(Button *button) {
    bool isLongPress = isLongPressed(button);
    bool isSimultaneousLongPress = isSimultaneousLongPressed(button);
    bool &wasLongPress = wasLongPressed(button);

    // LONG PRESS CALLBACKS
    if (isPressed(button) && isLongPress && !wasSimultaneousPress && isOneButtonPressed()) {
        button->onPressLong();
        wasLongPress = true;
        buttonLastStartPressed[button] = button->isMultipleLongPressSupported ? millis() : 0;
        simultaneousButtons.clear();
    // SIMULTANEOUS LONG PRESS CALLBACKS
    } else if (isPressed(button) && isSimultaneousLongPress && !wasSimultaneousPress && !isOneButtonPressed()) {
        simultaneousOnPressLong();
        simultaneousButtons.clear();
    // SIMULTANEOUS PRESS CALLBACKS
    } else if (wasPressed(button) && !isLongPress && !wasLongPress && !wasSimultaneousPress && isOneButtonPressed()) {
        button->onPress();
        buttonLastStartPressed[button] = 0;
        simultaneousButtons.clear();
    // PRESS CALLBACKS
    } else if(wasPressed(button) && !isLongPress && !wasLongPress && !wasSimultaneousPress) {
        simultaneousOnPress();
        buttonLastStartPressed[button] = 0;
        simultaneousButtons.clear();
    // SET VARIABLES WHEN BUTTON WAS JUST PRESSED
    } else if (wasReleased(button)) {
        buttonLastStartPressed[button] = millis();
        simultaneousButtons.insert(button);
        wasLongPress = false;
        wasSimultaneousPress = false;
    }
}