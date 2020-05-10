/*
MenuItemKey is based on MenuItemNumeric.
https://github.com/lovyan03/M5Stack_TreeView/blob/master/src/MenuItemNumeric.h
*/

#ifndef _MENUITEMKEY_H_
#define _MENUITEMKEY_H_

#include "MenuItemSpinner.h"
#include "Chord.h"

class MenuItemKey : public MenuItemSpinner {
public:

  MenuItemKey(const String& title, int value, int tg = 0, TCallBackEnter cb = 0)
  : MenuItemSpinner(title, Chord::rootStrings.size()-1, value, tg, cb){};

  MenuItemKey(const String& title, int value, TCallBackEnter cb)
  : MenuItemSpinner(title, Chord::rootStrings.size()-1, value, cb){};

  String getStringOfItem(int value) override;
};

#endif