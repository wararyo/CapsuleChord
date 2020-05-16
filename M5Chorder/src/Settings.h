#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <M5Stack.h>
#include <ArduinoJson.h>
#include <ArduinoJson/Object/ObjectRef.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>
#undef min
#include <vector>
#include <stdio.h>

const int maxJsonCapacity = JSON_OBJECT_SIZE(64);
const int maxJsonFileSize = 2048;
const String jsonFilePath = "/capsulechord/settings.json";

#define AUTO_NVP(T) #T, T

class OutputArchive;

struct is_serializable_internally_impl {
    template <class T>
    static auto check(const char * name,T&& x,OutputArchive doc)->decltype(x.serialize(doc,name),x.deserialize(doc,name),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};
struct is_serializable_externally_impl {
    template <class T>
    static auto check(const char * name,T&& x, OutputArchive doc)->decltype(serialize(doc,name,x),deserialize(doc,name,x),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};

// Serializable internally or externally (disjuction cant be used)
template <class T>
class is_serializable :
    public decltype(
        is_serializable_internally_impl::check<T>(std::declval<const char *>(),std::declval<T>(),std::declval<OutputArchive>()) ? 
        std::true_type{} :
        is_serializable_externally_impl::check<T>(std::declval<const char *>(),std::declval<T>(),std::declval<OutputArchive>())
        ) {};
template <class T>
class is_serializable_internally :
    public decltype(is_serializable_internally_impl::check<T>(std::declval<const char *>(),std::declval<T>(),std::declval<OutputArchive>())){};
template <class T>
class is_serializable_externally :
    public decltype(is_serializable_externally_impl::check<T>(std::declval<const char *>(),std::declval<T>(),std::declval<OutputArchive>())){};

template <class T, typename std::enable_if<is_serializable<T>::value, std::nullptr_t>::type = nullptr>
class SettingItem {
public:
    const char *name;
    std::vector<SettingItem*> children;
    T content;
    SettingItem(const char *name,T content) {
        this->name = name;
        this->content = content;
    }
    SettingItem(const char *name,std::vector<SettingItem*> children){
        this->name = name;
        this->children = children;
    }
    void serialize(OutputArchive &archive,const char *key) {
        if(children.empty()) {
            Serial.printf("SettingItem#serialize(%s,content)",name);
            archive(name,content);
        } else {
            Serial.printf("SettingItem#serialize(%s,children)",name);
            // archive(name,children);
        }
    }
    void deserialize(OutputArchive &archive,const char *key) {
        if(children.empty()) {
            archive(name,content);
        } else {
            archive(name,children);
        }
    }
};

void serialize(OutputArchive &archive,const char *key,const char *string);
void deserialize(OutputArchive &archive,const char *key,const char *string);
void serialize(OutputArchive &archive,const char *key,int number);
void deserialize(OutputArchive &archive,const char *key,int number);
template <class T, class A>
typename std::enable_if<is_serializable<T>::value, void>::type serialize(OutputArchive &archive,const char *key,std::vector<T, A> list){
    for(int i = 0;i < list.size();i++) {
        char str[10];
        sprintf(str,"item %d",i);
        archive(str,list[i]);
    }
}
template <class T, class A>
typename std::enable_if<is_serializable<T>::value, void>::type deserialize(OutputArchive &archive,const char *key,std::vector<T, A> list){

}

class OutputArchive {
private:
public:
    DynamicJsonDocument doc = DynamicJsonDocument(maxJsonCapacity);
    OutputArchive() {
    }
    template <class T, typename std::enable_if<is_serializable_internally<T>::value, std::nullptr_t>::type = nullptr>
    inline void operator()(const char *key,T && arg) {
        arg.serialize(*this,key);
    }
    template <class T, typename std::enable_if<is_serializable_externally<T>::value, std::nullptr_t>::type = nullptr>
    inline void operator()(const char *key,T && arg) {
        serialize(*this,key,std::forward<T>(arg));
    }
    void toJSON(char *out){
        char output[maxJsonFileSize];
        serializeJson(doc,output);
        out = std::move(output);
    }
};

class Settings : public SettingItem<int> {
public:
    Settings(std::vector<SettingItem*> items) : SettingItem<int>("Settings",items){}
    void load(String path){
    }
    void save(String path = jsonFilePath){
        OutputArchive archive = OutputArchive();
        std::vector<int> vec = std::vector<int>{1,2,3,4};
        // archive("test","tanosiine");
        archive(name,vec);
        //archive(name,*this);
        char output[maxJsonFileSize];
        archive.toJSON(output);
        Serial.println(output);
        // DynamicJsonDocument doc(maxJsonCapacity);
        // doc["value"] = "value";
        // has_serialize<const char*> hoge = "value";
        // // Serialize
        // char output[maxJsonFileSize];
        // serializeJson(doc,output);
        // File file = SD.open(path,FILE_WRITE);
        // file.print(output);
        // file.close();
    }
};

#endif