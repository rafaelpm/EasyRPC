#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "libdebug.h"
#include "package_builders/build_package_from_client.h"
#include "package_builders/build_package_to_client.h"
/* ---------------------------------------------------------------------------*/
int main(int argv, char *argc)
{
    Stream streamToServer, streamFromClient;
    EasyRPCPackage packageToServer, packageFromClient;

    resetEasyRPC_Package(&packageToServer);
    resetEasyRPC_Package(&packageFromClient);

    memcpy(&packageToServer.functionName[0], "sum", 3);
    packageToServer.returnInfo->type = Float;

    if (!easyRPC_toBytesToClient(&packageToServer, &streamToServer)) {
        set_red("easyRPC_toBytesToClient->Error");
    }

    if (!easyRPC_ProcessDataFromClient(&streamToServer, &packageFromClient)) {
        set_red("easyRPC_ProcessDataFromClient->Error");
    }

    if (memcmp(packageFromClient.functionName, "sum", 3) != 0) {        
        set_red("functionName->Error");
    } else if (packageFromClient.returnInfo->type != Float) {
        set_red("returnInfo->Type->Error");
    }

    set_green("OK");

    return 0;
}
/* ---------------------------------------------------------------------------*/