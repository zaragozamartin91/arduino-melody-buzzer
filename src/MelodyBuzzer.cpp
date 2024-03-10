#include "MelodyBuzzer.h"

mz::MelodyBuzzer::MelodyBuzzer(
    int buzzerPin,
    void (*emitTone)(uint8_t _pin, unsigned int frequency, unsigned long duration),
    void (*disableTone)(uint8_t _pin),
    unsigned long (*getTimeMillis)())
{
    this->buzzerPin = buzzerPin;
    this->emitTone = emitTone;
    this->disableTone = disableTone;
    this->getTimeMillis = getTimeMillis;

    startTime = 0;
    currentTime = 0;
    endTime = 0;

    freshTone = false;
    toneEndCallback = nullptr;
}

void mz::MelodyBuzzer::playAsync(
    unsigned int frequency, 
    unsigned long duration,
    void (*toneEndCallback)(unsigned int, unsigned long,unsigned long,unsigned long)) {

    if (this->playingTone()) { return; } // cannot play 2 tones at once

    this->frequency = frequency;
    this->freshTone = true; // new tone started playing
    this->startTime = this->getTimeMillis();
    this->endTime = startTime + duration;
    this->currentTime = startTime;
    this->toneEndCallback = toneEndCallback;

    this->emitTone(buzzerPin, frequency, duration);
}

bool mz::MelodyBuzzer::playingTone() {
    return currentTime < endTime;
}

void mz::MelodyBuzzer::update() {
    this->currentTime = this->getTimeMillis();
    bool toneFinished = this->freshTone && !this->playingTone();

    if (toneFinished && this->disableTone != nullptr) { this->disableTone(buzzerPin); }
    if (toneFinished && this->toneEndCallback != nullptr) { this->toneEndCallback(this->frequency , startTime, endTime, currentTime); }
    if (toneFinished) { this->freshTone = false; }
}
