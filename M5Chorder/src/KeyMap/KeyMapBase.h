#ifndef _KEYMAPBASE_H_
#define _KEYMAPBASE_H_

#include "Context.h"

class KeyMapBase {
public:
  virtual void update() {}
  virtual void setting() {}
  Context *context;

  KeyMapBase() {
    context = Context::getContext();
  }
};

#endif