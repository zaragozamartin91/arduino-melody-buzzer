#ifndef MELODYBUZZER_H
#define MELODYBUZZER_H

#include <stdint.h>

namespace mz
{
    class MelodyBuzzer {
        private:
            int buzzerPin;
            void (*emitTone)(uint8_t _pin, unsigned int frequency, unsigned long duration); // function for emitting a tone
            void (*disableTone)(uint8_t _pin); // function for disabling tones
            unsigned long (*getTimeMillis)(); // function for obtaining current time in milliseconds

            
            unsigned int frequency; // frequency of current playing tone
            unsigned long startTime; // start time of tone
            unsigned long endTime; // expected end time of tone
            unsigned long currentTime; // current time


            void (*toneEndCallback)(unsigned int, unsigned long, unsigned long, unsigned long); // callback when tone ends
            bool freshTone; // keeps track of callback invocation


            void invokeToneEndCallback();
            void invokeDisableTone();


        public:
            MelodyBuzzer(
                int buzzerPin, 
                void (*emitTone)(uint8_t _pin, unsigned int frequency, unsigned long duration),
                void (*disableTone)(uint8_t _pin),
                unsigned long (*getTimeMillis)()
            );

            // frequency (in hertz) and duration (in milliseconds).
            void playAsync(unsigned int frequency, unsigned long duration, void (*toneEndCallback)(unsigned int, unsigned long,unsigned long,unsigned long) = nullptr);

            // returns true if a note is currently being played
            bool playingTone();

            // to be called on every loop cycle
            void update();

            // stops the tune
            void stop();
    };
} // namespace mz

#endif
