#ifndef _EASY_RPC_BIND_H
#define _EASY_RPC_BIND_H
/* ---------------------------------------------------------------------------*/
#define EASY_RPC_FUNCTION_NOT_FOUND -1
#define EASY_RPC_MAX_FUNCTIONS 10
#define EASY_RPC_MAX_SIZE_NAME_FUNCTIONS 128
struct {
	char functions[EASY_RPC_MAX_FUNCTIONS][EASY_RPC_MAX_SIZE_NAME_FUNCTIONS];
	int totalFunctions;
} EasyRPCBind;

#define EASY_RPC_BIND_RESET memset(&EasyRPCBind.functions[0], 0, sizeof(EasyRPCBind));
/* ---------------------------------------------------------------------------*/
typedef bool (*EasyRPC_RunFunctionCallback)(EasyRPCPackage* package, int index);
EasyRPC_RunFunctionCallback easyRPC_RunFunction;
int getEasyRPC_SearchFunctionIndexByPackage(EasyRPCPackage* package);
/* ---------------------------------------------------------------------------*/
bool easyRPC_ProcessPackageBind(EasyRPCPackage* package) {
	int index = getEasyRPC_SearchFunctionIndexByPackage(package);
	if (index == EASY_RPC_FUNCTION_NOT_FOUND) {
		return false;
	}
	if (easyRPC_RunFunction != NULL) {
		easyRPC_RunFunction(package, index);
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
bool addEasyRPC_FunctionBind(char* name) {
	if (EasyRPCBind.totalFunctions + 1 >= EASY_RPC_MAX_FUNCTIONS) {
		return false;
	}
	memcpy(&EasyRPCBind.functions[EasyRPCBind.totalFunctions][0], name, strlen(name));
	EasyRPCBind.totalFunctions++;
	return true;
}
/* ---------------------------------------------------------------------------*/
int getEasyRPC_SearchFunctionIndexByName(char* nameFunction) {
	uint16_t sizeNameFunction = (uint16_t)strlen(nameFunction);
	for (int i = 0; i < EasyRPCBind.totalFunctions; i++) {
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