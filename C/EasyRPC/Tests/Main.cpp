#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#include <stdio.h>
#include <string.h>
#include "libdebug.h"
#include "Helpers.h"
#include "package_builders/build_package_from_client.h"
#include "package_builders/build_package_to_client.h"
/* ---------------------------------------------------------------------------*/
int main(int argv, char *argc)
{
    Stream streamToServer, streamFromClient;
    EasyRPCPackage packageToServer, packageFromClient;
    DataInfo param1, param2;
    char nameFunction[4] = "sum";
    
    makeFunction(&packageToServer, Float, nameFunction);
    setParamFloat(&packageToServer, 1.5);

    if (!easyRPC_toBytesToClient(&packageToServer, &streamToServer)) {
        set_red("easyRPC_toBytesToClient->Error");
    }

    if (!easyRPC_ProcessDataFromClient(&streamToServer, &packageFromClient)) {
        set_red("easyRPC_ProcessDataFromClient->Error");
    }

    if (memcmp(packageFromClient.functionName, "sum", 3) != 0) {        
        set_red("functionName->Error");
    } else if (packageFromClient.returnInfo.type != Float) {
        set_red("returnInfo->Type->Error");
    } else if (packageFromClient.totalParams != 1) {
        set_red("totalParams->Error");
    }

    set_green("OK");

    return 0;
}
/* ---------------------------------------------------------------------------*/