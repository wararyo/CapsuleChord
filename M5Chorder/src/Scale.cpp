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

Chord Scale::degreeToChord(uint8_t degree, uint8_t offset, Chord base) {
    return currentScale->degreeToChord(key,degree,offset,base);
}

Chord Scale::getDiatonic(uint8_t degree, bool seventh) {
    return currentScale->getDiatonic(key,degree,seventh);
}

std::vector<std::shared_ptr<ScaleBase>> Scale::getAvailableScales() {
    if(availableScales.empty()) {
        availableScales.push_back(std::make_shared<MajorScale>());
        availableScales.push_back(std::make_shared<MinorScale>());
    }
    return availableScales;
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
    Chord::Dimish};
const uint16_t MajorScale::diatonicSeventhOptions[] = {
    Chord::MajorSeventh,
    Chord::Minor|Chord::Seventh,
    Chord::Minor|Chord::Seventh,
    Chord::MajorSeventh,
    Chord::Seventh,
    Chord::Minor|Chord::Seventh,
    Chord::Minor|Chord::Seventh|Chord::FifthFlat};

Chord MajorScale::degreeToChord(uint8_t key, uint8_t degree, uint8_t offset, Chord base) {
    base.root = key + pitch[degree];
    base.root += offset;

    //0から12の範囲内に収める
    while(base.root < 0)  base.root += 12;
    while(base.root >= 12)  base.root -= 12;

    return base;
}

Chord MajorScale::getDiatonic(uint8_t key, uint8_t degree, bool seventh) {
    return degreeToChord(key,degree,0,
        Chord(0,seventh?diatonicSeventhOptions[degree]:diatonicOptions[degree]));
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



Chord MinorScale::degreeToChord(uint8_t key, uint8_t degree, uint8_t offset, Chord base) {
    base.root = key + pitch[degree];
    base.root += offset;

    //0から12の範囲内に収める
    while(base.root < 0)  base.root += 12;
    while(base.root >= 12)  base.root -= 12;

    return base;
}

Chord MinorScale::getDiatonic(uint8_t key, uint8_t degree, bool seventh) {
    return degreeToChord(key,degree,0,
        Chord(0,seventh?diatonicSeventhOptions[degree]:diatonicOptions[degree]));
}