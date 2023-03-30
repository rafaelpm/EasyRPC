#ifndef _HELPERS_H
#define _HELPERS_H
/* ---------------------------------------------------------------------------*/
#include "package_builders/build_package_from_client.h"
#include "package_builders/build_package_to_client.h"
/* ---------------------------------------------------------------------------*/
DataInfo paramTemp;
void makeFunction(EasyRPCPackage *package, TypeData typeReturn, char *nameFunction) {
    resetEasyRPC_Package(package);
    setEasyRPC_NameFunction(package, nameFunction);
    package->returnInfo.type = typeReturn;
}
/* ---------------------------------------------------------------------------*/
void setParamFloat(EasyRPCPackage* package, float value) {
    setEasyRPC_DataInfo_Float(value, &paramTemp);
    setEasyRPC_Param(package, &paramTemp);
}
/* ---------------------------------------------------------------------------*/
void setParamInt(EasyRPCPackage* package, int value) {
    setEasyRPC_DataInfo_Int(value, &paramTemp);
    setEasyRPC_Param(package, &paramTemp);
}
/* ---------------------------------------------------------------------------*/
void setParamBoolean(EasyRPCPackage* package, bool value) {
    setEasyRPC_DataInfo_Boolean(value, &paramTemp);
    setEasyRPC_Param(package, &paramTemp);
}
/* ---------------------------------------------------------------------------*/
void setParamString(EasyRPCPackage* package, char *value) {
    setEasyRPC_DataInfo_String(value, &paramTemp);
    setEasyRPC_Param(package, &paramTemp);
}
/* ---------------------------------------------------------------------------*/
void setParamArray(EasyRPCPackage* package, uint8_t* value, uint16_t len) {
    setEasyRPC_DataInfo_Array(value, len, &paramTemp);
    setEasyRPC_Param(package, &paramTemp);
}
/* ---------------------------------------------------------------------------*/
#endif
