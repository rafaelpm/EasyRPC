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
char hostClient[INET_ADDRSTRLEN];
struct sockaddr_in servAddr;
struct sockaddr_in clientAddr;
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_IsConnectedLinuxUDP(){
    return isConnected;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ConnectLinuxUDP(){
    isConnected = true;
    return isConnected;
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
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    if (sendto(clientSocket, data, dataLen, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
        #ifdef PRINT_DEBUG
        printf("Client->SendLinuxUDP (%s:%d): Fail!\n",hostClient , clientAddr.sin_port);
        #endif
        return false;
    }
    #ifdef PRINT_DEBUG
    printf("Client->SendLinuxUDP (%s:%d): OK\n",hostClient , clientAddr.sin_port);    
    #endif    
    isConnected = true;

    return isConnected;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_SetTimeoutUDP(int seconds, int ms){
    
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = ms;

    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ReceiveLinuxUDP(uint8_t* data, uint16_t *bytesRead, uint16_t timeout){
    int unitRead = 0;
    int maxTime = timeout / 10;
    int countTime = 0;

    //easyRPC_ClientConnection_SetTimeoutUDP(10,0);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
#ifdef PRINT_DEBUG
    //printf("Client->ReceiveLinuxUDP\n");
#endif

    *bytesRead = 0;
    do{
        unitRead = recvfrom(clientSocket, data, SIZE_BUFFER_STREAM, 0,
                            (struct sockaddr*)&client_addr, &client_addr_len);
        
        if(unitRead <= 0){                        
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
        if(*bytesRead >= 8){
            break;
        }
        usleep(100000);
    }while(unitRead > 0);

    #ifdef PRINT_DEBUG
    printf("Client->ReceiveLinuxUDP->unitRead = %d / %d\n",unitRead, *bytesRead);
    #endif

    return (*bytesRead >= 8);
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientLinuxUDP_Setup(char *host, int portClient) {
    if((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        return false;
    }
    memset(&clientAddr, 0, sizeof(clientAddr));
    memset(&hostClient[0], 0, INET_ADDRSTRLEN);
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(portClient);
    memcpy(&hostClient[0], host, strlen(host));
    //clientAddr.sin_addr.s_addr = inet_addr(host);

    if(inet_pton(AF_INET,host,&clientAddr.sin_addr) <= 0){
        printf("Fail to convert IP address!\n");
        return false;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(portClient)+1;

    if (bind(clientSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        return false;
    }

    #ifdef PRINT_DEBUG
    printf("Client->Setup->Client: %s:%d\n", host, clientAddr.sin_port);
    printf("Client->Setup->Server->Port: %d\n", servAddr.sin_addr , servAddr.sin_port);
    #endif    

    easyRPC_ClientConnection_IsConnected = easyRPC_ClientConnection_IsConnectedLinuxUDP;
	easyRPC_ClientConnection_Connect = easyRPC_ClientConnection_ConnectLinuxUDP;
    easyRPC_ClientConnection_Disconnect = easyRPC_ClientConnection_DisconnectLinuxUDP;

    easyRPC_ClientConnection_Send = easyRPC_ClientConnection_SendLinuxUDP;
    easyRPC_ClientConnection_Receive = easyRPC_ClientConnection_ReceiveLinuxUDP;

    return true;
}
/* ---------------------------------------------------------------------------*/
#endif