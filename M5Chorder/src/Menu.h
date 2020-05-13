#ifndef _MENU_H_
#define _MENU_H_

#include "Settings.h"
#include <M5Stack.h>
#include <M5TreeView.h>
#include <MenuItemToggle.h>
#include <MenuItemNumeric.h>
#include "MenuItem/Key.h"
#include "MenuItem/Scale.h"

typedef std::vector<MenuItem*> vmi;

class FunctionMenu {
private:
    Settings *settings;
    // void callBackKey(MenuItem* sender) {
    //     MenuItemKey* mi((MenuItemKey*)sender);
    //     settings->scale.key = mi->value;
    // }

    // void callBackScale(MenuItem* sender) {
    //     MenuItemKey* mi((MenuItemKey*)sender);
    //     settings->scale.currentScale = Scale::getAvailableScales()[mi->value].get();
    // }

    // void callBackSeventh(MenuItem* sender) {
    //     MenuItemToggle* mi((MenuItemToggle*)sender);
    //     seventh = mi->value;
    // }

    // void callBackCenterNoteNo(MenuItem* sender) {
    //     MenuItemNumeric* mi((MenuItemNumeric*)sender);
    //     centerNoteNo = mi->value;
    // }
public:
    M5TreeView treeview;

    FunctionMenu() {
        treeview.clientRect.x = 60;
        treeview.clientRect.y = 16;
        treeview.clientRect.w = 240;
        treeview.clientRect.h = 200;
        treeview.itemWidth = 192;
        treeview.itemHeight = 24;
        treeview.setTextFont(2);
        treeview.setItems(vmi{
            // new MenuItemKey("Key", scale.key, callBackKey),
            // new MenuItemScale("Scale", 0, callBackScale),
            // new MenuItemNumeric("CenterNoteNo",24,81,centerNoteNo,callBackCenterNoteNo)
        });
    }

    void begin(Settings *settings) {
        treeview.begin();
        this->settings = settings;
    }

    void update(){
        treeview.update();//Contains M5.update(); (probably...)
    }
    // True when the menu wants to exit
    bool isExitRequired() {
        return M5.BtnA.wasPressed() && (M5TreeView::getFocusItem()->parentItem() == &treeview);
    }
};

extern FunctionMenu Menu;

#endif