#ifndef _CAPSULECHORDKEYMAP_H_
#define _CAPSULECHORDKEYMAP_H_

#include "KeyMapBase.h"
#include "Keypad.h"
#include "Chord.h"

class CapsuleChordKeyMap : public KeyMapBase {
public:
    using KeyMapBase::KeyMapBase;
    void update() override;
};

#endif