#ifndef _SCALE_H_
#define _SCALE_H_

#include <stdint.h>
#include <vector>
#include <memory>
#include <map>
#include <WString.h>
#include <functional>
#include "Chord.h"

class ScaleBase;

//Chordクラスと同様に現在使っているスケールを管理するためのクラス
class Scale {
protected:
    static std::vector<std::shared_ptr<ScaleBase>> availableScales;
public:
    Scale();
    Scale(uint8_t key);
    uint8_t key = 0; //主音 C=0
    ScaleBase *currentScale;

    //Degree: 一般に1-7までの値 Offset: ♭=-1 ♯=+1
    Chord degreeToChord(uint8_t degree, uint8_t offset, Chord base);
    Chord degreeToChord(uint8_t degree, uint8_t offset, uint8_t option) { return degreeToChord(degree, offset, Chord(Chord::C, option)); }
    Chord getDiatonic(uint8_t degree, bool seventh);
    String toString();
    static std::vector<std::shared_ptr<ScaleBase>> getAvailableScales();
};

//ある特定の種類のスケールを定義するための基底クラス
class ScaleBase {
public:
    virtual String name() {return "BaseScale";};
    virtual Chord degreeToChord(uint8_t key, uint8_t degree, uint8_t offset, Chord base) {return base;};
    virtual Chord getDiatonic(uint8_t key, uint8_t degree, bool seventh){return degreeToChord(key,degree,0,Chord());}
};

class MajorScale : public ScaleBase {
public:
    static const uint16_t diatonicOptions[];
    static const uint16_t diatonicSeventhOptions[];
    String name() {return "Major";};
    static const uint8_t pitch[];
    Chord degreeToChord(uint8_t key, uint8_t degree, uint8_t offset, Chord base) override;
    Chord getDiatonic(uint8_t key, uint8_t degree, bool seventh) override;
};

class MinorScale : public ScaleBase {
public:
    static const uint16_t diatonicOptions[];
    static const uint16_t diatonicSeventhOptions[];
    String name() {return "Minor";};
    static const uint8_t pitch[];
    Chord degreeToChord(uint8_t key, uint8_t degree, uint8_t offset, Chord base) override;
    Chord getDiatonic(uint8_t key, uint8_t degree, bool seventh) override;
};

#endif