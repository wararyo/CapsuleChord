#include <Archive.h>

//const char *
void serialize(OutputArchive &archive,const char *key,const char *string){
    archive.getDocument()[key] = string;
}
void deserialize(InputArchive &archive,const char *key,const char *string){
}

//int
void serialize(OutputArchive &archive,const char *key,int number){
    char str[32];
    itoa(number,str,10);
    archive.getDocument()[key] = std::move(str);
}
void deserialize(InputArchive &archive,const char *key,int number){
}