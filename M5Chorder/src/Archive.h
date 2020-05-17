#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

#include <ArduinoJson.h>
#undef min
#include <vector>
#include <string>
#include <stdio.h>

#define AUTO_NVP(T) #T, T

const int maxJsonCapacity = JSON_OBJECT_SIZE(64);
const int maxJsonFileSize = 2048;

class OutputArchive;

struct is_serializable_internally_impl {
    template <class T>
    static auto check(OutputArchive doc, const char * name,T&& x)->decltype(x.serialize(doc,name),x.deserialize(doc,name),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};
struct is_serializable_externally_impl {
    template <class T>
    static auto check(OutputArchive doc, const char * name,T&& x)->decltype(serialize(doc,name,x),deserialize(doc,name,x),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};
struct is_serializable_impl {
    template <class T>
    static auto check(OutputArchive doc, const char * name,T&& x)->decltype(x.serialize(doc,name),x.deserialize(doc,name),std::true_type{});
    template <class T>
    static auto check(OutputArchive doc, const char * name,T&& x)->decltype(serialize(doc,name,x),deserialize(doc,name,x),std::true_type{});
    template <class T>
    static auto check(...)->std::false_type;
};

// Serializable internally or externally
template <class T>
class is_serializable :
    public decltype(is_serializable_impl::check<T>(std::declval<OutputArchive>(), std::declval<const char *>(), std::declval<T>())){};

template <class T>
class is_serializable_internally :
    public decltype(is_serializable_internally_impl::check<T>(std::declval<OutputArchive>(), std::declval<const char *>(), std::declval<T>())){};
template <class T>
class is_serializable_externally :
    public decltype(is_serializable_externally_impl::check<T>(std::declval<OutputArchive>(), std::declval<const char *>(), std::declval<T>())){};

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

/* Serialization of Types */

//const char *
void serialize(OutputArchive &archive,const char *key,const char *string);
void deserialize(OutputArchive &archive,const char *key,const char *string);

//int
void serialize(OutputArchive &archive,const char *key,int number);
void deserialize(OutputArchive &archive,const char *key,int number);

//std::vector
template <class T, class A>
typename std::enable_if<is_serializable<T>::value, void>::type serialize(OutputArchive &archive,const char *key,std::vector<T, A> list){
    for(int i = 0;i < list.size();i++) {
        archive(AUTO_NVP(list[i]));
    }
}
template <class T, class A>
typename std::enable_if<is_serializable<T>::value, void>::type deserialize(OutputArchive &archive,const char *key,std::vector<T, A> list){

}

//pointer
template <class T>
typename std::enable_if<is_serializable<T>::value, void>::type serialize(OutputArchive &archive,const char *key,T* ptr){
    archive(key,*ptr);
}
template <class T>
typename std::enable_if<is_serializable<T>::value, void>::type deserialize(OutputArchive &archive,const char *key,T* ptr){

}

#endif