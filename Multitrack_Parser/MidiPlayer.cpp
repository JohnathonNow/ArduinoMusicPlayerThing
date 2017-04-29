#include "MidiPlayer.h"
#include "track.h"
#include <MD_MIDIFile.h>
#include <SdFat.h>

//Delta is how many MIDI ticks we process in each iteration of the parse loop, 1000 is reasonable, 1 is not
#define DELTA (1000)
int TIME = 0; // How far along in the file we are

SdFat  SD;
MD_MIDIFile SMF;

Track track;

//Set up our parsing
bool MidiPlayer::start(int sd_sel) {
    //Begin by connecting the SD card, and ensuring we can talk with it
    if (!SD.begin(sd_sel, SPI_FULL_SPEED)) {
        return false;
    }
    //Next, begin communication with the card and the SMF file library
    SMF.begin(&SD);
    //Finally, set up our parser callback
    SMF.setMidiHandler(midiCallback);
    return true;
}

//Load our MIDI file for the library to parse
void MidiPlayer::load(char* fname) {
    SMF.setFilename(fname);
    SMF.load();
}

//Actually parse the tracks
void MidiPlayer::parse() {
    //Loop over the entire file
    while (!SMF.isEOF()) {
        //Process all events in the last DELTA bits of time
        TIME += DELTA;
        SMF.processEvents(DELTA);
        
    }
    //Calculate our time signature
    int sig = SMF.getTimeSignature();
    int d = 1<<(sig & 0xFF);
    int n = sig >> 8;
    int tpqn = SMF.getTicksPerQuarterNote();
    //The reason we need all that is to make the end of the track end on the end of a measure
    //So this adds a sentinel value
    int off = (d*tpqn/n);
    track.doOff(off);
    //Finally, close the file
    SMF.close();
}

void MidiPlayer::play() { //play for one iteration, so this should go in loop()
    track.doLoop(SMF.tickClock());
}

//Called for every MIDI event by the SMF MD_MIDIFile library
void MidiPlayer::midiCallback(midi_event *e) {//uint32_t TIME = 0;
    //If the higher byte is 9, then this is a MIDI note start event
    if (e->data[0]>>4 == 0x9) {        
        //So, add the note to the corresponding track
        track.addNote(e->track, TIME-e->offset);
    }
}




