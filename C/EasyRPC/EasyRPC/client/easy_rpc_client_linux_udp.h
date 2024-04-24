#ifndef _EASY_RPC_CLIENT_LINUX_UDP_H
#define _EASY_RPC_CLIENT_LINUX_UDP_H
/* ---------------------------------------------------------------------------*/
#include "client/easy_rpc_client_connection.h"
#include "libstream.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h> 
/* ---------------------------------------------------------------------------*/
#define INVALID_SOCKET 0
/* ---------------------------------------------------------------------------*/
int clientSocket = INVALID_SOCKET;
bool isConnected = false;
struct sockaddr_in servAddr;
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_IsConnectedLinuxUDP(){
    return isConnected;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ConnectLinuxUDP(){
    return true;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ClientConnection_DisconnectLinuxUDP(){
    /*if(clientSocket != INVALID_SOCKET){
        close(clientSocket);
        clientSocket = INVALID_SOCKET;
        isConnected = false;
    }*/
    isConnected = false;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_SendLinuxUDP(uint8_t *data, uint16_t dataLen){
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    if (sendto(clientSocket, data, dataLen, 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
        return false;
    }
    isConnected = true;

    return isConnected;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ReceiveLinuxUDP(uint8_t* data, uint16_t *bytesRead, uint16_t timeout){
    int unitRead = 0;
    int maxTime = timeout / 10;
    int countTime = 0;

    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    *bytesRead = 0;
    do{
        unitRead = recvfrom(clientSocket, data, SIZE_BUFFER_STREAM, 0,
                            (struct sockaddr*)&client_addr, &client_addr_len);
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
            countTime = maxTime-1;
        }
        usleep(10000);
    }while(unitRead > 0);
    return false;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientLinuxUDP_Setup(char *host, int port) {
    if((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        return false;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    if(inet_pton(AF_INET,host,&servAddr.sin_addr) <= 0){
        printf("Fail to convert IP address!\n");
        return false;
    }

    easyRPC_ClientConnection_IsConnected = easyRPC_ClientConnection_IsConnectedLinuxUDP;
	easyRPC_ClientConnection_Connect = easyRPC_ClientConnection_ConnectLinuxUDP;
    easyRPC_ClientConnection_Disconnect = easyRPC_ClientConnection_DisconnectLinuxUDP;

    easyRPC_ClientConnection_Send = easyRPC_ClientConnection_SendLinuxUDP;
    easyRPC_ClientConnection_Receive = easyRPC_ClientConnection_ReceiveLinuxUDP;

    return true;
}
/* ---------------------------------------------------------------------------*/
#endif