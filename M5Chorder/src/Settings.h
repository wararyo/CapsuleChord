#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <M5Stack.h>
#undef min
#include <vector>
#include <stdio.h>
#include "Archive.h"
#include "Chord.h"
#include "Scale.h"

const String jsonFilePath = "/capsulechord/settings.json";

class SettingItem {
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
        InputArchive archive = InputArchive();
        archive.fromJSON(output);
        uint jsonVersion = 0;
        archive("version",std::forward<uint>(jsonVersion));
        // if(version > jsonVersion) migration.invoke(jsonVersion);
        archive(name,*this);
        return true;
    }
    bool save(String path = jsonFilePath){
        //Serialize
        OutputArchive archive = OutputArchive();
        archive("version",std::forward<uint>(version));
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
};

class SettingItemScale : public SettingItem {
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
};

class SettingItemNumeric : public SettingItem {
public:
    int number;
    SettingItemNumeric(const char *name,int min, int max, int number)
        : SettingItem(name), number(number) {}
        void serialize(OutputArchive &archive,const char *key) override {
        archive(name,std::forward<int>(number));
    }
    void deserialize(InputArchive &archive,const char *key) override {
        archive(name,std::forward<int>(number));
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
};

#endif