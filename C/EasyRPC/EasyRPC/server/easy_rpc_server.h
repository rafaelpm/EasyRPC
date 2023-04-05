#ifndef _EASY_RPC_SERVER_H
#define _EASY_RPC_SERVER_H
/* ---------------------------------------------------------------------------*/
#include <string>
#include "entities/easy_rpc_package.h"
#include "entities/easy_rpc_package_base.h"
#include "easy_rpc_bind.h"
#include "../package_builders/build_package_to_client.h"
#include "libstream.h"

typedef bool (*EasyRPC_Server_Listen)();
typedef bool (*EasyRPC_Server_Send)(uint8_t *data, uint16_t dataLen);
//typedef bool (*EasyRPC_Server_HasData)();
typedef bool (*EasyRPC_Server_Receive)(uint8_t* data, uint16_t* bytesRead, uint16_t timeout);
typedef void (*EasyRPC_Server_Close)();
typedef bool (*EasyRPC_Server_IsConnected)();
/* ---------------------------------------------------------------------------*/

EasyRPC_Server_Listen easyRPC_Server_Listen;
EasyRPC_Server_Send easyRPC_Server_Send;
EasyRPC_Server_Receive easyRPC_Server_Receive;
EasyRPC_Server_Close easyRPC_Server_Close;
EasyRPC_Server_IsConnected easyRPC_Server_IsConnected;

Stream streamOnServer, streamOnServerToClient;
EasyRPCPackage packageServerUnwrap;
/* ---------------------------------------------------------------------------*/
bool processDataOnServer() {
	
	if (!easyRPC_Server_Receive(&streamOnServer.buffer[0], &streamOnServer.index, 1000)) {
		return false;
	}
	
	if (!unwrapData(&streamOnServer, &packageServerUnwrap) != Unwrap_Complete) {
		return false;
	}
	resetStream(&streamOnServer);

	if (!easyRPC_ProcessPackageBind(&packageServerUnwrap)) {
		return false;
	}

	if (!easyRPC_toBytesToServer(&packageServerUnwrap, &streamOnServer)) {
		return false;
	}

	resetStream(&streamOnServerToClient);
	wrapData(&streamOnServer, &streamOnServerToClient.buffer[0], streamOnServerToClient.size);

	if (!easyRPC_Server_Send(&streamOnServerToClient.buffer[0], streamOnServerToClient.size)) {
		return false;
	}

	return true;
}
/* ---------------------------------------------------------------------------*/
#endif