#include <Chord.h>

Chord::Chord()
: Chord(C,0) {}

Chord::Chord(uint8_t root, uint16_t option)
: root(root) , option(option) {}

String Chord::toString() {
    String str = "";
    str += rootStrings[root];
    for(auto item : optionStrings) {
        if(option & item.first) {
            str += item.second;
        }
    }
    return str;
}

std::vector<uint8_t> Chord::toMidiNoteNumbers(uint8_t centerNoteNo, uint8_t region) {
    maxNoteNo = centerNoteNo + region/2;
    minNoteNo = centerNoteNo - region/2;
    uint8_t baseNoteNo = root;
    while(baseNoteNo < minNoteNo) baseNoteNo += 12;

    std::vector<uint8_t> notes = std::vector<uint8_t>();

    //Root
    addMidiNote(&notes, baseNoteNo);

    //Third
    if(option & Sus4) addMidiNote(&notes, baseNoteNo + 5);
    else if(option & Minor) addMidiNote(&notes, baseNoteNo + 3);
    else addMidiNote(&notes, baseNoteNo + 4); //Major

    //Fifth
    if(option & FifthFlat) addMidiNote(&notes, baseNoteNo + 6);
    else if(option & Aug) addMidiNote(&notes, baseNoteNo + 8);
    else addMidiNote(&notes, baseNoteNo + 7);

    //Seventh
    if(option & Seventh) addMidiNote(&notes, baseNoteNo + 10);
    else if(option & MajorSeventh) addMidiNote(&notes, baseNoteNo + 11);

    //Ninth
    if(option & Ninth) addMidiNote(&notes, baseNoteNo + 14);

    //Eleventh
    if(option & Eleventh) addMidiNote(&notes, baseNoteNo + 17);

    //Eleventh
    if(option & Thirteenth) addMidiNote(&notes, baseNoteNo + 21);

    return notes;
}

void Chord::addMidiNote(std::vector<uint8_t>* notes,uint8_t noteNo) {
    while(noteNo > maxNoteNo) noteNo -= 12;
    while(noteNo < minNoteNo) noteNo += 12;
    notes->push_back(noteNo);
}

const std::vector<String> Chord::rootStrings = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
const std::map<uint16_t,String> Chord::optionStrings = {
        {Major  , ""},
        {Minor  , "m"},
        {Dimish , "dim"},
        {Sus4   , "sus4"},
        {Sus2   , "sus2"},
        {Aug    , "aug"},
        {Seventh, "7"},
        {MajorSeventh, "M7"},
        {Sixth, "6"},
        {FifthFlat       , "-5"},
        {Ninth           , "9"},
        {NinthSharp      , "♯9"},
        {Eleventh        , "11"},
        {EleventhSharp   , "♯11"},
        {Thirteenth      , "13"},
        {ThirteenthSharp , "♯13"},
        {ThirteenthFlat  , "♭13"},
    };

DegreeChord::DegreeChord()
: DegreeChord(I,0) {}

DegreeChord::DegreeChord(uint8_t root, uint16_t option)
: root(root) , option(option) {}

String DegreeChord::toString() {
    String str = "";
    str += rootStrings[root];
    for(auto item : Chord::optionStrings) {
        if(option & item.first) {
            str += item.second;
        }
    }
    return str;
}

bool DegreeChord::equals(DegreeChord other){
    if(other.root != this->root) return false;
    if(other.option != this->option) return false;
    return true;
}

const std::vector<String> DegreeChord::rootStrings = {"I","I#","II","II#","III","IV","IV#","V","V#","VI","VI#","VII"};
