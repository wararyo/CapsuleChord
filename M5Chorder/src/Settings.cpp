#include <Settings.h>
#include <string>

void serialize(OutputArchive &archive,const char *key,const char *string){
    archive.doc[key] = string;
}
void deserialize(OutputArchive &archive,const char *key,const char *string){
}

void serialize(OutputArchive &archive,const char *key,int number){
    char str[32];
    itoa(number,str,10);
    archive.doc[key] = std::move(str);
}
void deserialize(OutputArchive &archive,const char *key,int number){
}

// template <class T, typename std::enable_if<is_serializable<T>::value, std::nullptr_t>::type = nullptr>
// void serialize(OutputArchive &archive,const char *key,std::vector<T, std::allocator<T>> list){
// }
// template <class T, typename std::enable_if<is_serializable<T>::value, std::nullptr_t>::type = nullptr>
// void deserialize(OutputArchive &archive,const char *key,std::vector<T, std::allocator<T>> list){

// }