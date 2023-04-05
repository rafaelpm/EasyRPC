#ifndef _BUILD_PACKAGE_TO_CLIENT_H
#define _BUILD_PACKAGE_TO_CLIENT_H
/* ---------------------------------------------------------------------------*/
#include "entities/easy_rpc_package_base.h"
/* ---------------------------------------------------------------------------*/
bool easyRPC_toBytesToClient(EasyRPCPackage* package, Stream* streamReturn);
/* ---------------------------------------------------------------------------*/
bool easyRPC_toBytesToServer(EasyRPCPackage* package, Stream* streamReturn) {
	resetStream(streamReturn);
	if (!setEasyRPC_String(streamReturn, &package->functionName[0], getEasyRPC_NameFunctionLen(package))) {
		return false;
	}
	if (!writeByte(streamReturn, package->returnInfo.type)) {
		return false;
	}
	for (int p = 0; p < package->totalParams; p++) {
		if (!writeByte(streamReturn, package->params[p].type)) {
			return false;
		}
		if (package->params[p].type == BinaryArray) {
			setEasyRPC_BinaryArray(streamReturn, (uint8_t*)&package->params[p].value[0], package->params[p].valueSize);
		} else if (package->params[p].type != Void) {
			setEasyRPC_String(streamReturn, (uint8_t*)&package->params[p].value[0], package->params[p].valueSize);
		}
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_toBytesToClient(EasyRPCPackage *package, Stream* streamReturn) {
	resetStream(streamReturn);
	if (!setEasyRPC_String(streamReturn, &package->functionName[0], getEasyRPC_NameFunctionLen(package))) {
		return false;
	}
	if (!writeByte(streamReturn, package->returnInfo.type)) {
		return false;
	}
	
	if (package->returnInfo.type == BinaryArray) {
		setEasyRPC_BinaryArray(streamReturn, (uint8_t *) &package->returnInfo.value[0], package->returnInfo.valueSize);
	}else if (package->returnInfo.type != Void) {
		setEasyRPC_String(streamReturn, (uint8_t*) &package->returnInfo.value[0], package->returnInfo.valueSize);
	}	

	return true;
}

/* ---------------------------------------------------------------------------*/
#endif
