#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

#include <ArduinoJson.h>
#include <M5Stack.h> // for debuging
#undef min
#include <vector>
#include <string>
#include <stdio.h>

#define AUTO_NVP(T) #T, T

const int maxJsonCapacity = JSON_OBJECT_SIZE(64);
const int maxJsonFileSize = 2048;

/* Meta functions */

class OutputArchive;
class InputArchive;

struct is_serializable_internally_impl {
    template <class T>
    static auto check(OutputArchive oa,InputArchive ia, const char * name,T&& x)->decltype(x.serialize(oa,name),x.deserialize(ia,name),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};
struct is_serializable_externally_impl {
    template <class T>
    static auto check(OutputArchive oa,InputArchive ia, const char * name,T&& x)->decltype(serialize(oa,name,std::forward<T>(x)),deserialize(ia,name,std::forward<T>(x)),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};
struct is_serializable_impl {
    template <class T>
    static auto check(OutputArchive oa,InputArchive ia, const char * name,T&& x)->decltype(x.serialize(oa,name),x.deserialize(ia,name),std::true_type{});
    template <class T>
    static auto check(OutputArchive oa,InputArchive ia, const char * name,T&& x)->decltype(serialize(oa,name,std::forward<T>(x)),deserialize(ia,name,std::forward<T>(x)),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};

// Serializable internally or externally
template <class T>
class is_serializable :
    public decltype(is_serializable_impl::check<T>(std::declval<OutputArchive>(), std::declval<InputArchive>(), std::declval<const char *>(), std::declval<T>())){};

template <class T>
class is_serializable_internally :
    public decltype(is_serializable_internally_impl::check<T>(std::declval<OutputArchive>(), std::declval<InputArchive>(), std::declval<const char *>(), std::declval<T>())){};
template <class T>
class is_serializable_externally :
    public decltype(is_serializable_externally_impl::check<T>(std::declval<OutputArchive>(), std::declval<InputArchive>(), std::declval<const char *>(), std::declval<T>())){};


/* Archive class */

class OutputArchive {
private:
    std::vector<JsonObject> nestStack;
    DynamicJsonDocument doc = DynamicJsonDocument(maxJsonCapacity);
public:
    OutputArchive() {
        nestStack.push_back(doc.to<JsonObject>());
    }
    template <class T, typename std::enable_if<is_serializable_internally<T>::value, std::nullptr_t>::type = nullptr>
    inline void operator()(const char *key,T && arg) {
        arg.serialize(*this,key);
    }
    template <class T, typename std::enable_if<is_serializable_externally<T>::value, std::nullptr_t>::type = nullptr>
    inline void operator()(const char *key,T && arg) {
        serialize(*this,key,std::forward<T>(arg));
    }
    void pushNest(const char *key) {
        nestStack.push_back(nestStack[nestStack.size()-1].createNestedObject(key));
    }
    void popNest() {
        if(!nestStack.empty()) nestStack.pop_back();
    }
    JsonObject getDocument(){
        return nestStack[nestStack.size()-1];
    }
    void toJSON(char *out){
        char output[maxJsonFileSize];
        serializeJson(doc,output);
        out = std::move(output);
    }
};

class InputArchive {
private:
    std::vector<JsonObject> nestStack;
    DynamicJsonDocument doc = DynamicJsonDocument(maxJsonCapacity);
public:
    InputArchive() {
        nestStack.push_back(doc.to<JsonObject>());
    }
    template <class T, typename std::enable_if<is_serializable_internally<T>::value, std::nullptr_t>::type = nullptr>
    inline void operator()(const char *key,T && arg) { 
        arg.deserialize(*this,key);
    }
    template <class T, typename std::enable_if<is_serializable_externally<T>::value, std::nullptr_t>::type = nullptr>
    inline void operator()(const char *key,T && arg) {
        deserialize(*this,key,std::forward<T>(arg));
    }
    void pushNest(const char *key) {
        JsonObject top = nestStack[nestStack.size()-1];
        if(top.containsKey(key)){
            JsonObject obj = top[key];
            if(obj.size() > 0) nestStack.push_back(obj);
        }
    }
    void popNest() {
        if(!nestStack.empty()) nestStack.pop_back();
    }
    JsonObject getDocument(){
        return nestStack[nestStack.size()-1];
    }
    void fromJSON(const char *in){
        deserializeJson(doc,in);
    }
};


/* Serialization of Types */

//const char *
void serialize(OutputArchive &archive,const char *key,const char *string);
void deserialize(InputArchive &archive,const char *key,const char * && string);

//String
void serialize(OutputArchive &archive,const char *key,String string);
void deserialize(InputArchive &archive,const char *key,String && string);

//int
void serialize(OutputArchive &archive,const char *key,int number);
void deserialize(InputArchive &archive,const char *key,int && number);

//float
void serialize(OutputArchive &archive,const char *key,float number);
void deserialize(InputArchive &archive,const char *key,float && number);

//bool
void serialize(OutputArchive &archive,const char *key,bool value);
void deserialize(InputArchive &archive,const char *key,bool && value);

//std::vector
template <class T, class A>
void serialize(OutputArchive &archive,const char *key,std::vector<T, A> list){
    archive.pushNest(key);
    for(int i = 0;i < list.size();i++) {
        archive(AUTO_NVP(list[i]));
    }
    archive.popNest();
}
template <class T, class A>
void deserialize(InputArchive &archive,const char *key,std::vector<T, A> && list){
    archive.pushNest(key);
    for(int i = 0;i < list.size();i++) {
        archive(AUTO_NVP(std::forward<T>(list[i])));
    }
    archive.popNest();
}

//pointer
template <class T>
void serialize(OutputArchive &archive,const char *key,T* ptr){
    archive(key,*ptr);
}
template <class T>
void deserialize(InputArchive &archive,const char *key,T* ptr){
    archive(key,*ptr);
}

#endif