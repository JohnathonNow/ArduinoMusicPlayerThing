#ifndef _TRACK_H
#define _TRACK_H
#include "cll.h"
#include "list.h"
#define NUM_TRACKS (7)
#define BURN_TIME (100)

static int BUTTONS[] = {13, 12, 11, 10, 9,  6,  21};
static int MALLETS[] = {14, 15, 16, 17, 18, 19, 0};
static int BEAT_PIN = 1;

class Track {
    private:
    struct Event {
      int delta;
      int type;
    };
    CircularlyLinkedList<Event> tracks[NUM_TRACKS];
    int timers[NUM_TRACKS];
    int burners[NUM_TRACKS];
    public:
    Track() {
      for (int i = 0; i < NUM_TRACKS; i++) {
          tracks[i].addNode({0, 0});
          pinMode(MALLETS[i], OUTPUT);
          pinMode(BUTTONS[i], INPUT);
      }
    }
    void doOff(int off) {
      for (int i = 0; i < NUM_TRACKS; i++) {
          Event e = tracks[i].getTail();
          e.type = 0;
          e.delta = ((e.delta/off)+1)*off;
          tracks[i].addNode(e);
      }
    }
    void addNote(int track, int delta) {
        /*Serial.print("NOTE at ");
        Serial.print(delta);
        Serial.print(" for track ");
        Serial.println(track);*/
        tracks[track-1].addNode({delta, 1});
    }
    void doLoop(uint16_t ticks) {
        for (int i = 0; i < NUM_TRACKS; i++) {
            if (burners[i] > 0) {
                digitalWrite(MALLETS[i], burners[i]>1);
                burners[i]--;
            }
            timers[i]+=ticks;
            if (digitalRead(BUTTONS[i])) {
                timers[i] = 0;
                tracks[i].reset();
                continue;
            }
            if (tracks[i].getCurr().delta < timers[i]) {
                if (tracks[i].getCurr().type == 1) {
                     //DO what we need for this track
                    burners[i] = BURN_TIME;
                }
                if (tracks[i].next()) {
                    timers[i] = 0;
                }
            }
        }
    }
};
#endif


