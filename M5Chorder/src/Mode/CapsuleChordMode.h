#ifndef _CAPSULECHORDMODE_H_
#define _CAPSULECHORDMODE_H_

#include "ModeBase.h"
#include "Keypad.h"
#include "Chord.h"

class CapsuleChordMode : public ModeBase {
public:
    void update() override;
};

#endif