#ifndef _DATA_INFO_H
#define _DATA_INFO_H
/* ---------------------------------------------------------------------------*/
#include "type_data.h"

#define EASY_RPC_DATA_INFO_VALUE_SIZE 50

typedef struct {
	char value;
	TypeData type;
} DataInfo[EASY_RPC_DATA_INFO_VALUE_SIZE];
/* ---------------------------------------------------------------------------*/
#endif