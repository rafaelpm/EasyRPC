#include <iostream>
#include "client/easy_rpc_client.h"
#include "client/easy_rpc_client_windows_tcp.h"
#include "package_builders/build_package_to_client.h"
#include "package_builders/build_package_from_client_or_server.h"
/* ---------------------------------------------------------------------------*/
#include "libdebug.h"
/* ---------------------------------------------------------------------------*/
int main()
{
    easyRPC_ProcessData = easyRPC_ProcessDataFromServer;
    easyRPC_ClientWindowsTCP_Setup((char*)"127.0.0.1", (char*)"2000");

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
            set_red("easyRPC_ClientConnection_Send->Fail connect");
            return 0;
        }
    }
    if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) {
        set_red("easyRPC_ClientConnection_Send->Error");
    }

    if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 5000)) {
        set_red("easyRPC_ClientConnection_Receive->Error");
    }

    unwrapData(&streamFromServer, &packageFromServer);
    

    if (!isACK_EasyRPC()) {
        set_red("isACK_EasyRPC->Error");
    }

    resetStream(&streamToServer);
    resetStream(&streamFromServer);

    if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 10000)) {
        set_red("easyRPC_ClientConnection_Receive->Error");
    }

    unwrapData(&streamFromServer, &packageFromServer);

    int retValue = 0;

    if (!getEasyRPC_Return_Integer(&packageFromServer, &retValue)) {
        set_red("getEasyRPC_Return_Integer->Error");
    } else if (retValue != 5) {
        set_red("getEasyRPC_Return_Integer->Value->Error");
    }

    if (retValue != 5) {
        set_red("getEasyRPC_Return_Integer->Value->Content Error");
    }

    set_green("Windows Client = OK");
    return 0;
}
/* ---------------------------------------------------------------------------*/
