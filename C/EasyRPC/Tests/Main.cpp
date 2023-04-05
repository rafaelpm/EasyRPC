#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#include <stdio.h>
#include <string.h>
//#include "test_all.h"
#include "server/easy_rpc_server_windows_tcp.h"
#include "libdebug.h"

/* ---------------------------------------------------------------------------*/
int main(int argv, char *argc){
    //test_all();
    //test_wrap_unwrap();
    //test_client_connection();
    //test_builded_client_function();

    if (!easyRPC_ServerWindowsTCP_Setup((char*)"2000")) {
        set_red("easyRPC_ServerWindowsTCP_Setup = ERROR");
    }

    if (easyRPC_Server_Listen()) {
        if (!easyRPC_Server_IsConnected()) {
            set_red("easyRPC_Server_IsConnected = ERROR");
        }
    }else{
        set_red("easyRPC_Server_Listen = ERROR");
    }

    set_green("easyRPC_ServerWindowsTCP = OK");
        
    return 0;
}
/* ---------------------------------------------------------------------------*/