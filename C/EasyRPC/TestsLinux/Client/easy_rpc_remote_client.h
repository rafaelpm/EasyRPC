#ifndef _EASY_RPC_REMOTE_CLIENT
#define _EASY_RPC_REMOTE_CLIENT
/* ---------------------------------------------------------------------------*/
#include "client/easy_rpc_client.h"
#include "package_builders/build_package_to_client.h"
#include "package_builders/build_package_from_client_or_server.h"
/* ---------------------------------------------------------------------------*/
bool remote_sum(int *returnValue, int a, int b){
	if (!easyRPC_ClientConnection_IsConnected()) {
		printf("Is not connected!\n");
	  if (!easyRPC_ClientConnection_Connect()) { 
		printf("Cannot connect!\n");
		return false; 
	  }
	}

	//Build package
	printf("Build package!\n");
	resetEasyRPC_Package(&easyRPC_clientPackage);
	setEasyRPC_NameFunction(&easyRPC_clientPackage, (char *)"sum");
	easyRPC_clientPackage.returnInfo.type = Integer;
	setEasyRPC_Param_Integer(&easyRPC_clientPackage, a);
	setEasyRPC_Param_Integer(&easyRPC_clientPackage, b);

	//Prepare stream to send
	printf("Prepare stream to send\n");
	Stream streamData, streamToServer, streamFromServer;
	resetStream(&streamToServer); resetStream(&streamFromServer);
	easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);
	wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

	//Send and receive data
	printf("Send and receive data\n");
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { 
		printf("Fail send\n");
		return false; 
	}
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { 
		printf("Fail receive\n");
		return false; 
	}
	EasyRPCPackage packageFromServer;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!getEasyRPC_Return_Integer(&packageFromServer, returnValue)) { 
		printf("Fail unwrap return\n");
		return false; 
	}

	printf("Disconnect\n");
	easyRPC_ClientConnection_Disconnect();
	return true;
}
/* ---------------------------------------------------------------------------*/
bool remote_subtract(float *returnValue, float a, float b){
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { return false; }
	}

	//Build package
	resetEasyRPC_Package(&easyRPC_clientPackage);
	setEasyRPC_NameFunction(&easyRPC_clientPackage, (char *)"subtract");
	easyRPC_clientPackage.returnInfo.type = Float;
	setEasyRPC_Param_Float(&easyRPC_clientPackage, a);
	setEasyRPC_Param_Float(&easyRPC_clientPackage, b);

	//Prepare stream to send
	Stream streamData, streamToServer, streamFromServer;
	resetStream(&streamToServer); resetStream(&streamFromServer);
	easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);
	wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

	//Send and receive data
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { return false; }
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { return false; }
	EasyRPCPackage packageFromServer;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!getEasyRPC_Return_Float(&packageFromServer, returnValue)) { return false; }
	easyRPC_ClientConnection_Disconnect();
	return true;
}
/* ---------------------------------------------------------------------------*/
bool remote_print(char * message){
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { return false; }
	}

	//Build package
	resetEasyRPC_Package(&easyRPC_clientPackage);
	setEasyRPC_NameFunction(&easyRPC_clientPackage, (char *)"print");
	easyRPC_clientPackage.returnInfo.type = Void;
	setEasyRPC_Param_String(&easyRPC_clientPackage, message);

	//Prepare stream to send
	Stream streamData, streamToServer, streamFromServer;
	resetStream(&streamToServer); resetStream(&streamFromServer);
	easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);
	wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

	//Send and receive data
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { return false; }
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { return false; }
	easyRPC_ClientConnection_Disconnect();
	return true;
}
/* ---------------------------------------------------------------------------*/
bool remote_binaryData(uint8_t * data, uint16_t data_len){
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { return false; }
	}

	//Build package
	resetEasyRPC_Package(&easyRPC_clientPackage);
	setEasyRPC_NameFunction(&easyRPC_clientPackage, (char *)"binaryData");
	easyRPC_clientPackage.returnInfo.type = Void;
	setEasyRPC_Param_Array(&easyRPC_clientPackage, data, data_len);

	//Prepare stream to send
	Stream streamData, streamToServer, streamFromServer;
	resetStream(&streamToServer); resetStream(&streamFromServer);
	easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);
	wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

	//Send and receive data
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { return false; }
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { return false; }
	easyRPC_ClientConnection_Disconnect();
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif