#ifndef _TYPE_DATA_H
#define _TYPE_DATA_H
/* ---------------------------------------------------------------------------*/
#include <string>

using namespace std;

typedef enum {
	Void = 0,
	Integer,
	Float,
	Boolean,
	String,
	BinaryArray
} TypeData;

class TypeDataParser {

public:
	string TypeToName(TypeData type);
	TypeData NameToType(string* name);
	string TypeToNameGetFunction(TypeData type);
	string TypeToNameSetFunction(TypeData type);

};
/* ---------------------------------------------------------------------------*/
string TypeDataParser::TypeToName(TypeData type) {
	if (type == Integer) {
		return "int";
	} else if (type == Float) {
		return "float";
	} else if (type == Boolean) {
		return "bool";
	} else if (type == String) {
		return "char *";
	} else if (type == BinaryArray) {
		return "uint8_t *";
	}
	return "void";
}
/* ---------------------------------------------------------------------------*/
string TypeDataParser::TypeToNameGetFunction(TypeData type) {
	if (type == Integer) {
		return "getEasyRPC_Param_Integer";
	} else if (type == Float) {
		return "getEasyRPC_Param_Float";
	} else if (type == Boolean) {
		return "getEasyRPC_Param_Boolean";
	} else if (type == String) {
		return "getEasyRPC_Param_String";
	} else if (type == BinaryArray) {
		return "getEasyRPC_Param_Array";
	}
	return "";
}
/* ---------------------------------------------------------------------------*/
string TypeDataParser::TypeToNameSetFunction(TypeData type) {
	if (type == Integer) {
		return "setEasyRPC_Param_Integer";
	} else if (type == Float) {
		return "setEasyRPC_Param_Float";
	} else if (type == Boolean) {
		return "setEasyRPC_Param_Boolean";
	} else if (type == String) {
		return "setEasyRPC_Param_String";
	} else if (type == BinaryArray) {
		return "setEasyRPC_Param_Array";
	}
	return "";
}
/* ---------------------------------------------------------------------------*/
TypeData TypeDataParser::NameToType(string* name) {
	if (strcmp(name->c_str(), "int") == 0) {
		return Integer;
	} else if (strcmp(name->c_str(), "float") == 0 ||
		strcmp(name->c_str(), "double") == 0) {
		return Float;
	} else if (strcmp(name->c_str(), "bool") == 0) {
		return Boolean;
	} else if (strcmp(name->c_str(), "char*") == 0) {
		return String;
	} else if (strcmp(name->c_str(), "uint8_t*") == 0 ||
		strcmp(name->c_str(), "byte") == 0) {
		return BinaryArray;
	}
	return Void;
}
/* ---------------------------------------------------------------------------*/
#endif