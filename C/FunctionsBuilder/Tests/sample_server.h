#ifndef _SAMPLE_SERVER_H
#define _SAMPLE_SERVER_H
/* ---------------------------------------------------------------------------*/
//#include <server/easy_rpc_server_linux_tcp.h>
#include <server/easy_rpc_server_linux_udp.h>
#include "easy_rpc/easy_rpc_server_bind.h"
/* ---------------------------------------------------------------------------*/
//PUT YOUR CODE HERE - BEGIN
//PUT YOUR CODE HERE - END
/* ---------------------------------------------------------------------------*/
void start_easyrpc_server(){
	easyRPC_Server_Bind_Functions();
	//easyRPC_ServerLinuxTCP_Setup(2000);
	easyRPC_ServerLinuxUDP_Setup(2000);
	if (!easyRPC_Server_Listen()) { printf("Fail when start listen TCP port 2000!\n"); return; }
	while (true) {
		//printf("Wait connection\n");
		while (!easyRPC_Server_IsConnected()) { sleep(0.1); }
		//printf("Wait disconnection\n");
		while (easyRPC_Server_IsConnected()) { sleep(0.1); }
	}
}
#endif