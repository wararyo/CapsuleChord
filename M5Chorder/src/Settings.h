#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <M5Stack.h>
#undef min
#include <vector>
#include <stdio.h>
#include "Archive.h"

const String jsonFilePath = "/capsulechord/settings.json";

class SettingItem {
public:
    const char *name;
    std::vector<SettingItem*> children;
    SettingItem(){}
    SettingItem(const char *name,std::vector<SettingItem*> children){
        this->name = name;
        this->children = children;
    }
    virtual void serialize(OutputArchive &archive,const char *key) {
        if(children.empty()) {
            archive(name,"Empty Content");
        } else {
            archive(name,children);
        }
    }
    virtual void deserialize(OutputArchive &archive,const char *key) {
        if(children.empty()) {
            //Deserialize nothing
        } else {
            archive(name,children);
        }
    }
};

class Settings : public SettingItem {
public:
    Settings(std::vector<SettingItem*> items) : SettingItem("Settings",items){}
    void load(String path){
    }
    void save(String path = jsonFilePath){
        OutputArchive archive = OutputArchive();
        archive(name,*this);
        char output[maxJsonFileSize];
        archive.toJSON(output);
        Serial.println(output);
        // File file = SD.open(path,FILE_WRITE);
        // file.print(output);
        // file.close();
    }
};

class SettingItemString : public SettingItem {
public:
    const char *content;
    SettingItemString(const char *name,const char *content){
        this->name = name;
        this->content = content;
    }
    void serialize(OutputArchive &archive,const char *key) override {
        archive(name,content);
    }
    void deserialize(OutputArchive &archive,const char *key) override {
        archive(name,content);
    }
};

#endif