#ifndef _MENUITEMDEGREECHORD_H_
#define _MENUITEMDEGREECHORD_H_

#include <MenuItem.h>
#include "Chord.h"

class MenuItemDegreeChord : public MenuItem {
public:
  DegreeChord value;

  MenuItemDegreeChord(const String& title, DegreeChord value, int tg = 0, TCallBackEnter cb = 0)
  : MenuItem(title, tg, cb), value(value) {};

  MenuItemDegreeChord(const String& title, DegreeChord value, TCallBackEnter cb)
  : MenuItem(title, cb), value(value) {};

  virtual void onAfterDraw();
  virtual void onEnter();
  void setValue(DegreeChord value);
private:
  void drawChord(DegreeChord value, int flg);
  void drawWindow(DegreeChord value);
};

#endif