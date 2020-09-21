#ifndef _MENUITEMENUM_H_
#define _MENUITEMENUM_H_

#include <MenuItem/Spinner.h>

class MenuItemEnum : public MenuItemSpinner {
public:
    std::vector<const char *> member;
    MenuItemEnum(const String& title, std::vector<const char *> member, int value, int tg = 0, TCallBackEnter cb = 0)
    : MenuItemSpinner(title,member.size() - 1, value, tg, cb), member(member) {};

    MenuItemEnum(const String& title, std::vector<const char *> member, int value, TCallBackEnter cb)
    : MenuItemSpinner(title,member.size() - 1, value, cb), member(member) {};

    String getStringOfItem(int value) override;
};

#endif