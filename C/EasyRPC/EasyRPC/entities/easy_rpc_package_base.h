#ifndef _EASY_RPC_PACKAGE_BASE_H
#define _EASY_RPC_PACKAGE_BASE_H
/* ---------------------------------------------------------------------------*/
//#define SIZE_BUFFER_STREAM 2048
#include "libstream.h"
#include "entities/easy_rpc_package.h"

#define HEADER 0x55
#define VERSION 1
uint8_t sequence = 0, version = 0, reserved = 0;
uint16_t checksum = 0, unwrapPosition = 0, sizePackage = 0;
#define MAX_POSITIONS 7
/* ---------------------------------------------------------------------------*/

typedef enum {
	Unwrap_Incomplete,
	Unwrap_Complete,
	Unwrap_Error
} UnwrapStates;
/* ---------------------------------------------------------------------------*/
typedef bool (*EasyRPC_ProcessDataCallback)(Stream* stream, EasyRPCPackage* packageReturn);
//typedef bool (*EasyRPC_ToBytesCallback)(EasyRPCPackage *package, Stream* streamReturn);

EasyRPC_ProcessDataCallback easyRPC_ProcessData;
//EasyRPC_ToBytesCallback easyRPC_ToBytes;*/
/* ---------------------------------------------------------------------------*/
UnwrapStates unwrapData(Stream* streamIn, EasyRPCPackage* packageOut) {
	resetEasyRPC_Package(packageOut);
	byte header = 0;

	while (unwrapPosition < MAX_POSITIONS) {
		switch (unwrapPosition) {
			case 0:
				
				do {					
					if (!readByte(streamIn, &header)) {
						return Unwrap_Incomplete;
					}
				} while (header != HEADER);
				unwrapPosition++;
				break;
			case 1:
				if (!readByte(streamIn, &version)) {
					return Unwrap_Incomplete;
				}
				if (version > VERSION) {
					printf("Error: Difference between protocol version:\nReceived: %d / Expected: %d\n",version, VERSION);
					perror("Error: Difference between protocol version, details above.");
					return Unwrap_Incomplete;
				}
				unwrapPosition++;
				break;
			case 2:
				if (!readByte(streamIn, &sequence)) {
					return Unwrap_Incomplete;
				}
				unwrapPosition++;
				break;
			case 3:
				if (!readShort_Inverted(streamIn, &checksum)) {
					return Unwrap_Incomplete;
				}
				unwrapPosition++;
				break;
			case 4:
				if (!readByte(streamIn, &reserved)) {
					return Unwrap_Incomplete;
				}
				unwrapPosition++;
				break;
			case 5:
				if (!readShort_Inverted(streamIn, &sizePackage)) {
					return Unwrap_Incomplete;
				}
				unwrapPosition++;
				break;
			case 6:
				if (isReadEOS_Plus(streamIn, sizePackage)) {
					return Unwrap_Incomplete;
				}

				unwrapPosition = 0;
				if (!easyRPC_ProcessData(streamIn, packageOut)) {
					return Unwrap_Error;
				}
				
				return Unwrap_Complete;
		}
	}

	return Unwrap_Incomplete;
}
/* ---------------------------------------------------------------------------*/
void wrapData(Stream* streamOut, byte *data, uint16_t data_len) {	
	startToWrite(streamOut);
	writeByte(streamOut, HEADER);
	writeByte(streamOut, VERSION);
	writeByte(streamOut, sequence);
	writeShort(streamOut, checksum);
	writeByte(streamOut, 0);//Reserved
	writeShort_Inverted(streamOut, data_len);
	writeArray(streamOut, (uint8_t *)data, data_len);
}
/* ---------------------------------------------------------------------------*/
bool setEasyRPC_BinaryArray(Stream* stream, uint8_t* data, int len) {
	if (!writeShort_Inverted(stream, len)) {
		return false;
	}
	if (!writeArray(stream, data, len)) {
		return false;
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_BinaryArray(Stream* stream, uint8_t* contentReturn, uint16_t* contentLen) {
	uint16_t len = 0;
	if (!readShort_Inverted(stream, &len)) {
		return false;
	}
	if (!readArray(stream, contentReturn, len)) {
		return false;
	}
	if (contentLen != NULL) {
		*contentLen = len;
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_StringLen(Stream* stream, uint8_t* contentReturn, uint16_t* contentLen) {
	uint8_t len = 0;
	if (!readByte(stream, &len)) {
		return false;
	}	
	if (!readArray(stream, contentReturn, len)) {
		return false;
	}
	contentReturn[len] = 0;
	if (contentLen != NULL) {
		*contentLen = len;		
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_String(Stream* stream, uint8_t* contentReturn) {
	return getEasyRPC_StringLen(stream, contentReturn, NULL);
}
/* ---------------------------------------------------------------------------*/
bool setEasyRPC_String(Stream* stream, uint8_t* str, int len) {
	if (!writeByte(stream, len)) {
		return false;
	}
	if (!writeArray(stream, str, len)) {
		return false;
	}
	return true;
}

/* ---------------------------------------------------------------------------*/
#endif

