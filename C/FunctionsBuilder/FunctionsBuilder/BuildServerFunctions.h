#ifndef _BUILD_SERVER_FUNCTIONS_H
#define _BUILD_SERVER_FUNCTIONS_H
/* ---------------------------------------------------------------------------*/
#include <string.h>
using namespace std;
class BuildServerFunctions {
	char bufferTemp[100];

	string buildAddFunctionsInList(ParserFunctions* parserFunctions);
	string buildFunctionToRunFunctionApp(FunctionInfo* function);
	string buildRunFunctionApp(ParserFunctions* parserFunctions);
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
	BuildServerFunctions() {		
		buildBeginSpace(1,0);
	};
	~BuildServerFunctions() {};

	string buildToContent(ParserFunctions* parserFunctions);
};
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildToContent(ParserFunctions* parserFunctions) {
	string content = buildAddFunctionsInList(parserFunctions);

	content += buildRunFunctionApp(parserFunctions);

	content += "void easyRPC_Setup(){\n";
	content += addCodeWithSpace("easyRPC_RunFunction = easyRPC_RunFunctionApp;\n");
	content += addCodeWithSpace("easyRPC_AddFunctions();\n");
	content += "}\n";

	printf("%s",content.c_str());

	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildRunFunctionApp(ParserFunctions* parserFunctions) {
	string content = "bool easyRPC_RunFunctionApp(EasyRPCPackage* package, int index){\n";
	FunctionInfo* functionTemp;

	buildBeginSpace(1,0);
	content += addCodeWithSpace("switch(index){\n");
	
	for (int i = 0; i < parserFunctions->functions.size(); i++) {
		functionTemp = parserFunctions->functions.at(i);
		content += buildFunctionToRunFunctionApp(functionTemp);
	}
	buildBeginSpace(1,0);
	content += addCodeWithSpace("}\n");

	content += addCodeWithSpace("return true;\n");

	content += "}\n";
	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildFunctionToRunFunctionApp(FunctionInfo* function) {
	string content = "";

	buildBeginSpace(1, 2);
	
	sprintf(bufferTemp, "case %d:\n", function->position);
	content += addCodeWithSpace(bufferTemp);

	buildBeginSpace(1,4);

	//Declare vars
	DataInfo* param;
	for (int i = 0; i < function->params.size(); i++) {
		param = function->params.at(i);
		sprintf(bufferTemp, "%s param%d;\n", typeDataParser.TypeToName(param->type).c_str(), i);
		content += addCodeWithSpace(bufferTemp);		
	}

	//Check and get vars
	for (int i = 0; i < function->params.size(); i++) {
		param = function->params.at(i);		
		content += addCodeWithSpace("if(!" + typeDataParser.TypeToNameGetFunction(param->type) + "(package,");
		sprintf(bufferTemp, "&param%d,", i);
		content += bufferTemp;
		sprintf(bufferTemp, "%d)){\n", i);
		content += bufferTemp;

		content += addCodeWithSpace("  return false;\n");
		content += addCodeWithSpace("}\n");
	}

	//Call function
	if (function->returnInfo.type != Void) {
		//With return
		content += addCodeWithSpace(typeDataParser.TypeToName(function->returnInfo.type));
		content += " res = "+function->name + "(";
	} else {
		//Void return
		content += addCodeWithSpace(function->name + "(");
	}	

	//Has params?
	if (function->params.size() == 0) {
		//No
		content += ");\n";
	} else {
		//Yes
		for (int i = 0; i < function->params.size(); i++) {
			param = function->params.at(i);
			sprintf(bufferTemp, "param%d", i);
			content += bufferTemp;

			if (i + 1 >= function->params.size()) {
				content += ");\n";
			} else {
				content += ",";
			}
		}
	}

	if (function->returnInfo.type != Void) {
		content += addCodeWithSpace(typeDataParser.TypeToNameSetFunction(function->returnInfo.type) + "(package,res);\n");
	}

	content += addCodeWithSpace("break;\n");

	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildServerFunctions::buildAddFunctionsInList(ParserFunctions* parserFunctions) {
	string content = "void easyRPC_AddFunctions(){\n";
	FunctionInfo* functionTemp;
	for (int i = 0; i < parserFunctions->functions.size(); i++) {
		functionTemp = parserFunctions->functions.at(i);		
		content += addCodeWithSpace("addEasyRPC_FunctionBind(\""+ functionTemp->name +"\");\n");
	}
	content += "}\n";
	return content;
}

/* ---------------------------------------------------------------------------*/
#endif