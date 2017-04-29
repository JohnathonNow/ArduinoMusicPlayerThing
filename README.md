# ArduinoMusicPlayerThing

Uses a modified verson of the [MD\_MIDIFile](https://github.com/MajicDesigns/MD_MIDIFile) library to load single notes from tracks into memory to play back in a loop.

The basic code structure is as follows:

- Multitrack\_Parser handles the highest level of program flow - it
includes the `setup()` and `loop()` Arduino functions. `setup` handles
reading in the file and parsing the track while `loop` handles playing
the tracks. It wraps MidiPlayer.
- MidiPlayer handles preparing the tracks, including using MD\_MIDIFile
to parse the tracks. It wraps a Track object.
- Track handles a set of tracks and the low-level details of the project.
It handles all of the I/O, and encompasses a CircularlyLinkedList (cll)
for each track.
- Cll is simply an implementation of a linked list where the tail's next
pointer points to the head of the list. This makes sense for a looping
track where the idea of "restarting" is, for the most part, unimportant.
It allows for higher level pieces of code to only care about playing
the next note, ignoring the detail of wrapping around to the start.
