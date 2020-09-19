#ifndef _KEYMODE_H_
#define _KEYMODE_H_

#include <memory>
#include <vector>
#include "KeyModeBase.h"
#include "CapsuleChordMode.h"

class KeyMode {
protected:
  static std::vector<std::shared_ptr<KeyModeBase>> availableModes;
public:
    static std::vector<std::shared_ptr<KeyModeBase>> getAvailableModes() {
    if(availableModes.empty()) {
        availableModes.push_back(std::make_shared<CapsuleChordMode>());
    }
    return availableModes;
  }
};

#endif