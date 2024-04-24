#ifndef _EASY_RPC_PACKAGE_PARAMS_H
#define _EASY_RPC_PACKAGE_PARAMS_H
/* ---------------------------------------------------------------------------*/
#include "data_info.h"

void setEasyRPC_Param(EasyRPCPackage* pkg, DataInfo* param) {
	if (pkg->totalParams + 1 >= EASY_RPC_PACKAGE_MAX_PARAMS) {
		return;
	}
	memcpy(&pkg->params[pkg->totalParams], param, sizeof(DataInfo));
	pkg->totalParams++;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Param_Float(EasyRPCPackage* package, float value) {
	setEasyRPC_DataInfo_Float(value, &easyPackageParamTemp);
	setEasyRPC_Param(package, &easyPackageParamTemp);
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Param_Float(EasyRPCPackage* package, float* value, int position) {
	if (position >= package->totalParams) {
		return false;
	}
	if (package->params[position].type != Float) {
		return false;
	}
	*value = atof(package->params[position].value);
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Param_Integer(EasyRPCPackage* package, int value) {
	setEasyRPC_DataInfo_Int(value, &easyPackageParamTemp);
	setEasyRPC_Param(package, &easyPackageParamTemp);
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Param_Integer(EasyRPCPackage* package, int* value, int position) {
	if (position >= package->totalParams) {		
		return false;
	}
	if (package->params[position].type != Integer) {		
		return false;
	}
	*value = atoi(package->params[position].value);
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Param_Boolean(EasyRPCPackage* package, bool value) {
	setEasyRPC_DataInfo_Boolean(value, &easyPackageParamTemp);
	setEasyRPC_Param(package, &easyPackageParamTemp);
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Param_Boolean(EasyRPCPackage* package, bool* value, int position) {
	if (position >= package->totalParams) {
		return false;
	}
	if (package->params[position].type != Boolean) {
		return false;
	}
	*value = (package->params[position].value[0] == 't');
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Param_String(EasyRPCPackage* package, char* value) {
	setEasyRPC_DataInfo_String(value, &easyPackageParamTemp);
	setEasyRPC_Param(package, &easyPackageParamTemp);
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Param_String(EasyRPCPackage* package, char* value, int position) {	
	memset(value, 0, sizeof(value));
	if (position >= package->totalParams) {		
		return false;
	}
	if (package->params[position].type != StringRPC) {		
		return false;
	}	
	memcpy(value, &package->params[position].value[0], package->params[position].valueSize);
//	package->params[position].value[package->params[position].valueSize] = 0;
	return true;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_Param_Array(EasyRPCPackage* package, uint8_t* value, uint16_t len) {
	setEasyRPC_DataInfo_Array(value, len, &easyPackageParamTemp);
	setEasyRPC_Param(package, &easyPackageParamTemp);
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_Param_Array(EasyRPCPackage* package, uint8_t* value, uint16_t *len, int position) {
	if (position >= package->totalParams) {
		return false;
	}
	if (package->params[position].type != BinaryArray) {
		return false;
	}
	memcpy(value, &package->params[position].value[0], package->params[position].valueSize);
	*len = package->params[position].valueSize;
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif