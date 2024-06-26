#ifndef _BUILD_CLIENT_FUNCTIONS_H
#define _BUILD_CLIENT_FUNCTIONS_H
/* ---------------------------------------------------------------------------*/
#include <string.h>
#include "SaveContent.h"
using namespace std;
class BuildClientFunctions {
protected:
	string buildBeforeSend();
	string buildAfterSend();

	string buildHeaders();
	string buildTitleFunction(FunctionInfo* function);
	string buildParamsFunction(FunctionInfo* function);
	string buildFunction(FunctionInfo* function);
	string spaceLine() { return "/* ---------------------------------------------------------------------------*/\n"; };

	DataInfo* param;
	TypeDataParser typeDataParser;
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
	SaveContent saveContent;
public:
	BuildClientFunctions() {
		buildBeginSpace(1, 0);
	};
	~BuildClientFunctions() {};

	string buildToContent(ParserFunctions* parserFunctions);
	string contentFile;

	bool saveFileOnPath(string path) {
		return saveFile(path + "easy_rpc_remote_client.h");
	};

	bool saveFile(string name) {
		return saveContent.save(name, contentFile);
	};
};

/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildToContent(ParserFunctions* parserFunctions) {	
	FunctionInfo* function;

	contentFile = buildHeaders();
	contentFile += spaceLine();
	contentFile += buildBeforeSend();
	contentFile += spaceLine();
	contentFile += buildAfterSend();
	
	
	for (int f = 0; f < parserFunctions->functions.size(); f++) {
		function = parserFunctions->functions.at(f);
		contentFile += buildFunction(function);
	}

	contentFile += spaceLine();
	contentFile += "#endif";

	//printf("%s", contentFile.c_str());

	return contentFile;
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildBeforeSend() {
	string content = "";
	buildBeginSpace(0, 0);
	content += addCodeWithSpace("void easyRPC_Client_BeforeSend(){\n");
	buildBeginSpace(1, 0);
	content += addCodeWithSpace("easyRPC_ProcessData = easyRPC_ProcessDataFromServer;\n");
	content += addCodeWithSpace("//PUT YOUR CODE HERE\n");
	buildBeginSpace(0, 0);
	content += addCodeWithSpace("}\n");	
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildAfterSend() {
	string content = "";
	buildBeginSpace(0, 0);
	content += addCodeWithSpace("void easyRPC_Client_AfterSend(){\n");
	buildBeginSpace(1, 0);
	content += addCodeWithSpace("easyRPC_ProcessData = easyRPC_ProcessDataFromClient;\n");
	content += addCodeWithSpace("//PUT YOUR CODE HERE\n");
	buildBeginSpace(0, 0);
	content += addCodeWithSpace("}\n");	
	return content;	
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildFunction(FunctionInfo* function) {
	string content = "";
	content += spaceLine();
	content += buildTitleFunction(function);
	buildBeginSpace(1, 0);
	content += addCodeWithSpace("easyRPC_Client_BeforeSend();\n");
	content += addCodeWithSpace("if (!easyRPC_ClientConnection_IsConnected()) {\n");
	content += addCodeWithSpace("  if (!easyRPC_ClientConnection_Connect()) { easyRPC_Client_AfterSend(); return false; }\n");
	content += addCodeWithSpace("}\n\n");
	content += addCodeWithSpace("//Build package\n");
	content += addCodeWithSpace("resetEasyRPC_Package(&easyRPC_clientPackage);\n");
	content += addCodeWithSpace("setEasyRPC_NameFunction(&easyRPC_clientPackage, (char *)\"" + function->name + "\");\n");
	content += addCodeWithSpace("easyRPC_clientPackage.returnInfo.type = " + typeDataParser.TypeToNameObject(function->returnInfo.type) + ";\n");
	content += buildParamsFunction(function);
	content += "\n";
	content += addCodeWithSpace("//Prepare stream to send\n");
	content += addCodeWithSpace("Stream streamData, streamToServer, streamFromServer;\n");
	content += addCodeWithSpace("resetStream(&streamToServer); resetStream(&streamFromServer);\n");
	content += addCodeWithSpace("easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);\n");
	content += addCodeWithSpace("wrapData(&streamToServer, &streamData.buffer[0], streamData.size);\n");

	content += "\n";
	content += addCodeWithSpace("//Send and receive data\n");
	content += addCodeWithSpace("if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { easyRPC_Client_AfterSend(); return false; }\n");
	content += addCodeWithSpace("if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }\n");

	content += addCodeWithSpace("//Check receive ACK\n");
	content += addCodeWithSpace("EasyRPCPackage packageFromServer;\n");
	content += addCodeWithSpace("unwrapData(&streamFromServer, &packageFromServer);\n");
	content += addCodeWithSpace("if(!isACK_EasyRPC()){ easyRPC_Client_AfterSend(); return false; }\n");
	
	content += addCodeWithSpace("//Read response\n");
	content += addCodeWithSpace("resetStream(&streamFromServer);\n");
	content += addCodeWithSpace("if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { easyRPC_Client_AfterSend(); return false; }\n");

	if (function->returnInfo.type != Void) {
		content += addCodeWithSpace("unwrapPosition = 0;\n");		
		content += addCodeWithSpace("unwrapData(&streamFromServer, &packageFromServer);\n");
		content += addCodeWithSpace("if(!" + typeDataParser.TypeToReturnNameGetFunction(function->returnInfo.type) + "(&packageFromServer, returnValue)) { easyRPC_Client_AfterSend(); return false; }\n");
	}

	content += addCodeWithSpace("easyRPC_ClientConnection_Disconnect();\n");
	content += addCodeWithSpace("easyRPC_Client_AfterSend();\n");

	content += addCodeWithSpace("return true;\n");
	buildBeginSpace(0, 0);
	content += "}\n";
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildParamsFunction(FunctionInfo* function) {
	string content = "";
	for (int p = 0; p < function->params.size(); p++) {
		param = function->params.at(p);
		content += addCodeWithSpace(typeDataParser.TypeToNameSetFunction(param->type)+"(&easyRPC_clientPackage, "+param->name);		
		if (param->type == BinaryArray) {
			content += ", "+param->name + "_len);\n";
		} else {
			content += ");\n";
		}
	}
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildTitleFunction(FunctionInfo *function) {
	string content = "";
	

	buildBeginSpace(0, 0);
	content += addCodeWithSpace("bool remote_" + function->name + "(");

	if (function->returnInfo.type != Void) {	
		content += function->returnInfo.toParam(true);
	}

	for (int p = 0; p < function->params.size(); p++) {
		param = function->params.at(p);
		if (p == 0 && function->returnInfo.type == Void) {
			content += param->toParam();		
		} else {
			content += ", " + param->toParam();
		}
	}
	content += "){\n";
	
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildHeaders() {
	buildBeginSpace(0, 0);
	string content = addCodeWithSpace("#ifndef _EASY_RPC_REMOTE_CLIENT\n");
	content += addCodeWithSpace("#define _EASY_RPC_REMOTE_CLIENT\n");
	content += spaceLine();
	content += addCodeWithSpace("#include \"client/easy_rpc_client.h\"\n");
	content += addCodeWithSpace("#include \"package_builders/build_package_to_client.h\"\n");
	content += addCodeWithSpace("#include \"package_builders/build_package_from_client_or_server.h\"\n");
	//content += spaceLine();
			
	return content;
}
/* ---------------------------------------------------------------------------*/
#endif