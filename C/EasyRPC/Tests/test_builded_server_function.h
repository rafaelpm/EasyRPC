#ifndef _TEST_BUILDED_SERVER_FUNCTION_H
#define _TEST_BUILDED_SERVER_FUNCTION_H
/* ---------------------------------------------------------------------------*/
#include "server/easy_rpc_server_windows_tcp.h"
#include "libdebug.h"
/* ---------------------------------------------------------------------------*/
void test_builded_server_function() {
    if (!easyRPC_ServerWindowsTCP_Setup((char*)"2000")) {
        set_red("easyRPC_ServerWindowsTCP_Setup = ERROR");
    }

    if (easyRPC_Server_Listen()) {
        if (!easyRPC_Server_IsConnected()) {
            set_red("easyRPC_Server_IsConnected = ERROR");
        }
    } else {
        set_red("easyRPC_Server_Listen = ERROR");
    }

    set_green("easyRPC_ServerWindowsTCP = OK");
}
/* ---------------------------------------------------------------------------*/
#endif