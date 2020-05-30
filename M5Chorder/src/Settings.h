#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <M5Stack.h>
#undef min
#include <vector>
#include <stdio.h>
#include "Archive.h"
#include "Chord.h"
#include "Scale.h"
#include <MenuItem.h>
#include <MenuItemToggle.h>
#include <MenuItemNumeric.h>
#include "MenuItem/Scale.h"
#include "MenuItem/Key.h"
#include "MenuItem/Enum.h"

#define MAX_NEST_SIZE 16

const String jsonFilePath = "/capsulechord/settings.json";

class SettingItem {
protected:
    // Scheme for executing member functions when the value of TreeView is changed
    static const int MaxTagCount = 128;
    static SettingItem* tags[MaxTagCount];
    int obtainTag() { //Tagを発行する
        for(int i = 0;i < MaxTagCount;i++) {
            if(tags[i] == nullptr){
                tags[i] = this;
                return i;
            }
        }
        return -1;
    }
    void clearTag(int tag){ //Tagを開放する
        tags[tag] = nullptr;
    }
    static void treeViewFunc(MenuItem* mi) {
        if(tags[mi->tag] != nullptr) tags[mi->tag]->onTreeViewValueChanged(mi);
    }
    virtual void onTreeViewValueChanged(MenuItem* mi) {}
public:
    const char *name;
    std::vector<SettingItem*> children;
    SettingItem(){}
    SettingItem(const char *name): name(name){}
    SettingItem(const char *name,std::vector<SettingItem*> children){
        this->name = name;
        this->children = children;
    }
    virtual void serialize(OutputArchive &archive,const char *key) {
        if(children.empty()) {
            archive(name,"Empty Content");
        } else {
            archive(name,std::forward<decltype(children)>(children));
        }
    }
    virtual void deserialize(InputArchive &archive,const char *key) {
        if(archive.getDocument().containsKey(key))
        {
            archive(name,std::forward<decltype(children)>(children));
        }
    }
    virtual MenuItem *getTreeView() {
        if(children.empty()) return nullptr;
        MenuItem *mi = new MenuItem(name,std::vector<MenuItem*>());
        for(auto i : children) {
            mi->addItem(i->getTreeView());
        }
        return mi;
    }
};

class Settings : public SettingItem {
private:
    uint version;
public:
    Settings(std::vector<SettingItem*> items,uint version=1) : SettingItem("Settings",items),version(version){}
    bool load(String path = jsonFilePath){
        //Read file
        char output[maxJsonFileSize] = {'\0'};
        File file = SD.open(path);
        if(!file) return false;
        file.read((uint8_t *)output,maxJsonFileSize);
        file.close();
        //Deserialize
        Serial.println("Start deserialization");
        InputArchive archive = InputArchive();
        archive.fromJSON(output);
        uint jsonVersion = 0;
        archive("Version",std::forward<uint>(jsonVersion));
        Serial.printf("Setting file version = %d",jsonVersion);
        // if(version > jsonVersion) migration.invoke(jsonVersion);
        archive(name,*this);
        return true;
    }
    bool save(String path = jsonFilePath){
        //Serialize
        OutputArchive archive = OutputArchive();
        archive("Version",std::forward<uint>(version));
        archive(name,*this);
        char output[maxJsonFileSize];
        archive.toJSON(output,true);
        //Write to file
        File file = SD.open(path,FILE_WRITE);
        if(!file) {
            Serial.println("Something wrong happened with saving settings.");
            return false;
        }
        file.print(output);
        file.close();
        return true;
    }
    SettingItem *findSettingByKey(String query){
        String keys[MAX_NEST_SIZE] = {String("\0")};
        // Split query by '/'
        int index = 0;
        for(int i = 0; i < query.length(); i++){
            char tmp = query.charAt(i);
            if(tmp == '/') {
                index++;
                if(index > (MAX_NEST_SIZE - 1)) return nullptr;
            }
            else keys[index] += tmp;
        }
        // Find item
        SettingItem * cursor = this;
        for(String key : keys){
            if(key == String("\0")) break;
            auto children = cursor->children;
            for(SettingItem *k : cursor->children){
                if(String(k->name) == key){
                    cursor = k;
                    goto next_key; // forループを一気に抜けるための使用ならバチは当たらないはず…
                }
            }
            return nullptr;
next_key: ;
        }
        return cursor;
    }
};

// class SettingItemString : public SettingItem {
// public:
//     const char *content;
//     SettingItemString(const char *name,const char *content){
//         this->name = name;
//         this->content = content;
//     }
//     void serialize(OutputArchive &archive,const char *key) override {
//         archive(name,content);
//     }
//     void deserialize(InputArchive &archive,const char *key) override {
//         archive(name,std::forward<const char *>(content));
//     }
// };

class SettingItemDegreeChord : public SettingItem {
public:
    DegreeChord content;
    SettingItemDegreeChord(const char *name,DegreeChord content){
        this->name = name;
        this->content = content;
    }
    void serialize(OutputArchive &archive,const char *key) override {
        archive(name,content);
    }
    void deserialize(InputArchive &archive,const char *key) override {
        archive(name,std::forward<DegreeChord>(content));
    }
    MenuItem *getTreeView() override {
        return new MenuItemToggle(name,false);
    }
};

class SettingItemScale : public SettingItem {
private:
    int keyTag;
    int scaleTag;
public:
    Scale content;
    SettingItemScale(const char *name,Scale content){
        this->name = name;
        this->content = content;
    }
    void serialize(OutputArchive &archive,const char *key) override {
        archive(name,content);
    }
    void deserialize(InputArchive &archive,const char *key) override {
        archive(name,std::forward<Scale>(content));
    }

    void onTreeViewValueChanged(MenuItem* mi) override {
        if(mi->tag == keyTag){
            content.key = ((MenuItemKey*)mi)->value;
        }
        else if(mi->tag == scaleTag){
            content.currentScale = Scale::getAvailableScales()[((MenuItemScale*)mi)->value].get();
        }
    }

    MenuItem *getTreeView() override {
        MenuItem *mi = new MenuItem(name,std::vector<MenuItem*>());
        keyTag = obtainTag();
        scaleTag = obtainTag();
        mi->addItem(new MenuItemKey("Key",content.key,keyTag,treeViewFunc));
        mi->addItem(new MenuItemScale("Scale",content.getScaleIndex(),scaleTag,treeViewFunc));
        return mi;
    }
};

class SettingItemBoolean : public SettingItem {
public:
    bool content;
    SettingItemBoolean(const char *name,bool content)
        : SettingItem(name), content(content) {}
    void serialize(OutputArchive &archive,const char *key) override {
        archive(name,content);
    }
    void deserialize(InputArchive &archive,const char *key) override {
        archive(name,std::forward<bool>(content));
    }
    void onTreeViewValueChanged(MenuItem* mi) override {
        content = ((MenuItemToggle*)mi)->value;
    }
    MenuItem *getTreeView() override {
        return new MenuItemToggle(name,content,obtainTag(),treeViewFunc);
    }
};

class SettingItemNumeric : public SettingItem {
public:
    int number;
    int min;
    int max;
    SettingItemNumeric(const char *name,int min, int max, int number)
        : SettingItem(name), number(number), min(min), max(max) {}
    void serialize(OutputArchive &archive,const char *key) override {
        archive(name,std::forward<int>(number));
    }
    void deserialize(InputArchive &archive,const char *key) override {
        archive(name,std::forward<int>(number));
    }
    void onTreeViewValueChanged(MenuItem* mi) override {
        number = ((MenuItemNumeric*)mi)->value;
    }
    MenuItem *getTreeView() override {
        return new MenuItemNumeric(name,min,max,number,obtainTag(),treeViewFunc);
    }
};

class SettingItemEnum : public SettingItem {
public:
    std::vector<const char *> memberNames;
    uint8_t index;
    SettingItemEnum(const char *name, std::vector<const char *> memberNames, uint8_t index)
        : SettingItem(name), memberNames(memberNames), index(index) {}

    void serialize(OutputArchive &archive,const char *key) override {
        archive(name,memberNames[index]);
    }
    void deserialize(InputArchive &archive,const char *key) override {
        String memberName = "";
        archive(name,std::forward<String>(memberName));
        for(int i = 0;i < memberNames.size();i++){
            String m = String(memberNames[i]);
            if(String(m).equals(memberName)) {
                index = i;
            }
        }
    }
    void onTreeViewValueChanged(MenuItem* mi) override {
        index = ((MenuItemEnum*)mi)->value;
    }
    MenuItem *getTreeView() override {
        return new MenuItemEnum(name,memberNames,index,obtainTag(),treeViewFunc);
    }
};

#endif