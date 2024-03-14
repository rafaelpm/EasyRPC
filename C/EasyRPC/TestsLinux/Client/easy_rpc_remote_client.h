#ifndef _EASY_RPC_REMOTE_CLIENT
#define _EASY_RPC_REMOTE_CLIENT
/* ---------------------------------------------------------------------------*/
#include "client/easy_rpc_client.h"
#include "package_builders/build_package_to_client.h"
#include "package_builders/build_package_from_client_or_server.h"
/* ---------------------------------------------------------------------------*/
bool remote_sum(int *returnValue, int a, int b){
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { return false; }
	}

	//Build package
	resetEasyRPC_Package(&easyRPC_clientPackage);
	setEasyRPC_NameFunction(&easyRPC_clientPackage, (char *)"sum");
	easyRPC_clientPackage.returnInfo.type = Integer;
	setEasyRPC_Param_Integer(&easyRPC_clientPackage, a);
	setEasyRPC_Param_Integer(&easyRPC_clientPackage, b);

	//Prepare stream to send
	Stream streamData, streamToServer, streamFromServer;
	resetStream(&streamToServer); resetStream(&streamFromServer);
	easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);
	wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

	//Send and receive data
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { return false; }

	//Check Ack
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { 
		printf("1) Fail ACK!\n");
		return false; 
	}
	printf("Check ACK!\n");		
	
	EasyRPCPackage packageFromServer;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!isACK_EasyRPC()){ 
		printf("2) Fail ACK!\n");
		return false; 
	}

	resetStream(&streamFromServer);	
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { return false; }

	unwrapPosition = 0;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!getEasyRPC_Return_Integer(&packageFromServer, returnValue)) { return false; }
	easyRPC_ClientConnection_Disconnect();
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif