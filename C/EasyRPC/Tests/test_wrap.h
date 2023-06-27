#ifndef _TEST_WRAP_H
#define _TEST_WRAP_H
/* ---------------------------------------------------------------------------*/
#include "client/easy_rpc_client_windows_tcp.h"
#include "client/easy_rpc_client.h"
#include "package_builders/build_package_to_client.h"
#include "package_builders/build_package_from_client_or_server.h"
#include "libdebug.h"

bool isConnectedMock = false, isSendedMock = false, isReceivedMock = false;
uint8_t bufferReceiveMock[1024];
int totalReceiveMock = 0;

void easyRPC_ClientConnection_Mock_Setup();
void test_wrap_unwrap_sum();
void test_wrap_unwrap_text();
/* ---------------------------------------------------------------------------*/
void test_wrap_unwrap() {
    //test_wrap_unwrap_sum();
    test_wrap_unwrap_text();
}
/* ---------------------------------------------------------------------------*/
void test_wrap_unwrap_text() {
    char* url = (char *)"http://www.google.com.br";
    easyRPC_ClientConnection_Mock_Setup();
    easyRPC_ProcessData = easyRPC_ProcessDataFromClient;

    //easyRPC_ClientWindowsTCP_Setup((char*)"127.0.0.1", (char*)"2000");

    resetEasyRPC_Package(&easyRPC_clientPackage);
    setEasyRPC_NameFunction(&easyRPC_clientPackage, (char*)"connect");
    easyRPC_clientPackage.returnInfo.type = Boolean;

    setEasyRPC_Param_String(&easyRPC_clientPackage, url);
    
    Stream streamData, streamToServer;
    easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);    
    wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

    if (!easyRPC_ClientConnection_IsConnected()) {
        if (!easyRPC_ClientConnection_Connect()) {
            return;
        }
    }
    easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size);

    streamToServer.index = 0;

    EasyRPCPackage packageUnwrap;

    UnwrapStates res = unwrapData(&streamToServer, &packageUnwrap);
    if (res != Unwrap_Complete) {
        set_red("unwrapDataText->Error");
    }
    if (strcmp((const char *)&packageUnwrap.functionName, (char *)"connect") != 0) {
        set_red("unwrapDataText->FunctionName->Error");
    }
    if (packageUnwrap.totalParams < 1) {
        set_red("unwrapDataText->TotalParams->Error");
    }
    char urlTemp[50];
    memset(urlTemp, 0, 50);
    if (!getEasyRPC_Param_String(&packageUnwrap, (char *) & urlTemp, 0)) {
        set_red("unwrapDataText->getEasyRPC_Param_String->Error");
    }
    if (strcmp((const char*)&urlTemp, (char*)url) != 0) {
        set_red("unwrapDataText->Param->Error");
    }

    set_green("unwrapDataText->OK");
}
/* ---------------------------------------------------------------------------*/
void test_wrap_unwrap_sum() {
    easyRPC_ClientConnection_Mock_Setup();
    easyRPC_ProcessData = easyRPC_ProcessDataFromClient;

    //easyRPC_ClientWindowsTCP_Setup((char*)"127.0.0.1", (char*)"2000");

    resetEasyRPC_Package(&easyRPC_clientPackage);
    setEasyRPC_NameFunction(&easyRPC_clientPackage, (char*)"sum");
    easyRPC_clientPackage.returnInfo.type = Integer;

    setEasyRPC_Param_Integer(&easyRPC_clientPackage, 2);
    setEasyRPC_Param_Integer(&easyRPC_clientPackage, 3);

    Stream streamData, streamToServer;
    easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);
    wrapData(&streamToServer, &streamData.buffer[0], streamData.size);

    if (!easyRPC_ClientConnection_IsConnected()) {
        if (!easyRPC_ClientConnection_Connect()) {
            return;
        }
    }
    easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size);

    streamToServer.index = 0;

    EasyRPCPackage packageUnwrap;

    UnwrapStates res = unwrapData(&streamToServer, &packageUnwrap);
    if (res != Unwrap_Complete) {
        set_red("unwrapData->Error");
    }
    set_green("unwrapData->OK");
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_IsConnected_Mock() {
    if (!isConnectedMock) {
        isConnectedMock = true;
    }
    return isConnectedMock;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_Connect_Mock() {
    isConnectedMock = true;
    return isConnectedMock;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ClientConnection_Disconnect_Mock() {
    isConnectedMock = false;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_Send_Mock(uint8_t* data, uint16_t dataLen) {
    printf("Send: ");
    for (int i = 0; i < dataLen; i++) {
        printf("0x%02x ", data[i]);
    }
    printf("\n");
    return isSendedMock;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_Receive_Mock(uint8_t* data, uint16_t*bytesRead, uint16_t timeout) {
    if (totalReceiveMock <= 0) {
        return false;
    }
    memcpy(data, &bufferReceiveMock[0], totalReceiveMock);
    return isReceivedMock;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ClientConnection_Mock_Setup() {
    easyRPC_ClientConnection_IsConnected = easyRPC_ClientConnection_IsConnected_Mock;
    easyRPC_ClientConnection_Connect = easyRPC_ClientConnection_Connect_Mock;
    easyRPC_ClientConnection_Disconnect = easyRPC_ClientConnection_Disconnect_Mock;
    easyRPC_ClientConnection_Send = easyRPC_ClientConnection_Send_Mock;
    easyRPC_ClientConnection_Receive = easyRPC_ClientConnection_Receive_Mock;
}
/* ---------------------------------------------------------------------------*/
#endif
