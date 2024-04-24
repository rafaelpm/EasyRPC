#ifndef _SAMPLE_CLIENT_H
#define _SAMPLE_CLIENT_H
/* ---------------------------------------------------------------------------*/
//#include <client/easy_rpc_client_linux_tcp.h>
#include <client/easy_rpc_client_linux_udp.h>
#include "easy_rpc/easy_rpc_remote_client.h"
/* ---------------------------------------------------------------------------*/
void easyrpc_client_setup(){
	easyRPC_ProcessData = easyRPC_ProcessDataFromServer;
	easyRPC_ClientLinuxUDP_Setup((char*)"127.0.0.1", 2000);
	//Sample call sum remote function
	//int retIntValue = 0;
	//if (!remote_sum(&retIntValue, 1, 4)) {
	//printf("remote_sum = Error\n");
	//}else{
	//printf("remote_sum = Success\n");
	//}
}
#endif