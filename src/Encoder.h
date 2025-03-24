#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

#if defined(USE_LEGACY)
#define LEGACY 1
#else
#define LEGACY 0
#include <functional>
#endif

#define RELEASED 0
#define PRESSED 1

enum Mode {
    IN_PULLUP = INPUT_PULLUP,
    IN_PULLDOWN = INPUT_PULLDOWN,
    OUT = OUTPUT,
};

struct Pin {
    Pin(byte id, Mode mode);
    Pin(byte id, byte lowOrHigh);

    byte id;
    Mode mode;
    bool invertedState;
    bool initialized = false;

    struct {
        bool lastRawState;
        bool currentState;
        bool lastState;
        unsigned long lastChangeTime;
    } state;

    void initialize();
};

struct Encoder {
    // common GND
    Encoder(byte pinA, byte pinB);

    // common GND or VCC
    Encoder(byte pinA, byte pinB, Mode encoderMode);

    // common ordinary pin
    Encoder(byte pinA, byte pinB, byte pinCommon, Mode encoderMode);

    Pin pinA;
    Pin pinB;
    Pin *pinCommon = nullptr;

    volatile long position = 0;
    int accumulated = 0;

#if !LEGACY
    std::function<void()> onClockwise = nullptr;
    std::function<void()> onCounterClockwise = nullptr;
    void setOnClockwise(std::function<void()> behavior);
    void setOnCounterClockwise(std::function<void()> behavior);
#else
    void (*onClockwise)() = nullptr;
    void (*onCounterClockwise)() = nullptr;
    void setOnClockwise(void (*behavior)());
    void setOnCounterClockwise(void (*behavior)());
#endif

private:
    void initialize();
};

#endif //ENCODER_H
