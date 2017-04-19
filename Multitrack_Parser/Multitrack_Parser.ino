#include "MidiPlayer.h"
#define SD_SELECT   5
#define LED_PIN     13

void setup() {
    pinMode(LED_PIN, OUTPUT);
    //Serial.begin(115200);
    //while (!Serial);
    MidiPlayer::start(SD_SELECT);
    MidiPlayer::load("motortest2.mid");
    MidiPlayer::play();
}
void loop() {}




