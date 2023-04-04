#ifndef _BUILD_FUNCTION_H
#define _BUILD_FUNCTION_H
/* ---------------------------------------------------------------------------*/
#include <string.h>
using namespace std;
class BuildFunction {
	string buildAddFunctionsInList(ParserFunctions* parserFunctions);
	
	string addCodeWithSpace(string code) { return "    " + code; };

public:
	BuildFunction() {};
	~BuildFunction() {};

	string buildToContent(ParserFunctions* parserFunctions);
};
/* ---------------------------------------------------------------------------*/
string BuildFunction::buildToContent(ParserFunctions* parserFunctions) {
	string content = buildAddFunctionsInList(parserFunctions);

	return content;
}
/* ---------------------------------------------------------------------------*/
string BuildFunction::buildAddFunctionsInList(ParserFunctions* parserFunctions) {
	string content = "";

	for (int i = 0; i < parserFunctions->functions.size(); i++) {
		content += addCodeWithSpace("addEasyRPC_FunctionBind();\n");
	}

	return content;
}
/* ---------------------------------------------------------------------------*/
#endif