#ifndef _BUILD_PACKAGE_FROM_CLIENT_H
#define _BUILD_PACKAGE_FROM_CLIENT_H
/* ---------------------------------------------------------------------------*/
#include "entities/easy_rpc_package_base.h"
/* ---------------------------------------------------------------------------*/
bool easyRPC_ProcessDataFromClient(Stream* stream, EasyRPCPackage* packageReturn) {
	resetEasyRPC_Package(packageReturn);
	if (!getString(stream, &packageReturn->functionName[0])) {
		return false;
	}
	if (!readByte(stream, (uint8_t*)&packageReturn->returnInfo->type)) {
		return false;
	}

	return true;
}
/* ---------------------------------------------------------------------------*/
void buildPackageFromClient_Setup() {
	easyRPC_ProcessData = easyRPC_ProcessDataFromClient;
}
/* ---------------------------------------------------------------------------*/
#endif
