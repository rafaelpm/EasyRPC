#ifndef _EASY_RPC_SERVER_H
#define _EASY_RPC_SERVER_H
/* ---------------------------------------------------------------------------*/
#include <string>
#include "entities/easy_rpc_package.h"
#include "entities/easy_rpc_package_base.h"
#include "easy_rpc_bind.h"
#include "../package_builders/build_package_to_client.h"
#include "libstream.h"

#ifdef _CALCULE_TIME
#include "time_to_run.h"
#endif

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

	#ifdef _CALCULE_TIME
	start_time();
	#endif
	
	if (!easyRPC_Server_Receive(&streamOnServer.buffer[0], &streamOnServer.size, 1000)) {
		return false;
	}

	if (streamOnServer.size == 0) {
		return false;
	}
	
	#ifdef _CALCULE_TIME
	end_time((char *)"Receive data");
	start_time();
	#endif

	if (unwrapData(&streamOnServer, &packageServerUnwrap) != Unwrap_Complete) {
		return false;
	}

	#ifdef _CALCULE_TIME
	end_time((char *)"Unwrap data");
	start_time();
	#endif

	resetStream(&streamOnServer);

	if (!easyRPC_ProcessPackageBind(&packageServerUnwrap)) {
		return false;
	}

	#ifdef _CALCULE_TIME
	end_time((char *)"ProcessPackage");
	start_time();
	#endif

	if (!easyRPC_toBytesToClient(&packageServerUnwrap, &streamOnServer)) {
		return false;
	}

	#ifdef _CALCULE_TIME
	end_time((char *)"Prepare answer");
	start_time();
	#endif

	resetStream(&streamOnServerToClient);
	wrapData(&streamOnServerToClient, &streamOnServer.buffer[0], streamOnServer.size);

	#ifdef _CALCULE_TIME
	end_time((char *)"Wrap answer");
	start_time();
	#endif

	easyRPC_Server_Send(&streamOnServerToClient.buffer[0], streamOnServerToClient.size);

	#ifdef _CALCULE_TIME
	end_time((char *)"Send answer");
	start_time();
	#endif

	resetStream(&streamOnServer);

	return true;
}
/* ---------------------------------------------------------------------------*/
#endif