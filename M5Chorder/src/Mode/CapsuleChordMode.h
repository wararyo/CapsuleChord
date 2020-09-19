#ifndef _CAPSULECHORDMODE_H_
#define _CAPSULECHORDMODE_H_

#include "KeyModeBase.h"
#include "Keypad.h"
#include "Chord.h"

class CapsuleChordMode : public KeyModeBase {
public:
    using KeyModeBase::KeyModeBase;
    void update() override;
};

#endif