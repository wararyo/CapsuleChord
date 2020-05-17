#include <Archive.h>

//const char *
void serialize(OutputArchive &archive,const char *key,const char *string){
    archive.getDocument()[key] = string;
}
void deserialize(InputArchive &archive,const char *key,const char * && string){
    if(archive.getDocument().containsKey(key)) {
        string = archive.getDocument()[key];
    }
}

//int
void serialize(OutputArchive &archive,const char *key,int number){
    archive.getDocument()[key] = number;
}
void deserialize(InputArchive &archive,const char *key,int && number){
    if(archive.getDocument().containsKey(key)) {
        number = archive.getDocument()[key];
    }
}