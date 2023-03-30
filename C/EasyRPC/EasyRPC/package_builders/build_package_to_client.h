#ifndef _BUILD_PACKAGE_TO_CLIENT_H
#define _BUILD_PACKAGE_TO_CLIENT_H
/* ---------------------------------------------------------------------------*/
#include "entities/easy_rpc_package_base.h"
/* ---------------------------------------------------------------------------*/
bool easyRPC_toBytesToClient(EasyRPCPackage *package, Stream* streamReturn) {
	resetStream(streamReturn);
	if (!setString(streamReturn, &package->functionName[0], getNameLen(package))) {
		return false;
	}
	if (!writeByte(streamReturn, package->returnInfo->type)) {
		return false;
	}

	return true;
}
/* ---------------------------------------------------------------------------*/
void buildPackageToClient_Setup() {
	easyRPC_ToBytes = easyRPC_toBytesToClient;
}
/* ---------------------------------------------------------------------------*/
#endif
