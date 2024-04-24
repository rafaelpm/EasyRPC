#ifndef _EASY_RPC_REMOTE_CLIENT
#define _EASY_RPC_REMOTE_CLIENT
/* ---------------------------------------------------------------------------*/
#include "client/easy_rpc_client.h"
#include "package_builders/build_package_to_client.h"
#include "package_builders/build_package_from_client_or_server.h"
/* ---------------------------------------------------------------------------*/
void easyRPC_Client_BeforeSend(){
	easyRPC_ProcessData = easyRPC_ProcessDataFromServer;
	//PUT YOUR CODE HERE
}
/* ---------------------------------------------------------------------------*/
void easyRPC_Client_AfterSend(){
	easyRPC_ProcessData = easyRPC_ProcessDataFromClient;
	//PUT YOUR CODE HERE
}
/* ---------------------------------------------------------------------------*/
bool remote_sum(int *returnValue, int a, int b){
	easyRPC_Client_BeforeSend();
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { easyRPC_Client_AfterSend(); return false; }
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
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { easyRPC_Client_AfterSend(); return false; }
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	//Check receive ACK
	EasyRPCPackage packageFromServer;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!isACK_EasyRPC()){ easyRPC_Client_AfterSend(); return false; }
	//Read response
	resetStream(&streamFromServer);
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	unwrapPosition = 0;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!getEasyRPC_Return_Integer(&packageFromServer, returnValue)) { easyRPC_Client_AfterSend(); return false; }
	easyRPC_ClientConnection_Disconnect();
	easyRPC_Client_AfterSend();
	return true;
}
/* ---------------------------------------------------------------------------*/
bool remote_subtract(float *returnValue, float a, float b){
	easyRPC_Client_BeforeSend();
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { easyRPC_Client_AfterSend(); return false; }
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
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { easyRPC_Client_AfterSend(); return false; }
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	//Check receive ACK
	EasyRPCPackage packageFromServer;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!isACK_EasyRPC()){ easyRPC_Client_AfterSend(); return false; }
	//Read response
	resetStream(&streamFromServer);
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	unwrapPosition = 0;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!getEasyRPC_Return_Float(&packageFromServer, returnValue)) { easyRPC_Client_AfterSend(); return false; }
	easyRPC_ClientConnection_Disconnect();
	easyRPC_Client_AfterSend();
	return true;
}
/* ---------------------------------------------------------------------------*/
bool remote_print(char * message){
	easyRPC_Client_BeforeSend();
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { easyRPC_Client_AfterSend(); return false; }
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
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { easyRPC_Client_AfterSend(); return false; }
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	//Check receive ACK
	EasyRPCPackage packageFromServer;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!isACK_EasyRPC()){ easyRPC_Client_AfterSend(); return false; }
	//Read response
	resetStream(&streamFromServer);
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	easyRPC_ClientConnection_Disconnect();
	easyRPC_Client_AfterSend();
	return true;
}
/* ---------------------------------------------------------------------------*/
bool remote_binaryData(uint8_t * data, uint16_t data_len){
	easyRPC_Client_BeforeSend();
	if (!easyRPC_ClientConnection_IsConnected()) {
	  if (!easyRPC_ClientConnection_Connect()) { easyRPC_Client_AfterSend(); return false; }
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
	if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { easyRPC_Client_AfterSend(); return false; }
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	//Check receive ACK
	EasyRPCPackage packageFromServer;
	unwrapData(&streamFromServer, &packageFromServer);
	if(!isACK_EasyRPC()){ easyRPC_Client_AfterSend(); return false; }
	//Read response
	resetStream(&streamFromServer);
	if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }
	easyRPC_ClientConnection_Disconnect();
	easyRPC_Client_AfterSend();
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif