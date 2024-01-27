#include <iostream>
#include "server/easy_rpc_server.h"
#include "server/easy_rpc_server_windows_tcp.h"
#include "easy_rpc_server_bind.h"
#include "libdebug.h"

int sum(int a, int b) {
    printf("RUN -> SUM(%d, %d)\n",a,b);
    return a + b;
}

float subtract(float a, float b) {
    printf("RUN -> subtract(%0.2f, %0.2f)\n", a, b);
    return a - b;
}

void print(char* message) {
    printf("Print: %s\n", message);
}

void binaryData(uint8_t* data, uint16_t data_len) {
    printf("BinaryData: %d\n", data_len);
    for (int i = 0; i < data_len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main()
{
    printf("TestServer EasyRPC\n");
    easyRPC_Server_Bind_Functions();
    easyRPC_ServerWindowsTCP_Setup((char *)"2000");

    if (!easyRPC_Server_Listen()) {
        set_red("easyRPC_Server_Listen = ERROR");        
    }

    for (int i = 0; i < 4; i++) {
        printf("Wait connection: %d\n", i);
        while (!easyRPC_Server_IsConnected()) {
            Sleep(500);
        }

        printf("Wait disconnection: %d\n", i);
        while (easyRPC_Server_IsConnected()) {
            Sleep(500);
        }
    }

    printf("Finish\n");
    return 0;
}
