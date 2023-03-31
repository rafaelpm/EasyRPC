#ifndef _HELPERS_H
#define _HELPERS_H
/* ---------------------------------------------------------------------------*/
#include "package_builders/build_package_from_client.h"
#include "package_builders/build_package_to_client.h"
/* ---------------------------------------------------------------------------*/
Stream streamToServer, streamFromClient;
EasyRPCPackage packageToServer, packageFromClient;

char nameFunction1[10] = "func1";
char nameFunction2[10] = "func2";
char nameFunction3[10] = "func3";

void makeFunction(EasyRPCPackage *package, TypeData typeReturn, char *nameFunction) {
    resetEasyRPC_Package(package);
    setEasyRPC_NameFunction(package, nameFunction);
    package->returnInfo.type = typeReturn;
}
/* ---------------------------------------------------------------------------*/
void test_set_get_string_array(char* nameFunction, char *valueParam1, uint8_t *valueParam2, int sizeParam2) {
    char valueCompare1[50];
    uint8_t valueCompare2[50];

    makeFunction(&packageToServer, Void, nameFunction);
    setEasyRPC_Param_String(&packageToServer, valueParam1);
    setEasyRPC_Param_Array(&packageToServer, valueParam2, sizeParam2);

    if (!easyRPC_toBytesToClient(&packageToServer, &streamToServer)) {
        set_red("test_set_get_string_array->easyRPC_toBytesToClient->Error");
    }

    if (!easyRPC_ProcessDataFromClient(&streamToServer, &packageFromClient)) {
        set_red("test_set_get_string_array->easyRPC_ProcessDataFromClient->Error");
    }

    if (memcmp(packageFromClient.functionName, nameFunction, strlen(nameFunction)) != 0) {
        set_red("test_set_get_string_array->functionName->Error");
    } else if (packageFromClient.returnInfo.type != Void) {
        set_red("test_set_get_string_array->returnInfo->Type->Error");
    } else if (packageFromClient.totalParams != 2) {
        set_red("test_set_get_string_array->totalParams->Error");
    }
    if (!getEasyRPC_Param_String(&packageFromClient, &valueCompare1[0], 0)) {
        set_red("test_set_get_string_array->getEasyRPC_Param_Float->Error");
    } else if (memcmp(&valueCompare1[0],valueParam1,strlen(valueParam1)) == 0){
        set_red("test_set_get_string_array->getEasyRPC_Param_Float->Compare1->Error");
    }

    if (!getEasyRPC_Param_Array(&packageFromClient, &valueCompare2[0], 1)) {
        set_red("test_set_get_string_array->getEasyRPC_Param_Float->Error");
    } else if (memcmp(&valueCompare2[0], valueParam2, sizeParam2) == 0) {
        set_red("test_set_get_string_array->getEasyRPC_Param_Float->Compare2->Error");
    }

    set_green("test_set_get_string_array = OK");
}
/* ---------------------------------------------------------------------------*/
void test_set_get_bool(char* nameFunction, bool valueParam1, int valueParam2) {
    bool valueCompare1 = false;
    int valueCompare2 = 0;

    makeFunction(&packageToServer, Boolean, nameFunction);
    setEasyRPC_Param_Boolean(&packageToServer, valueParam1);
    setEasyRPC_Param_Integer(&packageToServer, valueParam2);

    if (!easyRPC_toBytesToClient(&packageToServer, &streamToServer)) {
        set_red("test_set_get_bool->easyRPC_toBytesToClient->Error");
    }

    if (!easyRPC_ProcessDataFromClient(&streamToServer, &packageFromClient)) {
        set_red("test_set_get_bool->easyRPC_ProcessDataFromClient->Error");
    }

    if (memcmp(packageFromClient.functionName, nameFunction, strlen(nameFunction)) != 0) {
        set_red("test_set_get_bool->functionName->Error");
    } else if (packageFromClient.returnInfo.type != Boolean) {
        set_red("test_set_get_bool->returnInfo->Type->Error");
    } else if (packageFromClient.totalParams != 2) {
        set_red("test_set_get_bool->totalParams->Error");
    }
    if (!getEasyRPC_Param_Boolean(&packageFromClient, &valueCompare1, 0)) {
        set_red("test_set_get_bool->getEasyRPC_Param_Float->Error");
    } else if (valueCompare1 != valueParam1) {
        set_red("test_set_get_bool->getEasyRPC_Param_Float->Compare1->Error");
    }

    if (!getEasyRPC_Param_Integer(&packageFromClient, &valueCompare2, 1)) {
        set_red("test_set_get_bool->getEasyRPC_Param_Float->Error");
    } else if (valueCompare2 != valueParam2) {
        set_red("test_set_get_bool->getEasyRPC_Param_Float->Compare2->Error");
    }

    set_green("test_set_get_bool = OK");
}
/* ---------------------------------------------------------------------------*/
void test_set_get_int(char* nameFunction, int valueParam1, float valueParam2) {
    int valueCompare1 = 0;
    float valueCompare2 = 0;

    makeFunction(&packageToServer, Integer, nameFunction);
    setEasyRPC_Param_Integer(&packageToServer, valueParam1);
    setEasyRPC_Param_Float(&packageToServer, valueParam2);

    if (!easyRPC_toBytesToClient(&packageToServer, &streamToServer)) {
        set_red("test_set_get_int->easyRPC_toBytesToClient->Error");
    }

    if (!easyRPC_ProcessDataFromClient(&streamToServer, &packageFromClient)) {
        set_red("test_set_get_int->easyRPC_ProcessDataFromClient->Error");
    }

    if (memcmp(packageFromClient.functionName, nameFunction, strlen(nameFunction)) != 0) {
        set_red("test_set_get_int->functionName->Error");
    } else if (packageFromClient.returnInfo.type != Integer) {
        set_red("test_set_get_int->returnInfo->Type->Error");
    } else if (packageFromClient.totalParams != 2) {
        set_red("test_set_get_int->totalParams->Error");
    }
    if (!getEasyRPC_Param_Integer(&packageFromClient, &valueCompare1, 0)) {
        set_red("test_set_get_int->getEasyRPC_Param_Float->Error");
    } else if (valueCompare1 != valueParam1) {
        set_red("test_set_get_int->getEasyRPC_Param_Float->Compare1->Error");
    }

    if (!getEasyRPC_Param_Float(&packageFromClient, &valueCompare2, 1)) {
        set_red("test_set_get_int->getEasyRPC_Param_Float->Error");
    } else if (valueCompare2 != valueParam2) {
        set_red("test_set_get_int->getEasyRPC_Param_Float->Compare2->Error");
    }

    set_green("test_set_get_int = OK");
}
/* ---------------------------------------------------------------------------*/
void test_set_get_float(char* nameFunction, float valueParam1, float valueParam2) {
    float valueCompare = 0;

    makeFunction(&packageToServer, Float, nameFunction);
    setEasyRPC_Param_Float(&packageToServer, valueParam1);
    setEasyRPC_Param_Float(&packageToServer, valueParam2);

    if (!easyRPC_toBytesToClient(&packageToServer, &streamToServer)) {
        set_red("test_set_get_float->easyRPC_toBytesToClient->Error");
    }

    if (!easyRPC_ProcessDataFromClient(&streamToServer, &packageFromClient)) {
        set_red("test_set_get_float->easyRPC_ProcessDataFromClient->Error");
    }

    if (memcmp(packageFromClient.functionName, nameFunction, strlen(nameFunction)) != 0) {
        set_red("test_set_get_float->functionName->Error");
    } else if (packageFromClient.returnInfo.type != Float) {
        set_red("test_set_get_float->returnInfo->Type->Error");
    } else if (packageFromClient.totalParams != 2) {
        set_red("test_set_get_float->totalParams->Error");
    }
    if (!getEasyRPC_Param_Float(&packageFromClient, &valueCompare, 0)) {
        set_red("test_set_get_float->getEasyRPC_Param_Float->Error");
    } else if (valueCompare != valueParam1) {
        set_red("test_set_get_float->getEasyRPC_Param_Float->Compare1->Error");
    }

    if (!getEasyRPC_Param_Float(&packageFromClient, &valueCompare, 1)) {
        set_red("test_set_get_float->getEasyRPC_Param_Float->Error");
    } else if (valueCompare != valueParam2) {
        set_red("test_set_get_float->getEasyRPC_Param_Float->Compare2->Error");
    }

    set_green("test_set_get_float = OK");
}
/* ---------------------------------------------------------------------------*/
#endif
