#include "ButtonsHandler.h"

ButtonsHandler::ButtonsHandler(std::initializer_list<Button *> buttons) : buttons(buttons) {
    std::for_each(this->buttons.begin(), this->buttons.end(), [this](Button *button) {
        buttonTemporary[button].buttonLastStartPressed = 0;
        buttonTemporary[button].buttonWasLongPressed = false;
        btnStates[button] = {false, false, false, false, false, false, false};
        lastBtnStates[button] = {false, false, false, false, false, false, false};
    });
}

void ButtonsHandler::setDebounceTime(unsigned int time) {
    debounceTime = time;
}

void ButtonsHandler::setClickSimultaneous(std::set<Button *> _buttons, std::function<void()> behavior) {
    simultaneousBehaviors[_buttons].behavior = std::move(behavior);
}

void ButtonsHandler::setClickSimultaneousLong(std::set<Button *> _buttons, std::function<void()> behavior,
                                              unsigned int longPressTime) {
    simultaneousBehaviors[_buttons].behaviorLong = std::move(behavior);
    simultaneousBehaviors[_buttons].longPressTime = longPressTime;
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

bool ButtonsHandler::isLongPressed(Button *button) {
    if (button->onPressLong == nullptr) return false;
    auto lastStartPressed = buttonTemporary[button].buttonLastStartPressed;
    auto matchTime = millis() - lastStartPressed >= button->longPressTime && lastStartPressed != 0;
    return matchTime && !areMultipleButtonsPressed();
}

bool ButtonsHandler::isSimultaneousLongPressed(Button *button) {
    if (!areMultipleButtonsPressed()) return false;
    if (simultaneousBehaviors[currentlyPressed].longPressTime == -1) return false;
    auto lastStartPressed = buttonTemporary[button].buttonLastStartPressed;
    return millis() - lastStartPressed >= simultaneousBehaviors[currentlyPressed].longPressTime;
}

bool &ButtonsHandler::wasLongPressed(Button *button) {
    return buttonTemporary[button].buttonWasLongPressed;
}

bool ButtonsHandler::areMultipleButtonsPressed() const {
    return currentlyPressed.size() >= 2;  // this is required to be able to run multiple OnPressLong
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
    for (auto &button: buttons) pollState(button);
    for (auto &button: buttons) processButtonState(button);
    for (auto &button: buttons) resetState(button);
}

void ButtonsHandler::processButtonState(Button *button) {
    if(lastBtnStates[button] != btnStates[button]) {
        btnStates[button].log(button->pinSecond);
    }
    lastBtnStates[button] = btnStates[button];
    bool isSimultaneousLongPress = isSimultaneousLongPressed(button);
    bool isLongPress = isLongPressed(button);
    bool _wasPressed = wasPressed(button) && !wasSimultaneousPress && !isLongPress && !wasLongPressed(button);
    bool _areMultipleButtonsPressed = areMultipleButtonsPressed();
    bool isDoublePressSupported = button->onPressDouble != nullptr;
    bool isRegisteredPress = buttonTemporary[button].buttonLastClicked != 0;
    bool isElapsedTime = millis() - buttonTemporary[button].buttonLastClicked > button->doublePressTime;

    btnStates[button] = {
            isSimultaneousLongPress,
            isLongPress,
            _wasPressed,
            _areMultipleButtonsPressed,
            isDoublePressSupported,
            isRegisteredPress,
            isElapsedTime
    };
    if (isPressed(button) && !wasSimultaneousPress && isSimultaneousLongPress) {
        onSimultaneousPressLong();
        Serial.println("\nXXXXXXXXXXXXXXXX SIMULTANEOUS LONG PRESS XXXXXXXXXXXXXXXX\n");
    } else if (isPressed(button) && !wasSimultaneousPress && isLongPress) {
        onPressLong(button);
        Serial.println("\nXXXXXXXXXXXXXXXX        LONG PRESS       XXXXXXXXXXXXXXXX\n");
    } else if ((_wasPressed && !_areMultipleButtonsPressed && !isDoublePressSupported) ||
               (isRegisteredPress && isElapsedTime)) {
        onPress(button);
        Serial.println("\nXXXXXXXXXXXXXXXX           PRESS         XXXXXXXXXXXXXXXX\n");
    } else if (_wasPressed && !_areMultipleButtonsPressed && isDoublePressSupported && isRegisteredPress && !isElapsedTime) {
        onDoublePress(button);
        Serial.println("\nXXXXXXXXXXXXXXXX       DOUBLE PRESS      XXXXXXXXXXXXXXXX\n");
    } else if (_wasPressed && !_areMultipleButtonsPressed && !isRegisteredPress) {
        registerPress(button);
    } else if (_wasPressed) {
        onSimultaneousPress(button);
        Serial.println("\nXXXXXXXXXXXXXXXX    SIMULTANEOUS PRESS    XXXXXXXXXXXXXXXX\n");
    } else if (wasReleased(button)) {
        onWasReleased(button);
    }
}

void ButtonsHandler::onSimultaneousPressLong() {
    auto behavior = simultaneousBehaviors[currentlyPressed].behaviorLong;
    if (behavior) behavior();
    currentlyPressed.clear();
    wasSimultaneousPress = true;
}

void ButtonsHandler::onPressLong(Button *button) {
    button->onPressLong();
    currentlyPressed.clear();
    wasLongPressed(button) = true;
    buttonTemporary[button].buttonLastStartPressed = button->isMultipleLongPressSupported ? millis() : 0;
}

void ButtonsHandler::onSimultaneousPress(Button *button) {
    auto behavior = simultaneousBehaviors[currentlyPressed].behavior;
    if (behavior) behavior();
    currentlyPressed.clear();
    wasSimultaneousPress = true;
    buttonTemporary[button].buttonLastStartPressed = 0;
}

void ButtonsHandler::registerPress(Button *button) {
    buttonTemporary[button].buttonLastClicked = millis();
}

void ButtonsHandler::onDoublePress(Button *button) {
    button->onPressDouble();
    currentlyPressed.clear();
    buttonTemporary[button].buttonLastStartPressed = 0;
    buttonTemporary[button].buttonLastClicked = 0;
}

void ButtonsHandler::onPress(Button *button) {
    button->onPress();
    currentlyPressed.clear();
    buttonTemporary[button].buttonLastStartPressed = 0;
    buttonTemporary[button].buttonLastClicked = 0;
}

void ButtonsHandler::onWasReleased(Button *button) {
    currentlyPressed.insert(button);
    wasSimultaneousPress = false;
    wasLongPressed(button) = false;
    buttonTemporary[button].buttonLastStartPressed = millis();
}