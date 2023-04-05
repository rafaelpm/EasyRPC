#ifndef _BUILD_SERVER_FUNCTIONS_H
#define _BUILD_SERVER_FUNCTIONS_H
/* ---------------------------------------------------------------------------*/
#include <string.h>
using namespace std;
class BuildClientFunctions {
protected:
	string buildTitleFunction(FunctionInfo* function);
	string buildParamsFunction(FunctionInfo* function);

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
public:
	BuildClientFunctions() {
		buildBeginSpace(1, 0);
	};
	~BuildClientFunctions() {};

	string buildToContent(ParserFunctions* parserFunctions);
};
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildToContent(ParserFunctions* parserFunctions) {
	string content = "";

	FunctionInfo* function;
	
	for (int f = 0; f < parserFunctions->functions.size(); f++) {
		function = parserFunctions->functions.at(f);
		content += buildTitleFunction(function);
		buildBeginSpace(1, 0);
		content += addCodeWithSpace("if (!easyRPC_ClientConnection_IsConnected()) {\n");
		content += addCodeWithSpace("  if (!easyRPC_ClientConnection_Connect()) { return false; }\n");		
		content += addCodeWithSpace("}\n\n");
		content += addCodeWithSpace("//Build package\n");
		content += addCodeWithSpace("resetEasyRPC_Package(&easyRPC_clientPackage);\n");
		content += addCodeWithSpace("setEasyRPC_NameFunction((char *)\""+ function->name +"\");\n");
		content += addCodeWithSpace("easyRPC_clientPackage.returnInfo.type = "+typeDataParser.TypeToNameObject(function->returnInfo.type) + ";\n");
		content += buildParamsFunction(function);
		content += "\n";
		content += addCodeWithSpace("//Prepare stream to send\n");
		content += addCodeWithSpace("Stream streamData, streamToServer, streamFromServer;\n");
		content += addCodeWithSpace("resetStream(&streamToServer); resetStream(&streamFromServer);\n");
		content += addCodeWithSpace("easyRPC_toBytesToServer(&easyRPC_clientPackage, &streamData);\n");
		content += addCodeWithSpace("wrapData(&streamToServer, &streamData.buffer[0], streamData.size);\n");

		content += "\n";
		content += addCodeWithSpace("//Send and receive data\n");
		content += addCodeWithSpace("if (!easyRPC_ClientConnection_Send(&streamToServer.buffer[0], streamToServer.size)) { return false; }\n");
		content += addCodeWithSpace("if (!easyRPC_ClientConnection_Receive(&streamFromServer.buffer[0], &streamFromServer.size, 1000)) { return false; }\n");
		content += addCodeWithSpace("return true;\n");		
		buildBeginSpace(0, 0);
		content += "}\n";
	}

	printf("%s", content.c_str());

	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildParamsFunction(FunctionInfo* function) {
	string content = "";
	for (int p = 0; p < function->params.size(); p++) {
		param = function->params.at(p);
		content += addCodeWithSpace(typeDataParser.TypeToNameSetFunction(param->type)+"(&easyRPC_clientPackage, "+param->name + ");\n");		
	}
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildClientFunctions::buildTitleFunction(FunctionInfo *function) {
	string content = "";
	

	buildBeginSpace(0, 0);
	content += addCodeWithSpace("bool remote_" + function->name + "(");

	if (function->returnInfo.type != Void) {		
		content += function->returnInfo.toParam();
	}

	for (int p = 0; p < function->params.size(); p++) {
		param = function->params.at(p);
		content += ", "+ param->toParam();
	}
	content += "){\n";
	
	return content;
}
/* ---------------------------------------------------------------------------*/
#endif