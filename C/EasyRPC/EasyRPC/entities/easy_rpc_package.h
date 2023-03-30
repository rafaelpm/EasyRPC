#ifndef _EASY_RPC_PACKAGE_H
#define _EASY_RPC_PACKAGE_H
/* ---------------------------------------------------------------------------*/
#include "data_info.h"

#define EASY_RPC_PACKAGE_FUNCTION_NAME_SIZE 50
#define EASY_RPC_PACKAGE_MAX_PARAMS 50

typedef struct {
	uint8_t functionName[EASY_RPC_PACKAGE_FUNCTION_NAME_SIZE];
	DataInfo returnInfo;
	int totalParams;
	DataInfo params[EASY_RPC_PACKAGE_MAX_PARAMS];
} EasyRPCPackage;
/* ---------------------------------------------------------------------------*/
void resetEasyRPC_Package(EasyRPCPackage* pkg) {
	memset(&pkg->functionName[0], 0, sizeof(EasyRPCPackage));
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Param(EasyRPCPackage* pkg, DataInfo *param) {
	if (pkg->totalParams + 1 >= EASY_RPC_PACKAGE_MAX_PARAMS) {
		return;
	}
	memcpy(&pkg->params[pkg->totalParams], param, sizeof(DataInfo));
	pkg->totalParams++;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_NameFunction(EasyRPCPackage* pkg, char* name) {
	size_t len = strlen(name);
	for (int i = 0; i < len; i++) {
		pkg->functionName[i] = (uint8_t)name[i];
	}
	pkg->functionName[len] = 0;
}
/* ---------------------------------------------------------------------------*/
uint8_t getEasyRPC_NameFunctionLen(EasyRPCPackage *pkg) {
	uint8_t total = 0;
	while (pkg->functionName[total] != 0) {
		total++;
	}
	return total;
}
/* ---------------------------------------------------------------------------*/
#endif