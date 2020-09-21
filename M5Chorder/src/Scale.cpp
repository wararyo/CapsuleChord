#include "Scale.h"
#include "Chord.h"

std::vector<std::shared_ptr<ScaleBase>> Scale::availableScales;

Scale::Scale(uint8_t key) : key(key) {
    currentScale = getAvailableScales()[0].get();
}

Scale::Scale() : key(0) {}

String Scale::toString() {
    return Chord::rootStrings[key] + " " + currentScale->name();
}

Chord Scale::degreeToChord(DegreeChord degree) {
    return currentScale->degreeToChord(key,degree);
}

Chord Scale::getDiatonic(uint8_t degree, bool seventh, Chord base) {
    return currentScale->getDiatonic(key,degree,seventh,base);
}

std::vector<std::shared_ptr<ScaleBase>> Scale::getAvailableScales() {
    if(availableScales.empty()) {
        availableScales.push_back(std::make_shared<MajorScale>());
        availableScales.push_back(std::make_shared<MinorScale>());
    }
    return availableScales;
}

int Scale::getScaleIndex() {
    for(int i = 0;i < getAvailableScales().size();i++){
        if(getAvailableScales()[i].get() == currentScale) {
            return i;
        }
    }
    return -1;
}

int Scale::getScaleIndexFromName(String scaleStr) {
    for(int i = 0;i < getAvailableScales().size();i++){
        if(getAvailableScales()[i]->name() == scaleStr) {
            return i;
        }
    }
}

ScaleBase *Scale::getScaleFromName(String scaleStr) {
    for(auto i : getAvailableScales()){
        if(i->name() == scaleStr) {
            return i.get();
        }
    }
}

//****
// ScaleBase
//****

Chord ScaleBase::degreeToChord(uint8_t key, DegreeChord degree) {
    Chord c = Chord(0,degree.option);
    c.root = key + degree.root;

    //0から12の範囲内に収める
    while(c.root < 0)  c.root += 12;
    while(c.root >= 12)  c.root -= 12;

    return c;
}

//****
// MajorScale
//****

const uint8_t MajorScale::pitch[7] = {0,2,4,5,7,9,11};
const uint16_t MajorScale::diatonicOptions[] = {
    0,
    Chord::Minor,
    Chord::Minor,
    0,
    0,
    Chord::Minor,
    Chord::Minor|Chord::FifthFlat};
const uint16_t MajorScale::diatonicSeventhOptions[] = {
    Chord::MajorSeventh,
    Chord::Minor|Chord::Seventh,
    Chord::Minor|Chord::Seventh,
    Chord::MajorSeventh,
    Chord::Seventh,
    Chord::Minor|Chord::Seventh,
    Chord::Minor|Chord::Seventh|Chord::FifthFlat};

Chord MajorScale::getDiatonic(uint8_t key, uint8_t number, bool seventh, Chord base) {
    // return degreeToChord(key,degree,0, Chord(0,seventh?diatonicSeventhOptions[degree]:diatonicOptions[degree]));
    return degreeToChord(key,DegreeChord(pitch[number], base.option | seventh?diatonicSeventhOptions[number]:diatonicOptions[number]));
}

//****
// MinorScale
//****

const uint8_t MinorScale::pitch[7] = {0,2,3,5,7,8,10};
const uint16_t MinorScale::diatonicOptions[] = {
    Chord::Minor,
    Chord::Dimish,
    0,
    Chord::Minor,
    Chord::Minor,
    0,
    0};
const uint16_t MinorScale::diatonicSeventhOptions[] = {
    Chord::Minor|Chord::Seventh,
    Chord::Minor|Chord::Seventh|Chord::FifthFlat,
    Chord::MajorSeventh,
    Chord::Minor|Chord::Seventh,
    Chord::Minor|Chord::Seventh,
    Chord::MajorSeventh,
    Chord::Seventh};

Chord MinorScale::getDiatonic(uint8_t key, uint8_t number, bool seventh, Chord base) {
    return degreeToChord(key,DegreeChord(pitch[number], base.option | seventh?diatonicSeventhOptions[number]:diatonicOptions[number]));
}