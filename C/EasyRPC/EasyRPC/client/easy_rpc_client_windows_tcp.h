#ifndef _EASY_RPC_CLIENT_WINDOWS_TCP_H
#define _EASY_RPC_CLIENT_WINDOWS_TCP_H
/* ---------------------------------------------------------------------------*/
#include "easy_rpc_client_connection.h"

#define WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>
/*#include <stdlib.h>
#include <stdio.h>*/

// Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
/* ---------------------------------------------------------------------------*/
WSADATA windowsTCP_wsaData;
struct addrinfo windowsTCP_hints;
struct addrinfo* windowsTCP_result;
SOCKET windowsTCP_socket = INVALID_SOCKET;
char* windowsTCP_host, *windowsTCP_port;
int windowsTCP_res;

/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_IsConnectedWindowsTCP() {
    return windowsTCP_socket != INVALID_SOCKET;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ConnectWindowsTCP() {
    struct addrinfo* ptr;

    ZeroMemory(&windowsTCP_hints, sizeof(windowsTCP_hints));
    windowsTCP_hints.ai_family = AF_UNSPEC;
    windowsTCP_hints.ai_socktype = SOCK_STREAM;
    windowsTCP_hints.ai_protocol = IPPROTO_TCP;

    windowsTCP_socket = INVALID_SOCKET;

    if (getaddrinfo(windowsTCP_host, windowsTCP_port, &windowsTCP_hints, &windowsTCP_result) != 0) {
        printf("GetAddrInfo error: %ld\n\n", WSAGetLastError());
        WSACleanup();
        return false;
    }

    for (ptr = windowsTCP_result; ptr != NULL; ptr = ptr->ai_next) {
        windowsTCP_socket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (windowsTCP_socket == INVALID_SOCKET) {
            printf("Socket error: %ld\n\n", WSAGetLastError());
            WSACleanup();
            return false;
        }        
        
        if (connect(windowsTCP_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
            closesocket(windowsTCP_socket);
            windowsTCP_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(windowsTCP_result);

    if (!easyRPC_ClientConnection_IsConnectedWindowsTCP()) {
        WSACleanup();
        return false;
    }
    

	return true;
}
/* ---------------------------------------------------------------------------*/
void easyRPC_ClientConnection_DisconnectWindowsTCP() {
    if (easyRPC_ClientConnection_IsConnectedWindowsTCP()) {
        closesocket(windowsTCP_socket);
        WSACleanup();
        windowsTCP_socket = INVALID_SOCKET;
    }
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_SendWindowsTCP(uint8_t* data, uint16_t dataLen) {
    if (!easyRPC_ClientConnection_IsConnectedWindowsTCP()) {
        return false;
    }

    if (send(windowsTCP_socket, (const char*)data, dataLen, 0) == SOCKET_ERROR) {
        printf("Socket send: %ld\n\n", WSAGetLastError());
        easyRPC_ClientConnection_DisconnectWindowsTCP();
        return false;
    }

    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientConnection_ReceiveWindowsTCP(uint8_t* data, uint16_t *bytesRead, uint16_t timeout) {
    struct timeval tv;
    tv.tv_sec = timeout/1000;
    tv.tv_usec = 0;
    setsockopt(windowsTCP_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    Sleep(100);

    *bytesRead = 0;
    do {
        windowsTCP_res = recv(windowsTCP_socket, (char*)data, 512, 0);
        if (windowsTCP_res < 0) {
            if (*bytesRead == 0) {
                printf("Socket recv: %ld\n\n", WSAGetLastError());
                easyRPC_ClientConnection_DisconnectWindowsTCP();
                return false;
            }
            break;
        }
        *bytesRead += windowsTCP_res;
        Sleep(10);
    } while (windowsTCP_res > 0);
    return true;
}
/* ---------------------------------------------------------------------------*/
bool easyRPC_ClientWindowsTCP_Setup(char *host, char *port) {
	if (WSAStartup(MAKEWORD(2, 2), &windowsTCP_wsaData) != 0) {
		return false;
	}
    windowsTCP_host = host;
    windowsTCP_port = port;
	
    easyRPC_ClientConnection_IsConnected = easyRPC_ClientConnection_IsConnectedWindowsTCP;
	easyRPC_ClientConnection_Connect = easyRPC_ClientConnection_ConnectWindowsTCP;
    easyRPC_ClientConnection_Disconnect = easyRPC_ClientConnection_DisconnectWindowsTCP;
	
	easyRPC_ClientConnection_Send = easyRPC_ClientConnection_SendWindowsTCP;
    easyRPC_ClientConnection_Receive = easyRPC_ClientConnection_ReceiveWindowsTCP;

	return true;
}
/* ---------------------------------------------------------------------------*/
#endif
