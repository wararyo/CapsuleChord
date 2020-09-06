#ifndef _MODEBASE_H_
#define _MODEBASE_H_

#include <memory>
#include <vector>
// #include "CapsuleChordMode.h"
#include "Context.h"

class ModeBase {
protected:
  static std::vector<std::shared_ptr<ModeBase>> availableModes;
public:
  virtual void update() {}
  virtual void setting() {}
  Context *context;

  ModeBase() {
    context = Context::getContext();
  }

  static std::vector<std::shared_ptr<ModeBase>> getAvailableModes() {
    // if(availableModes.empty()) {
    //     availableModes.push_back(std::make_shared<CapsuleChordMode>());
    // }
    return availableModes;
  }
};

#endif