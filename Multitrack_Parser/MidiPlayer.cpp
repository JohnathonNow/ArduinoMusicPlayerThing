#include "MidiPlayer.h"
#include <SdFat.h>
#include "Print.h"
#include "track.h"
#define DELTA (1000)
#include <MD_MIDIFile.h>

int LED = 0;
uint32_t TIME = 0;

SdFat  SD;
MD_MIDIFile SMF;

Track track;

bool MidiPlayer::start(int sd_sel) {

    if (!SD.begin(sd_sel, SPI_FULL_SPEED)) {
        return false;
    }

    SMF.begin(&SD);
    SMF.setMidiHandler(midiCallback);
    return true;
}

void MidiPlayer::load(char* fname) {
    SMF.setFilename(fname);
    SMF.load();
}

void MidiPlayer::play() {
    while (!SMF.isEOF()) {
        TIME+=DELTA;
        SMF.processEvents(DELTA);
    }
    TIME+=DELTA;
    SMF.processEvents(DELTA);
    int sig = SMF.getTimeSignature();
    int d = 1<<(sig & 0xFF);
    int n = sig >> 8;
    int tpqn = SMF.getTicksPerQuarterNote();
    int off = (d*tpqn/n);
    track.doOff(off);
    SMF.close();
    for (;;) track.doLoop(SMF.tickClock());
}

void MidiPlayer::midiCallback(midi_event *e) {
    /*Serial.print((int)e->track);
    Serial.print(' ');
    Serial.print((int)e->data[0]);
    Serial.print(' ');
    Serial.println(TIME-e->offset);*/
    if (e->data[0]>>4 == 0x9) {
        char note = e->data[1]%12;
        char octa = e->data[1]/12;
        
        track.addNote(e->track, TIME-e->offset);
    }
}



