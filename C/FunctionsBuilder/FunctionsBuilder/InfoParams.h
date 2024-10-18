#ifndef _INFO_PARAMS_H
#define _INFO_PARAMS_H
#include <stdio.h>
/* ---------------------------------------------------------------------------*/
#define VERSION "1.0.1"

#ifndef uint8_t
#define uint8_t unsigned char
#endif

enum {
    Both = 0,
    Server,
    Client
} ParamModes;

#define SIZE_PATH 255

struct {
    uint8_t mode;
    char fileIn[SIZE_PATH];
    char pathOut[SIZE_PATH];
    
} Params;

void printVersion(){
    printf("FunctionsBuilder - v%s\n",VERSION);
    
}
/* ---------------------------------------------------------------------------*/
int getParam(int argc, char *argv[], char *paramName, char *errorNotDefined){
    for(int i=1; i < argc; i++){
        if(strcmp(argv[i],paramName) == 0){
            if(i+1 >= argc){
                printf("%s\n",errorNotDefined);
                return -1;
            }
            i++;
            return i;
        }
    }
    return 0;
}
/* ---------------------------------------------------------------------------*/
bool loadParams(int argc, char *argv[]){
    memset(&Params,0,sizeof(Params));
    Params.mode = Both;
    //Check mode
    int index = getParam(argc, argv,(char *)"-mode",(char *)"Mode not defined!");
    if(index == -1){
        return false;
    }
    if(index > 0){
        if(strcmp(argv[index],"server") == 0){
            Params.mode = Server;
        }else if(strcmp(argv[index],"client") == 0){
            Params.mode = Client;
        }else if(strcmp(argv[index],"both") == 0){
            Params.mode = Both;
        }else{
            printf("Mode not found: %s\n",argv[index]);
            return false;
        }
    }
    //File in
    index = getParam(argc, argv,(char *)"-in",(char *)"File input not defined!");
    if(index == -1){
        return false;
    }
    if(index == 0){
        printf("Please, insert file input with functions name, parameters and return!\n");
        return false;
    }
    if(strlen(argv[index]) > SIZE_PATH){
        printf("File input cannot exced %d caracters\n",SIZE_PATH);
        return false;
    }    
    strcpy(Params.fileIn,argv[index]);

    //File out
    index = getParam(argc, argv,(char *)"-out",(char *)"File output not defined!");
    if(index == -1){
        return false;
    }
    if(index == 0){
        printf("Please, insert path to output!\n");
        return false;
    }
    if(strlen(argv[index]) > SIZE_PATH){
        printf("File output cannot exced %d caracters\n",SIZE_PATH);
        return false;
    }
    strcpy(Params.pathOut,argv[index]);    
    if(strlen(Params.pathOut) == 1 && Params.pathOut[0] == '.'){
        Params.pathOut[0] = '\0';
    }
                    
    return true;
}
/* ---------------------------------------------------------------------------*/
void printParams(){
    printf("-mode: server, client or both. Default: both\n");
    printf("-in: file .h with functions name, parameters and return\n");
    printf("-out: path to save file\n");
    printf("Example: \n");
    printf("FunctionsBuilder -mode server -in myFunctions.h -out . \n");
    printf("FunctionsBuilder -mode client -in myFunctions.h -out . \n");
    printf("FunctionsBuilder -mode both -in myFunctions.h -out . \n");
    printf("FunctionsBuilder -in myFunctions.h -out . \n");
}
/* ---------------------------------------------------------------------------*/
#endif