#include <Arduino.h>
#include "buzzer_pitches.h"



class AlertSounder {
protected:
    int _buzzerPin; // Pin connected to the buzzer
    void PlayTune(int* melody, int* durations, int noteCount) {
        for (int note = 0; note < noteCount; note++) {
            //to calculate the note duration, take one second divided by the note type.
            //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
            int duration = 1000 / durations[note];
            tone(_buzzerPin, melody[note], duration);

            //to distinguish the notes, set a minimum time between them.
            //the note's duration + 30% seems to work well:
            int pauseBetweenNotes = duration * 1.30;
            delay(pauseBetweenNotes);

            //stop the tone playing:
            noTone(_buzzerPin);
        }
    };
public:
    // Constructor
    AlertSounder(int buzzerPin) {
        _buzzerPin = buzzerPin;
        pinMode(buzzerPin, OUTPUT);
        digitalWrite(buzzerPin, LOW); // Ensure buzzer is off initially
    }

    // Method to play the alert sound
    virtual void play() = 0;
};


class SirenSounder : public AlertSounder {
public:
    // Constructor
    SirenSounder(int buzzerPin) : AlertSounder(buzzerPin) { }

    void play() override {
        tone(_buzzerPin, 2500); // start with a tone at 1000 Hz
        delay(150);
        tone(_buzzerPin, 1000);
        delay(150);
        noTone(_buzzerPin); // stop the tone
    }
};



class ItsASmallWorldSounder : public AlertSounder {

public:
    // Constructor
    ItsASmallWorldSounder(int buzzerPin) : AlertSounder(buzzerPin) { }
    
    void play() override {
        int melody[] = {
        NOTE_E4, NOTE_F4, NOTE_G4, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_B4
        // , NOTE_D4,
        // NOTE_E4, NOTE_F4, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_F4, 
        // NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, 
        // NOTE_D5, NOTE_G4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_C5, REST, NOTE_C5, REST, NOTE_C5, NOTE_E5, 
        // NOTE_C5, NOTE_D5, REST, NOTE_D5, NOTE_D5, NOTE_D5, REST, NOTE_D5, NOTE_F5, NOTE_D5, NOTE_E5, REST, NOTE_E5, 
        // NOTE_E5, NOTE_E5, REST, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_F5, REST, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_G4, 
        // NOTE_B4, NOTE_C5, NOTE_C5, REST
        };

        int durations[] = {
        8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 
        8, 8, 4, 8, 8, 4, 4, 4, 4, 2, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 2, 4, 8, 8, 4, 4, 4, 8, 8, 
        2, 4, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 2, 2, 2, 4, 4
        };

        int noteCount = sizeof(melody) / sizeof(int);
        PlayTune(melody, durations, noteCount);
        //AlertSounder::PlayTune(melody, durations, noteCount);
    }

};

class NeverGonnaGiveYouUpSounder : public AlertSounder {
    public:
    // Constructor
    NeverGonnaGiveYouUpSounder(int buzzerPin) : AlertSounder(buzzerPin) { }

    void play() override {
        int melody[] = {
            // NOTE_A4, REST, NOTE_B4, REST, NOTE_C5, REST, NOTE_A4, REST,
            // NOTE_D5, REST, NOTE_E5, REST, NOTE_D5, REST,

            NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
            NOTE_D5, REST,

            // NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_D5, REST,
            // NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

            // NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
            // NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

            // NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
            // NOTE_D5, REST,

            // NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G5, NOTE_B4, REST,
            // NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

            // NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
            // NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

            // NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_E5, REST,
            // NOTE_G5, NOTE_F5, NOTE_E5, REST,

            // NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST
        };

        int durations[] = {
            // 8, 8, 8, 8, 8, 8, 8, 4,
            // 8, 8, 8, 8, 2, 2,

            8, 8, 8, 8, 2, 8, 8,
            2, 8,

            // 8, 8, 8, 8, 2, 8, 8,
            // 4, 8, 8, 8, 8,

            // 8, 8, 8, 8, 2, 8, 8,
            // 2, 8, 4, 8, 8, 8, 8, 8, 1, 4,

            // 8, 8, 8, 8, 2, 8, 8,
            // 2, 8,

            // 8, 8, 8, 8, 2, 8, 8,
            // 2, 8, 8, 8, 8,

            // 8, 8, 8, 8, 2, 8, 8,
            // 4, 8, 3, 8, 8, 8, 8, 8, 1, 4,

            // 2, 6, 2, 6, 4, 4, 2, 6, 2, 3,
            // 8, 8, 8, 8,

            // 2, 6, 2, 6, 2, 1
        };

        int noteCount = sizeof(melody) / sizeof(int);
        
        PlayTune(melody, durations, noteCount);
    }
};
