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
    int8_t pinSecond = -1; // 2 pins instead pin and ground/3.3V

    Mode mode;
    bool invertedState;
    bool isMultipleLongPressSupported;
    unsigned int longPressTime;
    unsigned int doublePressTime;
    bool initialized = false;

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

    // support for buttons with two pins instead pin and ground/3.3V
    Button(byte buttonPinA, byte buttonPinB, Mode buttonMode);

    void setClick(std::function<void()> behavior);

    void setClickLong(std::function<void()> behavior,
                      unsigned int _longPressTime = 1000,
                      bool _isMultipleLongPressSupported = false);

    void setClickDouble(std::function<void()> behavior, unsigned int _doublePressTime = 250);

    bool operator<(const Button &other) const;

    bool operator==(const Button &other) const;

private:
    void initialize();
};


#endif //BUTTON_H
