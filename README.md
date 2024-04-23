# arduino-melody-buzzer

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/zaragozamartin91/library/SimpleMelodyBuzzer.svg)](https://registry.platformio.org/libraries/zaragozamartin91/SimpleMelodyBuzzer)

Simple library for playing tunes and notes on Arduino boards using passive buzzers 

The tune is played asynchronously and allows for callbacks to do steps on tunes.

Example on how to play HEY JUDE using the library:
```C++
#include "music_notes.h" // including the library with the frequencies of the note 
#include "MelodyBuzzer.h"

/* EXAMPLE code based on https://www.hackster.io/brambi001/play-any-song-with-arduino-passive-buzzer-000d52 written by Ilaria Brambilla - 25/09/2022 */

int BUZZER_PIN=PIN3;
mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);

int melody[] = {
  NOTE_C4, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_AS3, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4
}; // declaring the notes of the melody (they change depending on the song you wanna play)

int melodySize = sizeof(melody) / sizeof(melody[0]);

int durations[] = {
  2, 2, 3, 3, 3, 2, 3, 3, 2, 1, 3, 3, 3, 5, 5, 5, 1, 2, 1, 3, 3, 3, 3, 1
}; // declaring the duration of each note (4 is a quarter note ecc)


void setup () {
    Serial.begin(9600);
    while(!Serial){};
}


int melodyPtr = 0;

int calculateDurationMilliseconds(int durationVal) { return 1000 / durations[melodyPtr]; }

void onTuneEnd(unsigned int frequency, unsigned long startTime, unsigned long endTime, unsigned long currentTime) {
  Serial.print("frequency: "); Serial.print(frequency);
  Serial.print(" ; startTime: "); Serial.print(startTime);
  Serial.print(" ; endTime: "); Serial.print(endTime);
  Serial.print(" ; currentTime: "); Serial.print(currentTime);
  Serial.println();

  int duration = calculateDurationMilliseconds(durations[melodyPtr]);
  int pause = duration * 0.25; // adding a little delay in between each note to separate the sounds
  delay(pause);

  ++melodyPtr;
}


void loop() {
  bool songPlaying = melodyPtr < melodySize;
  unsigned int delayTime = songPlaying ? 50 : 5000;

  if (songPlaying) {
    melodyBuzzer.playAsync(melody[melodyPtr], calculateDurationMilliseconds(durations[melodyPtr]), &onTuneEnd);
  } 

  melodyBuzzer.update();

  delay(delayTime);
}
```

The `MelodyBuzzer` constructor expects 4 parameters:
* `int buzzerPin;` // pin to send the buzzer signal
* `void (*emitTone)(uint8_t _pin, unsigned int frequency, unsigned long duration);` // function for emitting a tone
* `void (*disableTone)(uint8_t _pin);` // function for disabling tones
* `unsigned long (*getTimeMillis)();` // function for obtaining current time in milliseconds

The `playAsync` method starts playing a tone given a frequency and a duration.
The tone plays asynchronously ; meaning that the loop function won't be blocked when a tune starts playing.
The function accepts an OPTIONAL callback function to be invoked every time a tone ends.

The `update` function is to be called on every loop cycle.
