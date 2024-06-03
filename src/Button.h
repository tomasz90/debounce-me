#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

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

    std::function<void()> onPress;
    std::function<void()> onPressLong;

    struct {
        bool lastRawState;
        bool currentState;
        bool lastState;
        unsigned long lastChangeTime;
    } state;

    Button(byte buttonPin,
           Mode buttonMode,
           std::function<void()> buttonOnPress,
           std::function<void()> buttonOnPressLong = nullptr);

    bool operator<(const Button &other) const;
    bool operator==(const Button& other) const;
};


#endif //BUTTON_H
