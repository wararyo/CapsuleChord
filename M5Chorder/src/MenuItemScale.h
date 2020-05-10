#ifndef _MENUITEMSCALE_H_
#define _MENUITEMSCALE_H_

/*
MenuItemScale is based on MenuItemNumeric.
https://github.com/lovyan03/M5Stack_TreeView/blob/master/src/MenuItemNumeric.h
*/

#include <MenuItemSpinner.h>
#include <Scale.h>

class MenuItemScale : public MenuItemSpinner {
public:
  MenuItemScale(const String& title, int value, int tg = 0, TCallBackEnter cb = 0)
  : MenuItemSpinner(title,Scale::getAvailableScales().size() - 1, value, tg, cb) {};

  MenuItemScale(const String& title, int value, TCallBackEnter cb)
  : MenuItemSpinner(title,Scale::getAvailableScales().size() - 1, value, cb) {};

  String getStringOfItem(int value) override;
};

#endif