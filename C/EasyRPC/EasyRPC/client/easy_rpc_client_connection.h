#ifndef _EASY_RPC_CLIENT_CONNECTION_H
#define _EASY_RPC_CLIENT_CONNECTION_H
/* ---------------------------------------------------------------------------*/
#include <string>

typedef bool (*EasyRPC_ClientConnection_IsConnected)();
typedef bool (*EasyRPC_ClientConnection_Connect)();
typedef void (*EasyRPC_ClientConnection_Disconnect)();
typedef bool (*EasyRPC_ClientConnection_Send)(uint8_t *data, uint16_t dataLen);
typedef bool (*EasyRPC_ClientConnection_Receive)(uint8_t* data, uint16_t *bytesRead, uint16_t timeout);

EasyRPC_ClientConnection_IsConnected easyRPC_ClientConnection_IsConnected;
EasyRPC_ClientConnection_Connect easyRPC_ClientConnection_Connect;
EasyRPC_ClientConnection_Disconnect easyRPC_ClientConnection_Disconnect;
EasyRPC_ClientConnection_Send easyRPC_ClientConnection_Send;
EasyRPC_ClientConnection_Receive easyRPC_ClientConnection_Receive;

/* ---------------------------------------------------------------------------*/
#endif
