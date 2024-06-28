#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <functional>

#define RELEASED 0
#define PRESSED 1

enum Mode {
    IN_PULLUP = INPUT_PULLUP,
    IN_PULLDOWN = INPUT_PULLDOWN,
};

struct Button {
    byte pin;
    Mode mode;
    bool invertedState;
    bool isLongPressSupported;
    bool isMultipleLongPressSupported;
    unsigned int longPressTime;

    std::function<void()> onPress;
    std::function<void()> onPressLong;

    struct {
        bool lastRawState;
        bool currentState;
        bool lastState;
        unsigned long lastChangeTime;
    } state;

    Button(byte buttonPin, Mode buttonMode);

    void setBehavior(std::function<void()> buttonOnPress,
                     std::function<void()> buttonOnPressLong = nullptr,
                     unsigned int longPressTime = 1000,
                     bool isMultipleLongPressSupported = false);

    void setLongPressTime(unsigned int time);
    bool operator<(const Button &other) const;
    bool operator==(const Button& other) const;
};


#endif //BUTTON_H
