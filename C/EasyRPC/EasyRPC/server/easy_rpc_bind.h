#ifndef _EASY_RPC_BIND_H
#define _EASY_RPC_BIND_H
/* ---------------------------------------------------------------------------*/

#define EASY_RPC_FUNCTION_NOT_FOUND -1
#ifndef EASY_RPC_MAX_FUNCTIONS
	#warning "Defina um valor para EASY_RPC_MAX_FUNCTIONS"
#endif
#define EASY_RPC_MAX_SIZE_NAME_FUNCTIONS 128
struct {
	char functions[EASY_RPC_MAX_FUNCTIONS][EASY_RPC_MAX_SIZE_NAME_FUNCTIONS];
	int totalFunctions;
} EasyRPCBind;

#include "../entities/easy_rpc_package.h"

#define EASY_RPC_BIND_RESET memset(&EasyRPCBind.functions[0], 0, sizeof(EasyRPCBind));
/* ---------------------------------------------------------------------------*/
typedef bool (*EasyRPC_RunFunctionCallback)(EasyRPCPackage* package, int index);
EasyRPC_RunFunctionCallback easyRPC_RunFunction;
int getEasyRPC_SearchFunctionIndexByPackage(EasyRPCPackage* package);
/* ---------------------------------------------------------------------------*/
bool easyRPC_ProcessPackageBind(EasyRPCPackage* package) {
	int index = getEasyRPC_SearchFunctionIndexByPackage(package);
	if (index == EASY_RPC_FUNCTION_NOT_FOUND) {		
		#ifdef _CALCULE_TIME
		printf("easyRPC_ProcessPackageBind = EASY_RPC_FUNCTION_NOT_FOUND\n");
		#endif
		return false;
	}
	if (easyRPC_RunFunction != NULL) {		
		easyRPC_RunFunction(package, index);
	}else{
		printf("easyRPC_RunFunction = NULL\n");
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
bool addEasyRPC_FunctionBind(char* name) {
	if (EasyRPCBind.totalFunctions + 1 > EASY_RPC_MAX_FUNCTIONS) {
		//printf("add false (%d) = %s\n",EasyRPCBind.totalFunctions, name);
		return false;
	}
	//printf("add(%d): %s\n",EasyRPCBind.totalFunctions,name);
	memcpy(&EasyRPCBind.functions[EasyRPCBind.totalFunctions][0], name, strlen(name));
	EasyRPCBind.totalFunctions++;
	return true;
}
/* ---------------------------------------------------------------------------*/
int getEasyRPC_SearchFunctionIndexByName(char* nameFunction) {	
	uint16_t sizeNameFunction = (uint16_t)strlen(nameFunction);
	for (int i = 0; i < EasyRPCBind.totalFunctions; i++) {		
		//printf("SearchName: %s = %s ?\n",&EasyRPCBind.functions[i][0], nameFunction);
		if (memcmp(&EasyRPCBind.functions[i][0], nameFunction, sizeNameFunction) == 0) {
			return i;
		}
	}
	return EASY_RPC_FUNCTION_NOT_FOUND;
}
/* ---------------------------------------------------------------------------*/
int getEasyRPC_SearchFunctionIndexByPackage(EasyRPCPackage* package) {
	return getEasyRPC_SearchFunctionIndexByName((char *) &package->functionName[0]);
}
/* ---------------------------------------------------------------------------*/
#endif