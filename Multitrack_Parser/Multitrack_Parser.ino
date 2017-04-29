#include "MidiPlayer.h"
#define SD_SELECT   5

/* Top level file - starts the SD card, reads from it, and 
 * plays the song
 * 
 * Note that notes are parsed into a circularly linked list by track,
 * and all notes are treated equally within a track
 */
void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("GO");
    MidiPlayer::start(SD_SELECT);
    MidiPlayer::load("motortest2.mid");
    MidiPlayer::parse();
}
void loop() {
    MidiPlayer::play();
}

