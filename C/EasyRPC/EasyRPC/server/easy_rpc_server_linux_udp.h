#ifndef _EASY_RPC_SERVER_LINUX_UDP_H
#define _EASY_RPC_SERVER_LINUX_UDP_H
/* ---------------------------------------------------------------------------*/
#include <thread>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <list>
/* ---------------------------------------------------------------------------*/
#include "easy_rpc_server.h"
#include "../package_builders/build_package_from_client_or_server.h"
#include "../package_builders/build_package_to_client.h"
/* ---------------------------------------------------------------------------*/
#define INVALID_SOCKET 0
struct sockaddr_in servServerAddr;
int linuxServerUDP_port = 2000;
int serverSocket = INVALID_SOCKET;
bool isServerConnected = false;

#define BUFFER_UDP_SIZE 1024    
typedef struct {
    pthread_t thread;
    sockaddr_in clientServerAddr;
    socklen_t client_addr_len;
    bool stop_thread;
    uint8_t buffer[BUFFER_UDP_SIZE];
    uint16_t len;
} THREAD_UDP;
THREAD_UDP thread_udp;

struct sockaddr_in clientServerAddr;
char ipClient[INET_ADDRSTRLEN];
unsigned short portClient = 0;
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxUDP_Listen(){
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    memset(&servServerAddr, 0, sizeof(servServerAddr));    
    servServerAddr.sin_family = AF_INET;
    servServerAddr.sin_addr.s_addr = INADDR_ANY;
    servServerAddr.sin_port = htons(linuxServerUDP_port);

    if (bind(serverSocket, (struct sockaddr*)&servServerAddr, sizeof(servServerAddr)) < 0) {
        return false;
    }
    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxUDP_Send(uint8_t *data, uint16_t dataLen){

    memset(&clientServerAddr, 0, sizeof(clientServerAddr));
    clientServerAddr.sin_family = AF_INET;
    clientServerAddr.sin_port = htons(portClient);
    clientServerAddr.sin_addr.s_addr = inet_addr(ipClient);
    
    if (sendto(serverSocket, data, dataLen, 0, (struct sockaddr *)&clientServerAddr, sizeof(clientServerAddr)) == -1) {
        #ifdef PRINT_DEBUG
        printf("Server->SendLinuxUDP (%s:%d): Fail!\n", ipClient , clientServerAddr.sin_port);
        #endif        
        return false;
    }
    #ifdef PRINT_DEBUG
    printf("Server->SendLinuxUDP (%s:%d): OK!\n", ipClient , clientServerAddr.sin_port);
    #endif        
    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxUDP_Receive(uint8_t* data, uint16_t* bytesRead, uint16_t timeout){
    if(thread_udp.len == 0){
        #ifdef PRINT_DEBUG
        printf("Server->ReceiveUDP->thread_udp.len == 0\n");
        #endif
        return false;
    }
    *bytesRead = thread_udp.len;
    memcpy(data,thread_udp.buffer,*bytesRead);
    #ifdef PRINT_DEBUG
        printf("easyRPC_ServerLinuxUDP_Receive = %d\n",*bytesRead);
    #endif
    if(thread_udp.len - *bytesRead == 0){
        thread_udp.len = 0;
    }else{
        int len = thread_udp.len - *bytesRead;
        memcpy(thread_udp.buffer,&thread_udp.buffer[*bytesRead],len);
        thread_udp.len -= *bytesRead;
    }
    return true;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ServerLinuxUDP_Close(){    
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxUDP_IsConnected(){
    if(isServerConnected){
        isServerConnected = false;
        return true;
    }
    return false;
}
/* ---------------------------------------------------------------------------*/
void *threadReadDataFromUDPServerLinux(void *arg){
    thread_udp.client_addr_len = sizeof(thread_udp.clientServerAddr);
    while(!thread_udp.stop_thread){
        int recv_len = recvfrom(serverSocket, &thread_udp.buffer[thread_udp.len], BUFFER_UDP_SIZE, 0,
                            (struct sockaddr*)&thread_udp.clientServerAddr, &thread_udp.client_addr_len);
        if (recv_len <= 0) {
            usleep(10000);
            continue;
        }        
        inet_ntop(AF_INET, &(thread_udp.clientServerAddr.sin_addr), ipClient, INET_ADDRSTRLEN);
        portClient = ntohs(thread_udp.clientServerAddr.sin_port);
        #ifdef PRINT_DEBUG
        printf("Server->Thread->IP: %s:%d\n", ipClient, portClient);
        #endif
        
        thread_udp.len += recv_len;
        isServerConnected = true;
        //#ifdef PRINT_DEBUG
        printf("Server->Thread->ReadUDP: %d / %d bytes\n",recv_len, thread_udp.len);
        //#endif
        processDataOnServer();
        
    }

    pthread_exit(NULL);
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxUDP_Setup(int port) {	
    if((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        return false;
    }

	easyRPC_ProcessData = easyRPC_ProcessDataFromClient;
	linuxServerUDP_port = port;

	easyRPC_Server_Listen = easyRPC_ServerLinuxUDP_Listen;
	easyRPC_Server_IsConnected = easyRPC_ServerLinuxUDP_IsConnected;
	easyRPC_Server_Close = easyRPC_ServerLinuxUDP_Close;
	easyRPC_Server_Receive = easyRPC_ServerLinuxUDP_Receive;
	easyRPC_Server_Send = easyRPC_ServerLinuxUDP_Send;

    memset(&thread_udp,0,sizeof(thread_udp));

    pthread_create(&thread_udp.thread, NULL, threadReadDataFromUDPServerLinux, NULL);

	return true;
}
/* ---------------------------------------------------------------------------*/
#endif