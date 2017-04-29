#ifndef _MIDIPPLAYER_H__
#define _MIDIPPLAYER_H__
#include <SdFat.h>
#include <MD_MIDIFile.h>
#include "track.h"

//A simple header file for the MidiPlayer 

namespace MidiPlayer {
      bool start(int sd_sel);
      void load(char* fname);
      void play();
      void parse();
      void midiCallback(midi_event *e);
};
#endif




