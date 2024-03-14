#include <stdio.h>
#include "client/easy_rpc_client_linux_tcp.h"
#include "easy_rpc_remote_client.h"
using namespace std;

void set_red(string msg){
    printf("%s\n",msg.c_str());
}

int main(int argc, char **argv){
    printf("TestLinuxClient\n");

    easyRPC_ProcessData = easyRPC_ProcessDataFromServer;
	//easyRPC_ClientLinuxTCP_Setup((char*)"127.0.0.1", 2000);
    easyRPC_ClientLinuxTCP_Setup((char*)"192.168.15.50", 2000);

    int retIntValue = 0;
	if (!remote_sum(&retIntValue, 1, 4)) {
		set_red("remote_sum = ERROR");
	} else if (retIntValue != 5) {
		set_red("remote_sum->value = ERROR");
	}
    printf("Res = %d\n",retIntValue);

    printf("Finish\n");
    return 0;
}