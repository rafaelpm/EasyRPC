#ifndef _EASY_RPC_PACKAGE_BASE_H
#define _EASY_RPC_PACKAGE_BASE_H
/* ---------------------------------------------------------------------------*/
//#define SIZE_BUFFER_STREAM 2048
#include "libstream.h"
#include "easy_rpc_package.h"

#define HEADER 0x55
#define VERSION 1
uint8_t sequence = 0, version = 0;
uint16_t checksum = 0;
/* ---------------------------------------------------------------------------*/
void wrapData(Stream* stream, byte *data, uint16_t data_len) {
	startToWrite(stream);
	writeByte(stream, HEADER);
	writeByte(stream, VERSION);
	writeByte(stream, sequence);
	writeShort(stream, checksum);
	writeByte(stream, 0);//Reserved
	writeShort(stream, data_len);
	writeArray(stream, (uint8_t *)data, data_len);	
}
/* ---------------------------------------------------------------------------*/
bool setEasyRPC_BinaryArray(Stream* stream, uint8_t* data, int len) {
	if (!writeShort(stream, len)) {
		return false;
	}
	if (!writeArray(stream, data, len)) {
		return false;
	}
	return true;
}
/* ---------------------------------------------------------------------------*/
bool getEasyRPC_String(Stream* stream, uint8_t* contentReturn) {
	uint8_t len = 0;
	if (!readByte(stream, &len)) {
		return false;
	}
	if (!readArray(stream, contentReturn, len)) {
		return false;
	}
	return true;
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
typedef bool (*EasyRPC_ProcessDataCallback)(Stream* stream, EasyRPCPackage* packageReturn);
typedef bool (*EasyRPC_ToBytesCallback)(EasyRPCPackage *package, Stream* streamReturn);

EasyRPC_ProcessDataCallback easyRPC_ProcessData;
EasyRPC_ToBytesCallback easyRPC_ToBytes;
/* ---------------------------------------------------------------------------*/
#endif

