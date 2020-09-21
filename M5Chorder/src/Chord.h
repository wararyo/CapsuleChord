#ifndef _CHORD_H_
#define _CHORD_H_

#include <stdint.h>
#include <vector>
#include <map>
#include <WString.h>
#include <functional>
#include "Archive.h"

class Chord {
public:
    //Root
    static const uint8_t C      = 0;
    static const uint8_t CSharp = 1;
    static const uint8_t D      = 2;
    static const uint8_t DSharp = 3;
    static const uint8_t E      = 4;
    static const uint8_t F      = 5;
    static const uint8_t FSharp = 6;
    static const uint8_t G      = 7;
    static const uint8_t GSharp = 8;
    static const uint8_t A      = 9;
    static const uint8_t ASharp = 10;
    static const uint8_t B      = 11;

    //Third, Fifth, and Tension
    static const uint16_t Major   = 0b00000000;
    static const uint16_t Minor   = 0b00000001;
    static const uint16_t Dimish  = 0b00000010;
    static const uint16_t Sus4    = 0b00000100;
    static const uint16_t Sus2    = 0b00001000;
    static const uint16_t Aug     = 0b00010000;

    static const uint16_t Seventh      = 0b00100000;
    static const uint16_t MajorSeventh = 0b01000000;
    static const uint16_t Sixth        = 0b10000000;

    static const uint16_t FifthFlat       = 0b00000001 << 8;
    static const uint16_t Ninth           = 0b00000010 << 8;
    static const uint16_t NinthSharp      = 0b00000100 << 8;
    static const uint16_t Eleventh        = 0b00001000 << 8;
    static const uint16_t EleventhSharp   = 0b00010000 << 8;
    static const uint16_t Thirteenth      = 0b00100000 << 8;
    static const uint16_t ThirteenthSharp = 0b01000000 << 8;
    static const uint16_t ThirteenthFlat  = 0b10000000 << 8;

    static const std::vector<String> rootStrings;
    static const std::map<uint16_t,String> optionStrings;

    uint8_t root;
    uint16_t option;

    Chord();
    Chord(uint8_t root, uint16_t option);
    
    std::vector<uint8_t> toMidiNoteNumbers(uint8_t centerNoteNo, uint8_t region);
    String toString();

    void serialize(OutputArchive &archive,const char *key) {
        archive.pushNest(key);
        archive("Root",root);
        archive("Option",option);
        archive.popNest();
    }
    void deserialize(InputArchive &archive,const char *key) {
        archive.pushNest(key);
        archive("Root",root);
        archive("Option",option);
        archive.popNest();
    }

protected:
    uint8_t maxNoteNo;
    uint8_t minNoteNo;
    void addMidiNote(std::vector<uint8_t>* notes,uint8_t noteNo);
};

class DegreeChord {
    //Root
    static const uint8_t I       = 0;
    static const uint8_t ISharp  = 1;
    static const uint8_t II      = 2;
    static const uint8_t IISharp = 3;
    static const uint8_t III     = 4;
    static const uint8_t IV      = 5;
    static const uint8_t IVSharp = 6;
    static const uint8_t V       = 7;
    static const uint8_t VSharp  = 8;
    static const uint8_t VI      = 9;
    static const uint8_t VISharp = 10;
    static const uint8_t VII     = 11;

public:
    static const std::vector<String> rootStrings;

    uint8_t root;
    uint16_t option;

    DegreeChord();
    DegreeChord(uint8_t root, uint16_t option);

    String toString();

    bool equals(DegreeChord other);

    void serialize(OutputArchive &archive,const char *key) {
        archive.pushNest(key);
        archive("Root",root);
        archive("Option",option);
        archive.popNest();
    }
    void deserialize(InputArchive &archive,const char *key) {
        archive.pushNest(key);
        archive("Root",root);
        archive("Option",option);
        archive.popNest();
    }
};

#endif