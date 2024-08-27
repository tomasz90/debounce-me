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
    bool isMultipleLongPressSupported;
    unsigned int longPressTime;
    unsigned int doublePressTime;

    std::function<void()> onPress;
    std::function<void()> onPressLong;
    std::function<void()> onPressDouble;

    struct {
        bool lastRawState;
        bool currentState;
        bool lastState;
        unsigned long lastChangeTime;
    } state;

    Button(byte buttonPin, Mode buttonMode);

    void setClick(std::function<void()> buttonOnPress);

    void setClickLong(std::function<void()> buttonOnPressLong,
                  unsigned int longPressTime = 1000,
                  bool isMultipleLongPressSupported = false);

    void setClickDouble(std::function<void()> buttonOnPressDouble,
                  unsigned int doublePressTime = 250);

    bool operator<(const Button &other) const;
    bool operator==(const Button& other) const;
};


#endif //BUTTON_H
