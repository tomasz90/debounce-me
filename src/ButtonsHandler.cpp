#include "ButtonsHandler.h"

#if !LEGACY
ButtonsHandler::ButtonsHandler(std::initializer_list<Button *> buttons) : buttons(buttons) {
    std::for_each(this->buttons.begin(), this->buttons.end(), [this](Button *button) {
        buttonLastStartPressed[button] = 0;
        buttonWasLongPressed[button] = false;
    });
}

void ButtonsHandler::setClickSimultaneous(std::set<Button *> _buttons, std::function<void()> behavior) {
    simultaneousBehaviors[_buttons] = std::move(behavior);
}

void ButtonsHandler::setClickSimultaneousLong(std::set<Button *> _buttons, std::function<void()> behavior,
                                              unsigned int longPressTime) {
    simultaneousBehaviorsLong[_buttons] = std::move(behavior);
    simultaneousLongPressTimes[_buttons] = longPressTime;
}
#else
// LEGACY (AVR) implementation
ButtonsHandler::ButtonsHandler(Button **buttons, uint8_t numButtons) : numButtons(numButtons) {
    for(uint8_t i = 0; i < numButtons; i++) {
        this->buttons[i] = buttons[i];
        buttonLastStartPressed[i] = 0;
        buttonWasLongPressed[i] = false;
    }
}

void ButtonsHandler::setClickSimultaneous(Button** buttons, uint8_t count, void (*behavior)()) {
    if (numSimultaneous >= MAX_GROUPS) return;

    ButtonGroup& group = simultaneousGroups[numSimultaneous++];
    group.count = count < MAX_BUTTONS ? count : MAX_BUTTONS;
    memcpy(group.buttons, buttons, sizeof(Button*) * group.count);
    group.behavior = behavior;
}

void ButtonsHandler::setClickSimultaneousLong(Button** buttons, uint8_t count, void (*behavior)(),
                                              unsigned int longPressTime) {
    if (numSimultaneousLong >= MAX_GROUPS) return;

    ButtonGroup& group = simultaneousLongGroups[numSimultaneousLong++];
    group.count = count < MAX_BUTTONS ? count : MAX_BUTTONS;
    memcpy(group.buttons, buttons, sizeof(Button*) * group.count);
    group.behaviorLong = behavior;
    group.longPressTime = longPressTime;
}

uint8_t ButtonsHandler::getButtonIndex(Button* button) const {
    for(uint8_t i = 0; i < numButtons; i++) {
        if (buttons[i] == button) return i;
    }
    return MAX_BUTTONS;
}

bool ButtonsHandler::checkGroupPressed(const ButtonGroup& group) const {
    for(uint8_t i = 0; i < group.count; i++) {
        auto button = group.buttons[i];
        if (!isPressed(button)) return false;
    }
    return true;
}

bool ButtonsHandler::checkGroupLongPressed(const ButtonGroup& group) const {
    for(uint8_t i = 0; i < group.count; i++) {
        const uint8_t idx = getButtonIndex(group.buttons[i]);
        if (idx >= MAX_BUTTONS) return false;
        if (millis() - buttonLastStartPressed[idx] < group.longPressTime) {
            return false;
        }
    }
    return true;
}

bool ButtonsHandler::checkNoOtherPressed(const ButtonGroup& group) const {
    for(uint8_t i = 0; i < numButtons; i++) {
        bool inGroup = false;
        for(uint8_t j = 0; j < group.count; j++) {
            if (buttons[i] == group.buttons[j]) {
                inGroup = true;
                break;
            }
        }
        if (!inGroup && isPressed(buttons[i])) return false;
    }
    return true;
}
#endif

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
#if !LEGACY
    if (button->onPressLong == nullptr) return false;
    auto lastStartPressed = buttonLastStartPressed.at(button);
#else
    const uint8_t idx = getButtonIndex(button);
    if (idx >= MAX_BUTTONS) return false;
    auto lastStartPressed = buttonLastStartPressed[idx];
#endif
    auto matchTime = millis() - lastStartPressed >= button->longPressTime && lastStartPressed != 0;
    return matchTime && isOneButtonPressed();
}

bool ButtonsHandler::isSimultaneousLongPressed(Button *button) {
    if (isOneButtonPressed()) return false;

#if !LEGACY
    if (!simultaneousLongPressTimes[simultaneousButtons]) return false;
    auto lastStartPressed = buttonLastStartPressed.at(button);
    return millis() - lastStartPressed >= simultaneousLongPressTimes[simultaneousButtons];
#else
    for(uint8_t i = 0; i < numSimultaneousLong; i++) {
        const ButtonGroup& group = simultaneousLongGroups[i];
        if (checkGroupPressed(group)) {
            return checkGroupLongPressed(group) && checkNoOtherPressed(group);
        }
    }
    return false;
#endif
}

bool &ButtonsHandler::wasLongPressed(Button *button) {
#if !LEGACY
    return buttonWasLongPressed.at(button);
#else
    const uint8_t idx = getButtonIndex(button);
    return buttonWasLongPressed[idx];
#endif
}

bool ButtonsHandler::isOneButtonPressed() const {
#if !LEGACY
    return simultaneousButtons.size() < 2;  // this is required to be able to run multiple OnPressLong
#else
    uint8_t pressedCount = 0;
    for(uint8_t i = 0; i < numButtons; i++) {
        if (isPressed(buttons[i])) pressedCount++;
        if (pressedCount > 1) return false;
    }
    return true;
#endif
}

// this function can be used to poll inside `setup()`
#if IS_FREE_RTOS_SUPPORTED
void ButtonsHandler::pollOnce(int pollInterval) {
    auto task = [](TimerHandle_t xTimer) {
        auto _this = static_cast<ButtonsHandler *>(pvTimerGetTimerID(xTimer));
        _this->poll();
    };
    timer = xTimerCreate(NULL, pdMS_TO_TICKS(pollInterval), true, this, task);
    xTimerStart(timer, 0);
}

void ButtonsHandler::pollStop() {
    if (timer == nullptr) return;
    xTimerStop(timer, 0);
    xTimerDelete(timer, 0);
}
#endif

// this function can be used to poll inside `loop()`
void ButtonsHandler::poll() {
#if !LEGACY
    for (auto &button: buttons) pollState(button);
    for (auto &button: buttons) processButtonState(button);
    for (auto &button: buttons) resetState(button);
#else
    for (uint8_t i = 0; i < numButtons; i++) pollState(buttons[i]);
    for (uint8_t i = 0; i < numButtons; i++) processButtonState(buttons[i]);
    for (uint8_t i = 0; i < numButtons; i++) resetState(buttons[i]);
#endif
}

void ButtonsHandler::processButtonState(Button *button) {
    bool isSimultaneousLongPress = isSimultaneousLongPressed(button);
    bool isLongPress = isLongPressed(button);
#if !LEGACY
    bool _wasPressed = wasPressed(button) && !wasSimultaneousPress && !isLongPress && !wasLongPressed(button);
#else
    const uint8_t idx = getButtonIndex(button);
    bool _wasPressed = wasPressed(button) && !wasSimultaneousPress && !isLongPress && !buttonWasLongPressed[idx];
#endif

    bool _isOneButtonPressed = isOneButtonPressed();
    bool isDoublePressSupported = button->onPressDouble != nullptr;
#if !LEGACY
    bool isRegisteredPress = buttonLastClicked[button] != 0;
    bool isElapsedTime = millis() - buttonLastClicked[button] > button->doublePressTime;
#else
    bool isRegisteredPress = buttonLastClicked[idx] != 0;
    bool isElapsedTime = millis() - buttonLastClicked[idx] > button->doublePressTime;
#endif

    if (isPressed(button) && !wasSimultaneousPress && isSimultaneousLongPress) {
        onSimultaneousPressLong();
    } else if (isPressed(button) && !wasSimultaneousPress && isLongPress) {
        onPressLong(button);
    } else if ((_wasPressed && _isOneButtonPressed && !isDoublePressSupported) ||
               (isRegisteredPress && isElapsedTime)) {
        onPress(button);
    } else if (_wasPressed && _isOneButtonPressed && isDoublePressSupported && isRegisteredPress && !isElapsedTime) {
        onDoublePress(button);
    } else if (_wasPressed && _isOneButtonPressed && !isRegisteredPress) {
        registerPress(button);
    } else if (_wasPressed) {
        onSimultaneousPress(button);
    } else if (wasReleased(button)) {
        onWasReleased(button);
    }
}

void ButtonsHandler::onSimultaneousPressLong() {
#if !LEGACY
    auto behavior = simultaneousBehaviorsLong[simultaneousButtons];
    if (behavior) behavior();
    simultaneousButtons.clear();
#else
    for(uint8_t i = 0; i < numSimultaneousLong; i++) {
        ButtonGroup& group = simultaneousLongGroups[i];
        if (checkGroupPressed(group) && checkNoOtherPressed(group)) {
            if (group.behaviorLong) group.behaviorLong();
            break;
        }
    }
#endif
    wasSimultaneousPress = true;
}

void ButtonsHandler::onPressLong(Button *button) {
    button->onPressLong();
#if !LEGACY
    simultaneousButtons.clear();
    wasLongPressed(button) = true;
    buttonLastStartPressed[button] = button->isMultipleLongPressSupported ? millis() : 0;
#else
    const uint8_t idx = getButtonIndex(button);
    buttonWasLongPressed[idx] = true;
    buttonLastStartPressed[idx] = button->isMultipleLongPressSupported ? millis() : 0;
#endif
}

void ButtonsHandler::onSimultaneousPress(Button *button) {
#if !LEGACY
    auto behavior = simultaneousBehaviors[simultaneousButtons];
    if (behavior) behavior();
    simultaneousButtons.clear();
#else
    for(uint8_t i = 0; i < numSimultaneous; i++) {
        ButtonGroup& group = simultaneousGroups[i];
        if (checkGroupPressed(group) && checkNoOtherPressed(group)) {
            if (group.behavior) group.behavior();
            break;
        }
    }
#endif
    wasSimultaneousPress = true;
#if !LEGACY
    buttonLastStartPressed[button] = 0;
#else
    const uint8_t idx = getButtonIndex(button);
    buttonLastStartPressed[idx] = 0;
#endif
}

// Rest of common event handlers with LEGACY adaptations
void ButtonsHandler::registerPress(Button *button) {
#if !LEGACY
    buttonLastClicked[button] = millis();
#else
    const uint8_t idx = getButtonIndex(button);
    buttonLastClicked[idx] = millis();
#endif
}

void ButtonsHandler::onDoublePress(Button *button) {
    button->onPressDouble();
#if !LEGACY
    simultaneousButtons.clear();
    buttonLastStartPressed[button] = 0;
    buttonLastClicked[button] = 0;
#else
    const uint8_t idx = getButtonIndex(button);
    buttonLastStartPressed[idx] = 0;
    buttonLastClicked[idx] = 0;
#endif
}

void ButtonsHandler::onPress(Button *button) {
    button->onPress();
#if !LEGACY
    simultaneousButtons.clear();
    buttonLastStartPressed[button] = 0;
    buttonLastClicked[button] = 0;
#else
    const uint8_t idx = getButtonIndex(button);
    buttonLastStartPressed[idx] = 0;
    buttonLastClicked[idx] = 0;
#endif
}

void ButtonsHandler::onWasReleased(Button *button) {
#if !LEGACY
    simultaneousButtons.insert(button);
#else
    // Handled through group checking in processButtonState
#endif
    wasSimultaneousPress = false;
#if !LEGACY
    wasLongPressed(button) = false;
#else
    const uint8_t idx = getButtonIndex(button);
    buttonWasLongPressed[idx] = false;
#endif
#if !LEGACY
    buttonLastStartPressed[button] = millis();
#else
    buttonLastStartPressed[idx] = millis();
#endif
}