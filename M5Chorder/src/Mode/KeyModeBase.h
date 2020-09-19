#ifndef _KEYMODEBASE_H_
#define _KEYMODEBASE_H_

#include "Context.h"

class KeyModeBase {
public:
  virtual void update() {}
  virtual void setting() {}
  Context *context;

  KeyModeBase() {
    context = Context::getContext();
  }
};

#endif