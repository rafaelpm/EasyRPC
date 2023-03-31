#ifndef _PARSER_FUNCTION_H
#define _PARSER_FUNCTION_H
/* ---------------------------------------------------------------------------*/
#include <string>
#include <vector>

using namespace std;

typedef enum {
	Void = 0,
	Integer,
	Float,
	Boolean,
	String,
	BinaryArray
} TypeData;

class DataInfo {
public:
	string name;	
	TypeData type;
};

class FunctionInfo {	
public:	
	string name;
	vector<DataInfo> params;
};
/* ---------------------------------------------------------------------------*/
class ParserFunctions {
private:
	vector<FunctionInfo> functions;
	bool GetParamsContent(int after, string content, string* params, int* startParamFunction, int* endParamFunction);
	bool GetNameFunction(int* startFunction, int* startParamFunction, string content, string* name, string* returnType);
	bool HasJunkContentStartFunction(int* startFunction, int* startName, string content);
	vector<char> junkTags;
	void buildJunkTags() {
		junkTags.push_back('\n');
		junkTags.push_back('\r');
		
	}
public:
	ParserFunctions() {
		buildJunkTags();
	};
	~ParserFunctions() {};

	void ParserContent(string content);
};
/* ---------------------------------------------------------------------------*/
void ParserFunctions::ParserContent(string content) {
	int startFunction = 0;
	string params, nameFunction, returnType;
	int startParamFunction=0, endParamFunction;

	while (GetParamsContent(startFunction, content, &params, &startParamFunction, &endParamFunction)) {
		if (!GetNameFunction(&startFunction, &startParamFunction, content, &nameFunction, &returnType)) {
			perror("ParserFunctions->ParserContent->GetNameFunction = Fail");
			return;
		}

		startFunction = endParamFunction;
	}
}
/* ---------------------------------------------------------------------------*/
bool ParserFunctions::GetNameFunction(int *startFunction, int *startParamFunction, string content, string *name, string *returnType) {
	int startName;
		
	do {
		startName = content.find(" ", *startFunction);
		if (startName < 0) {
			return false;
		}
		if (!HasJunkContentStartFunction(startFunction, &startName, content)) {
			break;
		}
	} while (true);

	*name = content.substr(startName+1, (*startParamFunction - (startName + 1)));
	*returnType = content.substr(*startFunction, startName - *startFunction);
	return true;
}
/* ---------------------------------------------------------------------------*/
bool ParserFunctions::HasJunkContentStartFunction(int *startFunction, int *startName, string content) {
	int junkContent = 0;
	for (int i = 0; i < junkTags.size(); i++) {
		junkContent = content.find(junkTags.at(i), *startFunction);
		if (junkContent >= 0 && junkContent < *startName) {
			*startFunction = junkContent + 1;
			return true;
		}
	}
	return false;
}
/* ---------------------------------------------------------------------------*/
bool ParserFunctions::GetParamsContent(int after, string content, string *params, int *startParamFunction, int *endParamFunction) {	
	*startParamFunction = content.find("(", after);
	if (*startParamFunction < 0) {
		return false;
	}
	*endParamFunction = content.find(")", *startParamFunction);
	if (*endParamFunction < 0) {
		return false;
	}
	*params = content.substr(*startParamFunction+1, *endParamFunction - ( * startParamFunction+1));
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif