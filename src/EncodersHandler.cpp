#include "EncodersHandler.h"

#if !LEGACY
EncodersHandler::EncodersHandler(std::initializer_list<Encoder *> encoders) : encoders(encoders) {}

void EncodersHandler::poll() {
    for (auto &encoder: encoders) pollEncoderState(encoder);
    for (auto &encoder: encoders) processEncoderState(encoder);
    for (auto &encoder: encoders) resetState(encoder);
}
#else
EncodersHandler::EncodersHandler(Encoder **encoders, uint8_t numEncoders) {
    this->numEncoders = min(numEncoders, MAX_ENCODERS);
    for (uint8_t i = 0; i < this->numEncoders; i++) {
        this->encoders[i] = encoders[i];
    }
}
void EncodersHandler::poll() {
    for (uint8_t i = 0; i < numEncoders; i++) { pollEncoderState(encoders[i]); }
    for (uint8_t i = 0; i < numEncoders; i++) { processEncoderState(encoders[i]); }
    for (uint8_t i = 0; i < numEncoders; i++) { resetState(encoders[i]); }
}
#endif

#if IS_FREE_RTOS_SUPPORTED
void EncodersHandler::pollOnce(int pollInterval) {
    auto task = [](TimerHandle_t xTimer) {
        auto _this = static_cast<EncodersHandler *>(pvTimerGetTimerID(xTimer));
        _this->poll();
    };
    timer = xTimerCreate(NULL, pdMS_TO_TICKS(pollInterval), true, this, task);
    xTimerStart(timer, 0);
}

void EncodersHandler::pollStop() {
    if (timer == nullptr) return;
    xTimerStop(timer, 0);
    xTimerDelete(timer, 0);
}
#endif

void EncodersHandler::setDebounceTime(unsigned long debounceTimeUs) {
    this->debounceTimeUs = debounceTimeUs;
}

void EncodersHandler::pollEncoderState(Encoder *encoder) const {
    pollPinState(&encoder->pinA);
    pollPinState(&encoder->pinB);
}

void EncodersHandler::pollPinState(Pin *pin) const {
    auto &s = pin->state;
    bool currentRawState = pin->invertedState == !digitalRead(pin->id);
    if (s.lastRawState != currentRawState) {
        bool moreThanDebounceTime = (unsigned long) (micros() - s.lastChangeTime) >= debounceTimeUs;
        if (moreThanDebounceTime) {
            s.currentState = currentRawState;
            s.lastChangeTime = micros();
        }
    }
    s.lastRawState = currentRawState;
}

void EncodersHandler::processEncoderState(Encoder *encoder) const {
    auto &sA = encoder->pinA.state;
    auto &sB = encoder->pinB.state;

    const bool aPrev = sA.lastState;
    const bool aCurr = sA.currentState;
    const bool bPrev = sB.lastState;
    const bool bCurr = sB.currentState;

    const uint8_t prevState = (aPrev << 1) | bPrev;
    const uint8_t currState = (aCurr << 1) | bCurr;

    // Get delta from decoder table
    int8_t delta = decoder[prevState][currState];
    encoder->accumulated += delta;
    encoder->position += delta;

    // Trigger callbacks every 4 steps (1 full detent)
    if (encoder->accumulated >= 4) {
        if (encoder->onClockwise) encoder->onClockwise();
        encoder->accumulated = 0;
    } else if (encoder->accumulated <= -4) {
        if (encoder->onCounterClockwise) encoder->onCounterClockwise();
        encoder->accumulated = 0;
    }
}

void EncodersHandler::resetState(Encoder *encoder) const {
    encoder->pinA.state.lastState = encoder->pinA.state.currentState;
    encoder->pinB.state.lastState = encoder->pinB.state.currentState;
}
