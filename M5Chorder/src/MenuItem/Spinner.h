#ifndef _MENUITEMSPINNER_H_
#define _MENUITEMSPINNER_H_

/*
MenuItemSpinner is based on MenuItemNumeric.
https://github.com/lovyan03/M5Stack_TreeView/blob/master/src/MenuItemNumeric.h
*/

#include <MenuItem.h>
#include <WString.h>

class MenuItemSpinner : public MenuItem {
public:
  int value = 0;
  bool canLoop = true;

  MenuItemSpinner(const String& title, int maximum, int value, int tg = 0, TCallBackEnter cb = 0)
  : MenuItem(title, tg, cb), value(value), maximum(maximum) {};

  MenuItemSpinner(const String& title, int maximum, int value, TCallBackEnter cb)
  : MenuItem(title, cb), value(value), maximum(maximum) {};

  virtual void onAfterDraw();
  virtual void onEnter();

  virtual String getStringOfItem(int value) {return String(value);}
  
  void setValue(int value);
protected:
  int minimum = 0;
  int maximum = 1;
  void drawNum(int value, int flg);
};

#endif