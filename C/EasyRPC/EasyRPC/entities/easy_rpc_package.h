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
uint8_t getNameLen(EasyRPCPackage *pkg) {
	uint8_t total = 0;
	while (pkg->functionName[total] != 0) {
		total++;
	}
	return total;
}
/* ---------------------------------------------------------------------------*/
#endif