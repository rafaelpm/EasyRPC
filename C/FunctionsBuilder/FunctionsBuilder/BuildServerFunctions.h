#ifndef _BUILD_SERVER_FUNCTIONS_H
#define _BUILD_SERVER_FUNCTIONS_H
/* ---------------------------------------------------------------------------*/
#include <string.h>
#include "SaveContent.h"

using namespace std;
class BuildServerFunctions {
	char bufferTemp[100];

	string buildHeaderFunctions(ParserFunctions* parserFunctions);

	string buildParamsFunction(FunctionInfo* function, bool addType);
	string buildParamsFunction(FunctionInfo* function) { return buildParamsFunction(function, false); };

	string buildAddFunctionsInList(ParserFunctions* parserFunctions);
	string buildFunctionToRunFunctionApp(FunctionInfo* function);
	string buildRunFunctionApp(ParserFunctions* parserFunctions);

	string buildHeaders();
	string spaceLine() { return "/* ---------------------------------------------------------------------------*/\n"; };

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
	string contentFile;
	SaveContent saveContent;
public:
	BuildServerFunctions() {		
		buildBeginSpace(1,0);
	};
	~BuildServerFunctions() {};

	string buildToContent(ParserFunctions* parserFunctions);

	bool saveFileOnPath(string path) {
		return saveFile(path + "easy_rpc_server_bind.h");
	};

	bool saveFile(string name) {
		return saveContent.save(name, contentFile);
	};
};
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildToContent(ParserFunctions* parserFunctions) {
	contentFile = buildHeaders();

	contentFile += buildHeaderFunctions(parserFunctions);
	contentFile += spaceLine();
	contentFile += buildAddFunctionsInList(parserFunctions);
	contentFile += spaceLine();
	contentFile += buildRunFunctionApp(parserFunctions);
	contentFile += spaceLine();
	contentFile += "void easyRPC_Server_Bind_Functions(){\n";
	contentFile += addCodeWithSpace("easyRPC_RunFunction = easyRPC_RunFunctionApp;\n");
	contentFile += addCodeWithSpace("easyRPC_AddFunctions();\n");
	contentFile += "}\n";
	contentFile += spaceLine();

	contentFile += "#endif";

	//printf("%s", contentFile.c_str());

	return contentFile;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildRunFunctionApp(ParserFunctions* parserFunctions) {
	string content = "";
	FunctionInfo* function;

	for (int i = 0; i < parserFunctions->functions.size(); i++) {
		function = parserFunctions->functions.at(i);
		content += "bool easyRPC_RunFunction_"+ function->name+"(EasyRPCPackage* package){\n";
		content += buildFunctionToRunFunctionApp(function);
		content += "}\n";
		content += spaceLine();
	}	
	content += "bool easyRPC_RunFunctionApp(EasyRPCPackage* package, int index){\n";

	buildBeginSpace(1,0);
	content += addCodeWithSpace("switch(index){\n");
	
	for (int i = 0; i < parserFunctions->functions.size(); i++) {
		function = parserFunctions->functions.at(i);
		//content += buildFunctionToRunFunctionApp(function);
		buildBeginSpace(1, 2);
		sprintf(bufferTemp, "case %d:\n", function->position);
		content += addCodeWithSpace(bufferTemp);
		content += addCodeWithSpace("  return easyRPC_RunFunction_" + function->name+"(package);\n");
	}
	buildBeginSpace(1,0);
	content += addCodeWithSpace("}\n");

	content += addCodeWithSpace("return false;\n");

	content += "}\n";
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildFunctionToRunFunctionApp(FunctionInfo* function) {
	string content = "";

	buildBeginSpace(1,0);

	//Declare vars
	DataInfo* param;
	for (int i = 0; i < function->params.size(); i++) {
		param = function->params.at(i);
		sprintf(bufferTemp, "%s param%d", typeDataParser.TypeToName(param->type, false).c_str(), i);
		content += addCodeWithSpace(bufferTemp);

		if (param->type == BinaryArray) {
			content += "[EASY_RPC_DATA_INFO_VALUE_SIZE];\n";
			content += addCodeWithSpace("memset(");
			sprintf(bufferTemp, "&param%d[0]", i);
			content += bufferTemp;
			content += ",0,EASY_RPC_DATA_INFO_VALUE_SIZE);\n";

			content += addCodeWithSpace("uint16_t ");
			sprintf(bufferTemp, "param%d_len", i);
			content += bufferTemp;
						
		}else if (param->type == String) {
			content += "[EASY_RPC_DATA_INFO_VALUE_SIZE];\n";
			content += addCodeWithSpace("memset(");
			sprintf(bufferTemp, "&param%d[0]", i);
			content += bufferTemp;
			content += ",0,EASY_RPC_DATA_INFO_VALUE_SIZE)";
		}
		content += ";\n";
	}

	//Check and get vars
	for (int i = 0; i < function->params.size(); i++) {
		param = function->params.at(i);		
		content += addCodeWithSpace("if(!" + typeDataParser.TypeToNameGetFunction(param->type) + "(package,");
		sprintf(bufferTemp, "&param%d", i);
		content += bufferTemp;

		if (param->type == BinaryArray) {
			content += "[0], ";
			content += bufferTemp;
			content += "_len";

		}else if (param->type == String) {
			content += "[0]";
		}
		content += ",";

		sprintf(bufferTemp, "%d)){\n", i);
		content += bufferTemp;

		content += addCodeWithSpace("  return false;\n");
		content += addCodeWithSpace("}\n");
	}

	//Call function
	if (function->returnInfo.type != Void) {
		//With return
		content += addCodeWithSpace(typeDataParser.TypeToName(function->returnInfo.type));
		content += " res_"+ function->name +" = "+function->name + buildParamsFunction(function);
	} else {
		//Void return
		content += addCodeWithSpace(function->name) + buildParamsFunction(function);
	}	
	content += ";\n";

	if (function->returnInfo.type != Void) {
		content += addCodeWithSpace(typeDataParser.TypeToReturnNameSetFunction(function->returnInfo.type) + "(package, res_" + function->name + ");\n");
	}

	content += addCodeWithSpace("return true;\n");

	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildParamsFunction(FunctionInfo* function, bool addType) {
	string content = "(";
	//Has params?
	if (function->params.size() == 0) {
		//No
		content += ")";
	} else {
		//Yes
		DataInfo *param;
		for (int i = 0; i < function->params.size(); i++) {
			param = function->params.at(i);
			if (addType) {
				content += param->toParam();
			} else {
				sprintf(bufferTemp, "param%d", i);
				content += bufferTemp;
				if (param->type == BinaryArray) {
					content += ", ";
					content += bufferTemp;
					content += "_len";
				}
			}

			if (i + 1 >= function->params.size()) {
				content += ")";
			} else {
				content += ",";
			}
		}
	}
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildHeaderFunctions(ParserFunctions* parserFunctions) {
	string content = "\n";
	FunctionInfo* function;

	buildBeginSpace(0, 0);
	for (int i = 0; i < parserFunctions->functions.size(); i++) {
		function = parserFunctions->functions.at(i);
		content += typeDataParser.TypeToName(function->returnInfo.type)+" "+ function->name;
		content += buildParamsFunction(function, true)+";\n";
	}
	
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildAddFunctionsInList(ParserFunctions* parserFunctions) {
	string content = "void easyRPC_AddFunctions(){\n";
	FunctionInfo* function;

	buildBeginSpace(1, 0);
	for (int i = 0; i < parserFunctions->functions.size(); i++) {
		function = parserFunctions->functions.at(i);		
		content += addCodeWithSpace("addEasyRPC_FunctionBind((char *)\""+ function->name +"\");\n");
	}
	content += "}\n";
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildHeaders() {
	buildBeginSpace(0, 0);
	string content = addCodeWithSpace("#ifndef _EASY_RPC_SERVER_BIND_H\n");
	content += addCodeWithSpace("#define _EASY_RPC_SERVER_BIND_H\n");
	content += spaceLine();
	content += addCodeWithSpace("#include \"server/easy_rpc_bind.h\"\n");
	content += spaceLine();

	return content;
}
/* ---------------------------------------------------------------------------*/
#endif