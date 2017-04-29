#ifndef _TRACK_H
#define _TRACK_H
#include "cll.h"
#define NUM_TRACKS (7) /*the number of tracks we care about*/
#define BURN_TIME (10000) /*how long to power each solenoid*/

static int BUTTONS[] = {13, 12, 11, 10, 9,  6,  21}; /*the list of input pins the buttons are connected to*/
static int MALLETS[] = {14, 15, 16, 17, 18, 19, 0};  /*the list of output pints the solenoids are connected to*/
static int BEAT_PIN = 1; /*The pin to drive in time with the beat*/

class Track {
    private:
    struct Event {
      int delta; /*time the note starts*/
      int type;  /*0 for sentinel, 1 for real note*/
    };
    
    CircularlyLinkedList<Event> tracks[NUM_TRACKS];
    
    int timers[NUM_TRACKS]; /*timer for each track*/
    int burners[NUM_TRACKS];/*a counter for how long each solenoid is firing for*/
    
    public:
    Track() {
      /*set each pin to the correct mode, set outputs low, and add track start sentinels*/
      for (int i = 0; i < NUM_TRACKS; i++) {
          tracks[i].addNode({0, 0});
          pinMode(MALLETS[i], OUTPUT);
          digitalWrite(MALLETS[i], LOW);
          pinMode(BUTTONS[i], INPUT);
      }
    }
    /*function for adding the track end sentinel*/
    void doOff(int off) {
      for (int i = 0; i < NUM_TRACKS; i++) {
          Event e = tracks[i].getTail();
          e.type = 0;
          e.delta = ((e.delta/off)+1)*off;
          tracks[i].addNode(e);
      }
    }
    /*add a note to a track*/
    void addNote(int track, int delta) {
        tracks[track-1].addNode({delta, 1});
    }
    /*process every track event in the last ticks ticks*/
    void doLoop(uint16_t ticks) {
        for (int i = 0; i < NUM_TRACKS; i++) {
            /*check if the track is firing or about to stop*/
            if (burners[i] > 0) {
                /*update its status*/
                digitalWrite(MALLETS[i], burners[i]>1);
                /*decrease its fire counter*/
                burners[i]--;
                /*Print for debugging*/
                if (burners[i] == 0) {
                    Serial.print("FIRE ");
                    Serial.println(i);
                }
            }
            /*let time pass for the track*/
            timers[i]+=ticks;
            /*if (digitalRead(BUTTONS[i])) {
                Serial.print("TRACK ");
                Serial.print(i);
                Serial.println(" off");
                timers[i] = 0;
                tracks[i].reset();
                continue;
            }*/
            /*Check if the next note is ready to fire for this track*/
            if (tracks[i].getCurr().delta < timers[i]) {
                /*if so, check if it is a real note*/
                if (tracks[i].getCurr().type == 1) {
                     /*if so, fire it!*/
                     /*consider setting it high here and only setting it low when it is time to stop firing*/
                    burners[i] = BURN_TIME;
                }
                /*Advance the track, and if we reached the end then reset out timer*/
                if (tracks[i].next()) {
                    timers[i] = 0;
                }
            }
        }
    }
};
#endif


