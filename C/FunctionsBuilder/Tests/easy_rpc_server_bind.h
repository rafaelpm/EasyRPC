#ifndef _EASY_RPC_SERVER_BIND_H
#define _EASY_RPC_SERVER_BIND_H
/* ---------------------------------------------------------------------------*/
#ifdef EASY_RPC_MAX_FUNCTIONS
#undef EASY_RPC_MAX_FUNCTIONS
#endif
#define EASY_RPC_MAX_FUNCTIONS 5
//If necessary adjust size, default is 255
//#define EASY_RPC_DATA_INFO_VALUE_SIZE 255
//#define _CALCULE_TIME
//#define DEBUG
//#define BUFFER_UDP_SIZE 1024
/* ---------------------------------------------------------------------------*/
#include "server/easy_rpc_bind.h"
/* ---------------------------------------------------------------------------*/

int sum(int a,int b);
float subtract(float a,float b);
void print(char * message);
void binaryData(uint8_t * data, uint16_t data_len);
/* ---------------------------------------------------------------------------*/
void easyRPC_AddFunctions(){
	addEasyRPC_FunctionBind((char *)"sum");
	addEasyRPC_FunctionBind((char *)"subtract");
	addEasyRPC_FunctionBind((char *)"print");
	addEasyRPC_FunctionBind((char *)"binaryData");
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_RunFunction_sum(EasyRPCPackage* package){
	int param0;
	int param1;
	if(!getEasyRPC_Param_Integer(package,&param0,0)){
	  return false;
	}
	if(!getEasyRPC_Param_Integer(package,&param1,1)){
	  return false;
	}
	int res_sum = sum(param0,param1);
	setEasyRPC_Return_Integer(package, res_sum);
	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_RunFunction_subtract(EasyRPCPackage* package){
	float param0;
	float param1;
	if(!getEasyRPC_Param_Float(package,&param0,0)){
	  return false;
	}
	if(!getEasyRPC_Param_Float(package,&param1,1)){
	  return false;
	}
	float res_subtract = subtract(param0,param1);
	setEasyRPC_Return_Float(package, res_subtract);
	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_RunFunction_print(EasyRPCPackage* package){
	char param0[EASY_RPC_DATA_INFO_VALUE_SIZE];
	memset(&param0[0],0,EASY_RPC_DATA_INFO_VALUE_SIZE);
	if(!getEasyRPC_Param_String(package,&param0[0],0)){
	  return false;
	}
	print(param0);
	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_RunFunction_binaryData(EasyRPCPackage* package){
	uint8_t param0[EASY_RPC_DATA_INFO_VALUE_SIZE];
	memset(&param0[0],0,EASY_RPC_DATA_INFO_VALUE_SIZE);
	uint16_t param0_len;
	if(!getEasyRPC_Param_Array(package,&param0[0], &param0_len,0)){
	  return false;
	}
	binaryData(param0, param0_len);
	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_RunFunctionApp(EasyRPCPackage* package, int index){
	switch(index){
	  case 0:
	    return easyRPC_RunFunction_sum(package);
	  case 1:
	    return easyRPC_RunFunction_subtract(package);
	  case 2:
	    return easyRPC_RunFunction_print(package);
	  case 3:
	    return easyRPC_RunFunction_binaryData(package);
	}
	return false;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_Server_Bind_Functions(){
	easyRPC_RunFunction = easyRPC_RunFunctionApp;
	easyRPC_AddFunctions();
}
/* ---------------------------------------------------------------------------*/
#endif