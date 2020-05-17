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
            archive(name,std::forward<decltype(children)>(children));
        }
    }
    virtual void deserialize(InputArchive &archive,const char *key) {
        if(archive.getDocument().containsKey(key))
        {
            archive(name,std::forward<decltype(children)>(children));
        }
    }
};

class Settings : public SettingItem {
public:
    Settings(std::vector<SettingItem*> items) : SettingItem("Settings",items){}
    void load(String path = jsonFilePath){
        char output[maxJsonFileSize] = {'\0'};
        File file = SD.open(path);
        if(file){
            file.read((uint8_t *)output,maxJsonFileSize);
            file.close();
        }
        Serial.println(output);
        InputArchive archive = InputArchive();
        archive.fromJSON(output);
        archive(name,*this);
    }
    void save(String path = jsonFilePath){
        OutputArchive archive = OutputArchive();
        archive(name,*this);
        char output[maxJsonFileSize];
        archive.toJSON(output);
        Serial.println(output);
        File file = SD.open(path,FILE_WRITE);
        if(!file) Serial.println("Something wrong happened with saving settings.");
        file.print(output);
        file.close();
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
    void deserialize(InputArchive &archive,const char *key) override {
        archive(name,std::forward<const char *>(content));
    }
};

#endif