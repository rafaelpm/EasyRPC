#ifndef _EASY_RPC_CLIENT_LINUX_H
#define _EASY_RPC_CLIENT_LINUX_H
/* ---------------------------------------------------------------------------*/
#include "easy_rpc_client_connection.h"
#include "libstream.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define INVALID_SOCKET 0
/* ---------------------------------------------------------------------------*/
int clientSocket = INVALID_SOCKET;
bool isConnected = false;
struct sockaddr_in servAddr;
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_IsConnectedLinuxTCP() {
    return isConnected;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ClientConnection_DisconnectLinuxTCP() {

    if (easyRPC_ClientConnection_IsConnectedLinuxTCP()) {        
        close(clientSocket);        
        isConnected = false;        
    }
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ConnectLinuxTCP(){
    if(connect(clientSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        if(clientSocket != INVALID_SOCKET){
            close(clientSocket);
        }

        if(connect(clientSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
            isConnected = false;
            printf("Fail when try connect to a server!\n");
            return false;
        }
    }
    isConnected = true;    
    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_SendLinuxTCP(uint8_t* data, uint16_t dataLen) {
    //printf("easyRPC_ClientConnection_SendLinuxTCP: %d\n", dataLen);
    if(send(clientSocket,data, dataLen, 0) < 0){
        return false;
    }
    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ReceiveLinuxTCP(uint8_t* data, uint16_t *bytesRead, uint16_t timeout) {
    int unitRead = 0;
    int maxTime = timeout / 10;
    int countTime = 0;

    *bytesRead = 0;
    do{
        unitRead = read(clientSocket, data, SIZE_BUFFER_STREAM);
        if(unitRead < 0){                        
            if(*bytesRead > 0){
                break;
            }
            countTime++;
            if(countTime >= maxTime){
                break;
            }
        }else{
            *bytesRead += unitRead;
        }
        usleep(10000);
    }while(unitRead > 0);

    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientLinuxTCP_Setup(char *host, int port) {
    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return false;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    if(inet_pton(AF_INET,host,&servAddr.sin_addr) <= 0){
        printf("Fail to convert IP address!\n");
        return false;
    }

    easyRPC_ClientConnection_IsConnected = easyRPC_ClientConnection_IsConnectedLinuxTCP;
	easyRPC_ClientConnection_Connect = easyRPC_ClientConnection_ConnectLinuxTCP;
    easyRPC_ClientConnection_Disconnect = easyRPC_ClientConnection_DisconnectLinuxTCP;

    easyRPC_ClientConnection_Send = easyRPC_ClientConnection_SendLinuxTCP;
    easyRPC_ClientConnection_Receive = easyRPC_ClientConnection_ReceiveLinuxTCP;

    return true;
}
/* ---------------------------------------------------------------------------*/
#endif