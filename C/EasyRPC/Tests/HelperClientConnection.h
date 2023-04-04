#ifndef _HELPER_CLIENT_CONNECTION_H
#define _HELPER_CLIENT_CONNECTION_H
/* ---------------------------------------------------------------------------*/
#include "client/easy_rpc_client_windows_tcp.h"
#include "client/easy_rpc_client.h"
#include "package_builders/build_package_to_client.h"
#include "package_builders/build_package_from_client_or_server.h"
#include "libdebug.h"
/* ---------------------------------------------------------------------------*/
void test_client_connection() {
    easyRPC_ProcessData = easyRPC_ProcessDataFromServer;
    easyRPC_ClientWindowsTCP_Setup((char *)"127.0.0.1", (char*)"2000");
    
    resetEasyRPC_Package(&easyRPC_clientPackage);
    setEasyRPC_NameFunction(&easyRPC_clientPackage, (char*)"sum");
    easyRPC_clientPackage.returnInfo.type = Integer;

    setEasyRPC_Param_Integer(&easyRPC_clientPackage, 2);
    setEasyRPC_Param_Integer(&easyRPC_clientPackage, 3);

    Stream streamData, streamToServer, streamFromServer;
    EasyRPCPackage packageFromServer;
    resetStream(&streamToServer);
    resetStream(&streamFromServer);

    easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);
    wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

    if (!easyRPC_ClientConnection_IsConnected()) {
        if (!easyRPC_ClientConnection_Connect()) {
            return;
        }
    }
    if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) {
        set_red("test_client_connection->easyRPC_ClientConnection_Send->Error");
    }

    if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) {
        set_red("test_client_connection->easyRPC_ClientConnection_Receive->Error");
    }

    unwrapData(&streamFromServer, &packageFromServer);
    
    int retValue = 0;    

    if (!getEasyRPC_Return_Integer(&packageFromServer, &retValue)) {
        set_red("test_client_connection->Error");
    } else if (retValue != 5) {
        set_red("test_client_connection->Value->Error");
    }

    set_green("test_client_connection->OK");
}
/* ---------------------------------------------------------------------------*/

#endif