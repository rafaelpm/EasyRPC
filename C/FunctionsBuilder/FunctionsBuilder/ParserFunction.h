#ifndef _PARSER_FUNCTION_H
#define _PARSER_FUNCTION_H
/* ---------------------------------------------------------------------------*/
#include <string>
#include <vector>
#include "Trim.h"
#include "TypeData.h"
/* ---------------------------------------------------------------------------*/
using namespace std;


/* ---------------------------------------------------------------------------*/
class DataInfo {
private:
	TypeDataParser typeDataParser;
public:
	string name;	
	TypeData type;

	string toParam() {
		return toParam(false);
	}

	string toParam(bool pointer) {
		if (type == BinaryArray) {
			return typeDataParser.TypeToName(type) + " " +(pointer ? "*":"")+ name + ", uint16_t " + name + "_len";
		}
		return typeDataParser.TypeToName(type) + " " + (pointer ? "*" : "") + name;
	}

};
/* ---------------------------------------------------------------------------*/
class FunctionInfo {	
public:	
	string name;
	DataInfo returnInfo;
	vector<DataInfo*> params;
	int position;

	string nameFunction;
	string contentFunction;
};
/* ---------------------------------------------------------------------------*/
class ParserFunctions {
private:	
	bool GetParamsContent(int after, string content, string* params, int* startParamFunction, int* endParamFunction);
	bool GetNameFunction(int* startFunction, int* startParamFunction, string content, string* name, string* returnType);	
	void ExtractParams(string* paramsFull, vector<string> *paramsList);
	void InsertParams(string* params, FunctionInfo* functionInfo);
	TypeDataParser typeDataParser;

	void checkPointer(string *name, string *returnType) {
		if (name->find("*") == 0) {
			*name = name->substr(1, name->length() - 1);
			*returnType += "*";
		}
	}

	//Junk - Begin
	bool HasJunkContentStartFunction(int* startFunction, int* startName, string content);
	vector<char> junkTags;
	void buildJunkTags() {
		junkTags.push_back('\n');
		junkTags.push_back('\r');		
	}
	//Junk - End
public:
	ParserFunctions() {
		buildJunkTags();
	};
	~ParserFunctions() {};

	vector<FunctionInfo*> functions;

	void ParserContent(string content);
};
/* ---------------------------------------------------------------------------*/
void ParserFunctions::ParserContent(string content) {
	functions.clear();

	int startFunction = 0;
	string params, nameFunction, returnType;
	int startParamFunction=0, endParamFunction;
	FunctionInfo* functionInfo;

	while (GetParamsContent(startFunction, content, &params, &startParamFunction, &endParamFunction)) {
		if (!GetNameFunction(&startFunction, &startParamFunction, content, &nameFunction, &returnType)) {
			perror("ParserFunctions->ParserContent->GetNameFunction = Fail");
			return;
		}
		functionInfo = new FunctionInfo();
		functionInfo->name = nameFunction;
		functionInfo->returnInfo.name = "returnValue";
		functionInfo->returnInfo.type = typeDataParser.NameToType(&returnType);
		functionInfo->position = functions.size();

		InsertParams(&params, functionInfo);

		functions.push_back(functionInfo);

		startFunction = endParamFunction;
	}
}
/* ---------------------------------------------------------------------------*/
void ParserFunctions::InsertParams(string* params, FunctionInfo *functionInfo) {
	vector<string> paramsList;		
	string param, name, typeName;
	int spaceIndex;
	DataInfo *paramInfo;

	ExtractParams(params, &paramsList);

	for (int i = 0; i < paramsList.size(); i++) {
		param = paramsList.at(i);
		spaceIndex = param.find(" ");
		if (spaceIndex < 0) {
			printf("Full Params: %s\n", *params);
			printf("Param with problem: %s\n",param);
			perror("ParserFunctions->ParserContent->InsertParams: Space between var and type not found.");
			continue;
		}
		typeName = trim_str(param.substr(0, spaceIndex));
		name = trim_str(param.substr(spaceIndex));

		checkPointer(&name, &typeName);

		paramInfo = new DataInfo();
		paramInfo->name = name;
		paramInfo->type = typeDataParser.NameToType(&typeName);

		functionInfo->params.push_back(paramInfo);
	}
}
/* ---------------------------------------------------------------------------*/
void ParserFunctions::ExtractParams(string* paramsFull, vector<string> *paramsList) {
	if (paramsFull->length() == 0) {
		return;
	}
	int startIndex = 0, tagPosition;

	do {
		tagPosition = paramsFull->find(",", startIndex);
		if (tagPosition < 0) {
			if (startIndex < paramsFull->length()) {
				tagPosition = paramsFull->length();
				paramsList->push_back(trim_str(paramsFull->substr(startIndex, tagPosition)));
			}
			break;
		}
		paramsList->push_back(trim_str(paramsFull->substr(startIndex, tagPosition- startIndex)));
		startIndex = tagPosition + 1;		
	} while (tagPosition >= 0);

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

	*name = trim_str(content.substr(startName+1, (*startParamFunction - (startName + 1))));	
	*returnType = trim_str(content.substr(*startFunction, startName - *startFunction));
	checkPointer(name, returnType);

	/*if (name->find("*") == 0) {
		*name = name->substr(1, name->length() - 1);
		*returnType += "*";
	}*/
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
	*params = trim_str(*params);	
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif
