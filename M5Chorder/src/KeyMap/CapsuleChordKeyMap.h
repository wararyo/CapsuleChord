#ifndef _CAPSULECHORDKEYMAP_H_
#define _CAPSULECHORDKEYMAP_H_

#include "KeyMapBase.h"
#include "Keypad.h"
#include "Chord.h"

class CapsuleChordKeyMap : public KeyMapBase {
public:
    using KeyMapBase::KeyMapBase;
    void update() override;
    static const uint8_t numberKeyMap[]; //Key and diatonic chord matching
};

#endif