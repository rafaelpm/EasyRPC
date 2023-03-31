#ifndef _HELPER_BIND_H
#define _HELPER_BIND_H
/* ---------------------------------------------------------------------------*/
#include "Helpers.h"
#include "server/easy_rpc_bind.h"
/* ---------------------------------------------------------------------------*/

int sum_bind1(int param1, float param2) {
	return param1 + param2;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_RunFunctionApp(EasyRPCPackage* package, int index) {
	int param1 = 0;
	float param2 = 0;
	int res;
	switch (index) {
	case 0:		
		if (!getEasyRPC_Param_Integer(package, &param1, 0)) {
			return false;
		}
		if (!getEasyRPC_Param_Float(package, &param2, 1)) {
			return false;
		}
		res = sum_bind1(param1, param2);
		setEasyRPC_Return_Integer(package, res);		
		return true;
	default:
		break;
	}
	return false;
}
/* ---------------------------------------------------------------------------*/
void test_bind() {
	EASY_RPC_BIND_RESET;
	test_set_get_int(nameFunction1, 3, 3.5);
	addEasyRPC_FunctionBind(&nameFunction1[0]);
	easyRPC_RunFunction = easyRPC_RunFunctionApp;

	if (!easyRPC_ProcessPackageBind(&packageFromClient)) {
		set_red("TestBind = ERROR");
	}
	int res = 0;
	if (!getEasyRPC_Return_Integer(&packageFromClient, &res)) {
		set_red("TestBind->CheckReturn = ERROR");
	} else if (res != 6) {
		set_red("TestBind->CheckReturn->Value = ERROR");
	}

	set_green("TestBind = OK");
}
/* ---------------------------------------------------------------------------*/
void test_bind_search_function() {
	EASY_RPC_BIND_RESET;

	addEasyRPC_FunctionBind(&nameFunction1[0]);
			
	if (getEasyRPC_SearchFunctionIndexByName(&nameFunction2[0]) != EASY_RPC_FUNCTION_NOT_FOUND) {
		set_red("getEasyRPC_FunctionIndex->NotFound->Error");
	} else if (getEasyRPC_SearchFunctionIndexByName(&nameFunction1[0]) == EASY_RPC_FUNCTION_NOT_FOUND) {
		set_red("getEasyRPC_FunctionIndex->Found->Error");
	}

	makeFunction(&packageToServer, Void, &nameFunction1[0]);
	if (getEasyRPC_SearchFunctionIndexByPackage(&packageToServer) == EASY_RPC_FUNCTION_NOT_FOUND) {
		set_red("getEasyRPC_FunctionIndex->Package->Found->Error");
	} 

	makeFunction(&packageToServer, Void, &nameFunction2[0]);
	if (getEasyRPC_SearchFunctionIndexByPackage(&packageToServer) != EASY_RPC_FUNCTION_NOT_FOUND) {
		set_red("getEasyRPC_FunctionIndex->Package->NotFound->Error");
	}

	set_green("getEasyRPC_FunctionIndex = OK");
}
/* ---------------------------------------------------------------------------*/
#endif