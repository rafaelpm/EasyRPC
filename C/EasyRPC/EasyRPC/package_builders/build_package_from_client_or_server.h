#ifndef _BUILD_PACKAGE_FROM_CLIENT_H
#define _BUILD_PACKAGE_FROM_CLIENT_H
/* ---------------------------------------------------------------------------*/
#include "entities/easy_rpc_package_base.h"
/* ---------------------------------------------------------------------------*/
bool easyRPC_ProcessDataFromClient(Stream* stream, EasyRPCPackage* packageReturn) {
	resetEasyRPC_Package(packageReturn);
	if (!getEasyRPC_String(stream, &packageReturn->functionName[0])) {
		return false;
	}
	if (!readByte(stream, (uint8_t*)&packageReturn->returnInfo.type)) {
		return false;
	}

	int p = 0;
	while (!isReadEOS_Plus(stream, 1)) {
		if (!readByte(stream, (uint8_t *) &packageReturn->params[p].type)) {
			return false;
		}		
		memset(&packageReturn->params[p].value[0], 0, EASY_RPC_DATA_INFO_VALUE_SIZE);
		
		if (packageReturn->params[p].type == BinaryArray) {
			if (!getEasyRPC_BinaryArray(stream, (uint8_t*)&packageReturn->params[p].value[0], &packageReturn->params[p].valueSize)) {
				return false;
			}
		} else if (packageReturn->params[p].type != Void) {
			if (!getEasyRPC_StringLen(stream, (uint8_t *) &packageReturn->params[p].value[0], &packageReturn->params[p].valueSize)) {
				return false;
			}
		}		
		p++;
	}
	packageReturn->totalParams = p;

	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ProcessDataFromServer(Stream* stream, EasyRPCPackage* packageReturn) {
	resetEasyRPC_Package(packageReturn);
	if (!getEasyRPC_String(stream, &packageReturn->functionName[0])) {
		return false;
	}
	if (!readByte(stream, (uint8_t*)&packageReturn->returnInfo.type)) {
		return false;
	}
	
	if (packageReturn->returnInfo.type == BinaryArray) {
		if (!getEasyRPC_BinaryArray(stream, (uint8_t*)&packageReturn->returnInfo.value[0], &packageReturn->returnInfo.valueSize)) {
			return false;
		}
	} else if (packageReturn->returnInfo.type != Void) {
		if (!getEasyRPC_StringLen(stream, (uint8_t*)&packageReturn->returnInfo.value[0], &packageReturn->returnInfo.valueSize)) {
			return false;
		}
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif

