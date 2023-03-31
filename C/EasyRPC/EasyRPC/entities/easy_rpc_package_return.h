#ifndef _EASY_RPC_PACKAGE_RETURN_H
#define _EASY_RPC_PACKAGE_RETURN_H
/* ---------------------------------------------------------------------------*/
#include "data_info.h"

/* ---------------------------------------------------------------------------*/
void setEasyRPC_Return_Float(EasyRPCPackage* package, float value) {
	setEasyRPC_DataInfo_Float(value, &package->returnInfo);
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Return_Float(EasyRPCPackage* package, float* value) {	
	if(package->returnInfo.type != Float) {
		return false;
	}
	*value = atof(package->returnInfo.value);
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Return_Integer(EasyRPCPackage* package, int value) {
	setEasyRPC_DataInfo_Int(value, &package->returnInfo);	
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Return_Integer(EasyRPCPackage* package, int* value) {	
	if (package->returnInfo.type != Integer) {
		return false;
	}
	*value = atoi(package->returnInfo.value);
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Return_Boolean(EasyRPCPackage* package, bool value) {
	setEasyRPC_DataInfo_Boolean(value, &package->returnInfo);	
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Return_Boolean(EasyRPCPackage* package, bool* value) {	
	if (package->returnInfo.type != Boolean) {
		return false;
	}
	*value = (package->returnInfo.value[0] == 't');
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Return_String(EasyRPCPackage* package, char* value) {
	setEasyRPC_DataInfo_String(value, &package->returnInfo);
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Return_String(EasyRPCPackage* package, char* value) {	
	if (package->returnInfo.type != String) {
		return false;
	}
	memcpy(value, &package->returnInfo.value[0], package->returnInfo.valueSize);
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Return_Array(EasyRPCPackage* package, uint8_t* value, uint16_t len) {
	setEasyRPC_DataInfo_Array(value, len, &package->returnInfo);	
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Return_Array(EasyRPCPackage* package, uint8_t* value) {	
	if (package->returnInfo.type != BinaryArray) {
		return false;
	}
	memcpy(value, &package->returnInfo.value[0], package->returnInfo.valueSize);
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif