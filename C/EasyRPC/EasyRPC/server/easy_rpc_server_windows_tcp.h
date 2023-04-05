#ifndef _EASY_RPC_SERVER_WINDOWS_TCP_H
#define _EASY_RPC_SERVER_WINDOWS_TCP_H
/* ---------------------------------------------------------------------------*/
#define WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>
#include <string.h>
#include "easy_rpc_server.h"
#include "../package_builders/build_package_from_client_or_server.h"
#include "../package_builders/build_package_to_client.h"
#include <sys\timeb.h> 

// Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define TIMEOUT_DISCONNECT_CLIENT 1000
bool startedTimeZeroByes = false;
struct timeb startTimeZeroBytes, endTimeZeroBytes;
int diff;

char* windowsServerTCP_port;
WSADATA windowsServerTCP_wsaData;
SOCKET windowsServerTCP_ListenSocket = INVALID_SOCKET;
SOCKET windowsServerTCP_ClientSocket = INVALID_SOCKET;

DWORD threadWaitConnectionId;
HANDLE threadWaitConnection;
char error_code;
socklen_t error_code_size = sizeof(socklen_t);

/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerWindowsTCP_WaitNewConnection() {
	windowsServerTCP_ClientSocket = accept(windowsServerTCP_ListenSocket, NULL, NULL);
	if (windowsServerTCP_ClientSocket == INVALID_SOCKET) {
		printf("AcceptSocket error: %ld\n", WSAGetLastError());
		closesocket(windowsServerTCP_ClientSocket);
		WSACleanup();
		return false;
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
DWORD WINAPI threadWaitSocketConnection(LPVOID lpParameter) {
	printf("Thread Wait Socket Connection\n");

	resetStream(&streamOnServer);
	unwrapPosition = 0;

	while (easyRPC_ServerWindowsTCP_WaitNewConnection()) {
		printf("New Connection\n");
		while (easyRPC_Server_IsConnected()) {
			processDataOnServer();
			Sleep(100);
		}
		printf("Close Client Socket***********\n");
		closesocket(windowsServerTCP_ClientSocket);
		windowsServerTCP_ClientSocket = INVALID_SOCKET;
		printf("End Connection\n");
	}

	printf("Thread Exit\n");
	easyRPC_Server_Close();
	return 0;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerWindowsTCP_Listen() {
	struct addrinfo hints;
	struct addrinfo* result = NULL;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, windowsServerTCP_port, &hints, &result) != 0) {
		WSACleanup();
		return false;
	}
	windowsServerTCP_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (windowsServerTCP_ListenSocket == INVALID_SOCKET) {
		printf("Socket error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	if (bind(windowsServerTCP_ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		printf("BindSocket error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(windowsServerTCP_ListenSocket);
		WSACleanup();
		return false;
	}

	if (listen(windowsServerTCP_ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("ListenSocket error: %ld\n", WSAGetLastError());
		closesocket(windowsServerTCP_ListenSocket);
		WSACleanup();
		return false;
	}

	threadWaitConnection = CreateThread(NULL, 0, threadWaitSocketConnection, NULL, 0, &threadWaitConnectionId);
	if (threadWaitConnection == NULL) {
		freeaddrinfo(result);
		closesocket(windowsServerTCP_ListenSocket);
		WSACleanup();
		return false;
	}

	return true;
}

/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerWindowsTCP_IsConnected() {	
	if (windowsServerTCP_ClientSocket == INVALID_SOCKET) {
		return false;
	}
	
	return true;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ServerWindowsTCP_Close() {	
	if (windowsServerTCP_ListenSocket != INVALID_SOCKET){
		closesocket(windowsServerTCP_ListenSocket);
		windowsServerTCP_ListenSocket = INVALID_SOCKET;
	}
	if (windowsServerTCP_ClientSocket != INVALID_SOCKET) {
		closesocket(windowsServerTCP_ClientSocket);
		windowsServerTCP_ClientSocket = INVALID_SOCKET;
	}
	WSACleanup();

	WSAStartup(MAKEWORD(2, 2), &windowsServerTCP_wsaData);
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerWindowsTCP_Send(uint8_t* data, uint16_t dataLen) {	
	if (send(windowsServerTCP_ClientSocket, (const char*)data, dataLen, 0) == SOCKET_ERROR) {
		printf("easyRPC_ServerWindowsTCP_Send: %ld\n\n", WSAGetLastError());		
		return false;
	}
	return true;
}
/* ---------------------------------------------------------------------------*/

bool easyRPC_ServerWindowsTCP_Receive(uint8_t* data, uint16_t* bytesRead, uint16_t timeout) {
	struct timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = 0;
	setsockopt(windowsServerTCP_ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

	Sleep(100);

	int windowsTCP_res;
	
	*bytesRead = 0;
	do {
		windowsTCP_res = recv(windowsServerTCP_ClientSocket, (char*)data, 512, 0);
		if (windowsTCP_res < 0) {
			if (*bytesRead == 0) {
				//printf("easyRPC_ServerWindowsTCP_Receive: %ld\n\n", WSAGetLastError());		
				if (!startedTimeZeroByes) {
					startedTimeZeroByes = true;
					ftime(&startTimeZeroBytes);
				} else {
					ftime(&endTimeZeroBytes);
					diff = (int)(1000.0 * (endTimeZeroBytes.time - startTimeZeroBytes.time)
						+ (endTimeZeroBytes.millitm - startTimeZeroBytes.millitm));
					if (diff >= TIMEOUT_DISCONNECT_CLIENT) {
						printf("Disconnect timeout\n");
						startedTimeZeroByes = false;
						if (windowsServerTCP_ClientSocket != INVALID_SOCKET) {
							closesocket(windowsServerTCP_ClientSocket);
							windowsServerTCP_ClientSocket = INVALID_SOCKET;
						}
					}
				}
				return false;
			}
			break;
		} else {
			startedTimeZeroByes = false;
		}
		*bytesRead += windowsTCP_res;
		Sleep(10);
	} while (windowsTCP_res > 0);
	return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ServerWindowsTCP_Setup(char* port) {
	if (WSAStartup(MAKEWORD(2, 2), &windowsServerTCP_wsaData) != 0) {
		return false;
	}

	easyRPC_ProcessData = easyRPC_ProcessDataFromClient;

	windowsServerTCP_port = port;

	easyRPC_Server_Listen = easyRPC_ServerWindowsTCP_Listen;
	easyRPC_Server_IsConnected = easyRPC_ServerWindowsTCP_IsConnected;
	easyRPC_Server_Close = easyRPC_ServerWindowsTCP_Close;
	easyRPC_Server_Receive = easyRPC_ServerWindowsTCP_Receive;
	easyRPC_Server_Send = easyRPC_ServerWindowsTCP_Send;
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif
