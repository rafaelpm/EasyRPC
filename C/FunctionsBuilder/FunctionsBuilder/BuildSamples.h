#ifndef _BUILD_SAMPLES_H
#define _BUILD_SAMPLES_H
/* ---------------------------------------------------------------------------*/
#include <string.h>
#include "SaveContent.h"
/* ---------------------------------------------------------------------------*/
using namespace std;
class BuildSamples {
private:
    string spaceLine() { return "/* ---------------------------------------------------------------------------*/\n"; };
    string beginSpace;
	void buildBeginSpace(int tabs, int spaces) {
		beginSpace = "";
		for (int i = 0; i < tabs; i++) {
			beginSpace += "\t";
		}
		for (int i = 0; i < spaces; i++) {
			beginSpace += " ";
		}
	};
	
	string addCodeWithSpace(string code) { return beginSpace + code; };
    string buildSampleTCPServer();
    string buildSampleTCPClient();

    string contentFile;
    
	SaveContent saveContent;

public:
    BuildSamples(){
        
    };
    ~BuildSamples(){};

    bool saveFileOnPath(string path) {
        printf("Saving samples\n");
        contentFile = buildSampleTCPServer();
		if(!saveContent.save(path + "sample_server.h",contentFile)){
            return false;
        }
        contentFile = buildSampleTCPClient();
        if(!saveContent.save(path + "sample_client.h",contentFile)){
            return false;
        }
        return true;
	};

};
/* ---------------------------------------------------------------------------*/
string BuildSamples::buildSampleTCPServer() {
    buildBeginSpace(0, 0);
	string content = addCodeWithSpace("#ifndef _SAMPLE_SERVER_H\n");
    content += addCodeWithSpace("#define _SAMPLE_SERVER_H\n");
    content += spaceLine();
    content += addCodeWithSpace("//#include <server/easy_rpc_server_linux_tcp.h>\n");
    content += addCodeWithSpace("#include <server/easy_rpc_server_linux_udp.h>\n");
    content += addCodeWithSpace("#include \"easy_rpc/easy_rpc_server_bind.h\"\n");
    content += spaceLine();
    content += addCodeWithSpace("//PUT YOUR CODE HERE - BEGIN\n");
    content += addCodeWithSpace("//PUT YOUR CODE HERE - END\n");
    content += spaceLine();
    content += addCodeWithSpace("void start_easyrpc_server(){\n");
    buildBeginSpace(1, 0);
    content += addCodeWithSpace("easyRPC_Server_Bind_Functions();\n");
    content += addCodeWithSpace("//easyRPC_ServerLinuxTCP_Setup(2000);\n");
    content += addCodeWithSpace("easyRPC_ServerLinuxUDP_Setup(2000);\n");
    content += addCodeWithSpace("if (!easyRPC_Server_Listen()) { printf(\"Fail when start listen TCP port 2000!\\n\"); return; }\n");    
    content += addCodeWithSpace("while (true) {\n");
    buildBeginSpace(2, 0);
    content += addCodeWithSpace("//printf(\"Wait connection\\n\");\n");
    content += addCodeWithSpace("while (!easyRPC_Server_IsConnected()) { sleep(0.1); }\n");
    content += addCodeWithSpace("//printf(\"Wait disconnection\\n\");\n");
    content += addCodeWithSpace("while (easyRPC_Server_IsConnected()) { sleep(0.1); }\n");
    buildBeginSpace(1, 0);
    content += addCodeWithSpace("}\n");

    buildBeginSpace(0, 0);
    //End start_easyrpc_server
    content += addCodeWithSpace("}\n");

    content += addCodeWithSpace("#endif");
    return content;
}
/* ---------------------------------------------------------------------------*/
string BuildSamples::buildSampleTCPClient() {
    buildBeginSpace(0, 0);
	string content = addCodeWithSpace("#ifndef _SAMPLE_CLIENT_H\n");
    content += addCodeWithSpace("#define _SAMPLE_CLIENT_H\n");

    content += spaceLine();
    content += addCodeWithSpace("//#include <client/easy_rpc_client_linux_tcp.h>\n");
    content += addCodeWithSpace("#include <client/easy_rpc_client_linux_udp.h>\n");
    content += addCodeWithSpace("#include \"easy_rpc/easy_rpc_remote_client.h\"\n");
    content += spaceLine();

    content += addCodeWithSpace("void easyrpc_client_setup(){\n");
    buildBeginSpace(1, 0);
    content += addCodeWithSpace("easyRPC_ProcessData = easyRPC_ProcessDataFromServer;\n");
    content += addCodeWithSpace("easyRPC_ClientLinuxUDP_Setup((char*)\"127.0.0.1\", 2000);\n");
    content += addCodeWithSpace("//Sample call sum remote function\n");
    content += addCodeWithSpace("//int retIntValue = 0;\n");
    content += addCodeWithSpace("//if (!remote_sum(&retIntValue, 1, 4)) {\n");
    content += addCodeWithSpace("//printf(\"remote_sum = Error\\n\");\n");
    content += addCodeWithSpace("//}else{\n");
    content += addCodeWithSpace("//printf(\"remote_sum = Success\\n\");\n");
    content += addCodeWithSpace("//}\n");
    buildBeginSpace(0, 0);
    content += addCodeWithSpace("}\n");

    content += addCodeWithSpace("#endif");
    return content;
}
/* ---------------------------------------------------------------------------*/
#endif