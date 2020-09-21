#ifndef _SCALE_H_
#define _SCALE_H_

#include <stdint.h>
#include <vector>
#include <memory>
#include <map>
#include <WString.h>
#include <functional>
#include "Chord.h"
#include "Archive.h"

//ある特定の種類のスケールを定義するための基底クラス
class ScaleBase {
public:
    virtual String name() {return "BaseScale";};
    virtual Chord degreeToChord(uint8_t key, DegreeChord degree);
    virtual Chord getDiatonic(uint8_t key, uint8_t number, bool seventh, Chord base = Chord()){return degreeToChord(key,DegreeChord(number,seventh?Chord::Seventh:0));}
};

class MajorScale : public ScaleBase {
public:
    static const uint16_t diatonicOptions[];
    static const uint16_t diatonicSeventhOptions[];
    String name() {return "Major";};
    static const uint8_t pitch[];
    // Chord degreeToChord(uint8_t key, DegreeChord degree, Chord base = Chord()) override;
    Chord getDiatonic(uint8_t key, uint8_t number, bool seventh, Chord base = Chord()) override;
};

class MinorScale : public ScaleBase {
public:
    static const uint16_t diatonicOptions[];
    static const uint16_t diatonicSeventhOptions[];
    String name() {return "Minor";};
    static const uint8_t pitch[];
    // Chord degreeToChord(uint8_t key, DegreeChord degree, Chord base = Chord()) override;
    Chord getDiatonic(uint8_t key, uint8_t number, bool seventh, Chord base = Chord()) override;
};

//Chordクラスと同様に現在使っているスケールを管理するためのクラス
class Scale {
protected:
    static std::vector<std::shared_ptr<ScaleBase>> availableScales;
public:
    Scale();
    Scale(uint8_t key);
    uint8_t key = 0; //主音 C=0
    ScaleBase *currentScale;

    Chord degreeToChord(DegreeChord degree);
    Chord getDiatonic(uint8_t number, bool seventh, Chord base = Chord());
    String toString();
    static std::vector<std::shared_ptr<ScaleBase>> getAvailableScales();
    int getScaleIndex();
    int getScaleIndexFromName(String scaleStr);
    ScaleBase *getScaleFromName(String scaleStr);

    void serialize(OutputArchive &archive,const char *key) {
        archive.pushNest(key);
        archive("Key",this->key);
        archive("Scale",currentScale->name());
        archive.popNest();
    }
    void deserialize(InputArchive &archive,const char *key) {
        archive.pushNest(key);
        archive("Key",std::forward<uint8_t>(this->key));
        // Find scale which has its name
        String scaleStr = "";
        archive("Scale",std::forward<String>(scaleStr));
        currentScale = getScaleFromName(scaleStr);
        archive.popNest();
    }
};

#endif