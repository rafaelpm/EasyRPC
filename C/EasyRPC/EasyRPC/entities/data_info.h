#ifndef _DATA_INFO_H
#define _DATA_INFO_H
/* ---------------------------------------------------------------------------*/
#include "type_data.h"
#include <stdlib.h>

#ifndef EASY_RPC_DATA_INFO_VALUE_SIZE
	#define EASY_RPC_DATA_INFO_VALUE_SIZE 255
#endif

typedef struct {
	char value[EASY_RPC_DATA_INFO_VALUE_SIZE];
	uint16_t valueSize;
	TypeData type;
} DataInfo;

/* ---------------------------------------------------------------------------*/
void setEasyRPC_DataInfo_String(char *value, DataInfo* returnDataInfo) {
	returnDataInfo->type = StringRPC;
	snprintf(returnDataInfo->value, EASY_RPC_DATA_INFO_VALUE_SIZE, "%s", value);
	returnDataInfo->valueSize = strlen(returnDataInfo->value);
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_DataInfo_Array(uint8_t* value, uint16_t len, DataInfo* returnDataInfo) {
	returnDataInfo->type = BinaryArray;
	memcpy(returnDataInfo->value,  value, len);	
	returnDataInfo->valueSize = len;
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_DataInfo_Int(int value, DataInfo *returnDataInfo) {
	returnDataInfo->type = Integer;	
	snprintf(returnDataInfo->value, EASY_RPC_DATA_INFO_VALUE_SIZE, "%d", value);
	returnDataInfo->valueSize = strlen(returnDataInfo->value);
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_DataInfo_Float(float value, DataInfo* returnDataInfo) {
	returnDataInfo->type = Float;	
	snprintf(returnDataInfo->value, EASY_RPC_DATA_INFO_VALUE_SIZE, "%f", value);
	returnDataInfo->valueSize = strlen(returnDataInfo->value);
}
/* ---------------------------------------------------------------------------*/
void setEasyRPC_DataInfo_Boolean(bool value, DataInfo* returnDataInfo) {
	returnDataInfo->type = Boolean;
	returnDataInfo->value[0] = value ? 't' : 'f';
	returnDataInfo->value[1] = 0;
	returnDataInfo->valueSize = 1;
}
/* ---------------------------------------------------------------------------*/
#endif