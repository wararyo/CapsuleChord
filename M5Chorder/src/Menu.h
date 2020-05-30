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
    MenuItem* root;

    void initTreeView(){
        treeview.clientRect.x = 60;
        treeview.clientRect.y = 16;
        treeview.clientRect.w = 240;
        treeview.clientRect.h = 200;
        treeview.itemWidth = 192;
        treeview.itemHeight = 24;
        treeview.setTextFont(2);
    }

public:
    M5TreeView treeview;

    FunctionMenu() {}

    void begin(Settings *settings) {
        // Apply setting items to treeview
        this->settings = settings;
        root = settings->getTreeView();
        if(root != nullptr) treeview.setItems(root->Items);//Remove root

        initTreeView();
        treeview.begin();
    }

    void update(bool redraw = false){
        treeview.update(redraw);//Contains M5.update(); (probably...)
    }
    // True when the menu wants to exit
    bool isExitRequired() {
        return M5.BtnA.wasReleased() && !M5.BtnA.wasReleasefor(300) && (M5TreeView::getFocusItem()->parentItem() == &treeview);
    }
    void save() {settings->save();}
    void load() {settings->load();}
};

extern FunctionMenu Menu;

#endif