#ifndef _TEST_BUILDED_CLIENT_FUNCTION_H
#define _TEST_BUILDED_CLIENT_FUNCTION_H
/* ---------------------------------------------------------------------------*/
#include "client/easy_rpc_client_windows_tcp.h"
#include "easy_rpc_remote_client.h"
#include "libdebug.h"
/* ---------------------------------------------------------------------------*/
void test_builded_client_function() {
	easyRPC_ProcessData = easyRPC_ProcessDataFromServer;
	easyRPC_ClientWindowsTCP_Setup((char*)"127.0.0.1", (char*)"2000");
		
	float retFloatValue = 0;
	if (!remote_subtract(&retFloatValue, 3.5, 2)) {
		set_red("remote_subtract = ERROR");
	} else if (retFloatValue != 1.5) {
		set_red("remote_subtract->value = ERROR");
	}	

	int retIntValue = 0;
	if (!remote_sum(&retIntValue, 1, 4)) {
		set_red("remote_sum = ERROR");
	} else if (retIntValue != 5) {
		set_red("remote_sum->value = ERROR");
	}

	if (!remote_print((char *)"Test Build in C!")) {
		set_red("remote_print = ERROR");
	}

	uint8_t data[] = {1,2,3,4,5,6,7,8,9,10};
	if (!remote_binaryData(data,10)) {
		set_red("remote_binaryData = ERROR");
	}

	set_green("test_builded_client_function = OK");
}
/* ---------------------------------------------------------------------------*/
#endif
