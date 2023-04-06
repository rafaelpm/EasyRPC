#ifndef _EASY_RPC_SERVER_LINUX_TCP_H
#define _EASY_RPC_SERVER_LINUX_TCP_H
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


#include "easy_rpc_server.h"
#include "../package_builders/build_package_from_client_or_server.h"
#include "../package_builders/build_package_to_client.h"

using namespace std;

#define TIMEOUT_DISCONNECT_CLIENT 1000
bool startedTimeZeroByes = false;
//struct timeb startTimeZeroBytes, endTimeZeroBytes;
int diff;

struct sockaddr_in addressServerLinux;
int addrlen = 0;
int linuxServerTCP_ListenSocket = 0;
int linuxServerTCP_ClientSocket = 0;
int linuxServerTCP_port = 0;
int linuxServerTCP_res = 0;
bool isLinuxServerTCP_Connected = false, isLinuxServerTCP_ClientConnected = false;
pthread_t threadLinuxServer;

int countErrorsRecv = 0;

/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxTCP_WaitNewConnection() {
	if ((linuxServerTCP_ClientSocket
		= accept(linuxServerTCP_ListenSocket, (struct sockaddr*)&addressServerLinux,
				(socklen_t*)&addrlen)) < 0) {
		/*perror("accept");
		exit(EXIT_FAILURE);*/
		printf("New connection error, create new socket! ");
		return false;
	}
	isLinuxServerTCP_ClientConnected = true;
	countErrorsRecv = 0;
	return true;
}
/* ---------------------------------------------------------------------------*/
void *threadWaitSocketConnection(void *arg) {
	printf("Thread Wait Socket Connection\n");

	resetStream(&streamOnServer);
	unwrapPosition = 0;

	while (easyRPC_ServerLinuxTCP_WaitNewConnection()) {
		printf("New Connection\n");
		while (easyRPC_Server_IsConnected()) {
			processDataOnServer();
			sleep(0.1);
		}
		if(isLinuxServerTCP_ClientConnected){
			printf("Close Client Socket***********\n");
			isLinuxServerTCP_ClientConnected = false;
			close(linuxServerTCP_ClientSocket);
		}		
		printf("End Connection\n");
	}

	printf("Thread Exit\n");
	easyRPC_Server_Close();	

	pthread_exit(NULL);
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxTCP_Listen() {
	
    if( (linuxServerTCP_ListenSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0 ){
        printf("Fail start socket!\n");
        return false;
    }

	int opt = 0;

    if (setsockopt(linuxServerTCP_ListenSocket, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)) < 0) {
        printf("Fail set socket!\n");
        exit(EXIT_FAILURE);
        return false;
    }
	
    addressServerLinux.sin_family = AF_INET;
    addressServerLinux.sin_addr.s_addr = INADDR_ANY;
    addressServerLinux.sin_port = htons(linuxServerTCP_port);

	addrlen = sizeof(addressServerLinux);

    // Forcefully attaching socket to the port 8080
    if (bind(linuxServerTCP_ListenSocket, (struct sockaddr*)&addressServerLinux,
             addrlen)
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
        return false;
    }

    if (listen(linuxServerTCP_ListenSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
        return false;
    }

	// LINUX
	/*struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(linuxServerTCP_ListenSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));*/

	if(pthread_create(&threadLinuxServer,NULL,&threadWaitSocketConnection, NULL) != 0){
		perror("Fail when start thread for client connections!");
		return false;
	}

	isLinuxServerTCP_Connected = true;

	return isLinuxServerTCP_Connected;
}

/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxTCP_IsConnected() {	
	try{
		int error_code;
		socklen_t error_code_size = sizeof(socklen_t);
		getsockopt(linuxServerTCP_ClientSocket, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);		

		isLinuxServerTCP_Connected = (error_code == 0);		
	}catch(...){
		isLinuxServerTCP_Connected = false;	
		if(isLinuxServerTCP_ClientConnected){
			isLinuxServerTCP_ClientConnected = false;
			close(linuxServerTCP_ClientSocket);
		}
	}
	return isLinuxServerTCP_Connected;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ServerLinuxTCP_Close() {	
	if(isLinuxServerTCP_Connected){
		isLinuxServerTCP_Connected = false;
		shutdown(linuxServerTCP_ListenSocket, SHUT_RDWR);
	}
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxTCP_Send(uint8_t* data, uint16_t dataLen) {
	if(!easyRPC_ServerLinuxTCP_IsConnected()){
		return false;
	}

	if(send(linuxServerTCP_ClientSocket, data, dataLen, MSG_NOSIGNAL) < 0){
		printf("easyRPC_ServerLinuxTCP_Send = ERROR\n");
		isLinuxServerTCP_Connected = false;
		isLinuxServerTCP_ClientConnected = false;
		close(linuxServerTCP_ClientSocket);
		return false;
	}
	
	printf("easyRPC_ServerLinuxTCP_Send = OK\n");
	return true;
}
/* ---------------------------------------------------------------------------*/

bool easyRPC_ServerLinuxTCP_Receive(uint8_t* data, uint16_t* bytesRead, uint16_t timeout) {
	*bytesRead = 0;

	if(!easyRPC_ServerLinuxTCP_IsConnected()){
		return false;
	}

	struct timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = 0;
	setsockopt(linuxServerTCP_ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

	int count = 0;

	do {
		linuxServerTCP_res = recv(linuxServerTCP_ClientSocket, (char*)data, 512, 0 );
		if (linuxServerTCP_res < 0) {
			if (*bytesRead == 0) {	
				sleep(0.01);
				count++;
				if(count >= 2){
					countErrorsRecv++;
					if(countErrorsRecv >= 2){
						countErrorsRecv = 0;
						printf("Timeout recv");
						isLinuxServerTCP_Connected = false;
						isLinuxServerTCP_ClientConnected = false;
						close(linuxServerTCP_ClientSocket);
					}
					return false;
				}else{
					continue;
				}								
			}else{
				break;
			}
		} else {
			countErrorsRecv = 0;
		}
		*bytesRead += linuxServerTCP_res;
		
	} while (true);
	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerLinuxTCP_Setup(int port) {	
	easyRPC_ProcessData = easyRPC_ProcessDataFromClient;

	linuxServerTCP_port = port;

	easyRPC_Server_Listen = easyRPC_ServerLinuxTCP_Listen;
	easyRPC_Server_IsConnected = easyRPC_ServerLinuxTCP_IsConnected;
	easyRPC_Server_Close = easyRPC_ServerLinuxTCP_Close;
	easyRPC_Server_Receive = easyRPC_ServerLinuxTCP_Receive;
	easyRPC_Server_Send = easyRPC_ServerLinuxTCP_Send;
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif
