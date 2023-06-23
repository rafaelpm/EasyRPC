#ifndef _SAVE_CONTENT_H
#define _SAVE_CONTENT_H
/* ---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <fstream>

using namespace std;

class SaveContent {
private:

public:
	string contentRead;
	bool save(string name, string content);
	bool open(string name);
};
/* ---------------------------------------------------------------------------*/
bool SaveContent::open(string name) {
	FILE* file = fopen(name.c_str(), "rb");  

    if (file == nullptr) {
        printf("File not found: %s\n", name.c_str());
        return false;
    }

    fseek(file, 0, SEEK_END);  
    long size = ftell(file); 
    rewind(file);  

    char* buffer = new char[size + 1];  
    size_t readBytes = fread(buffer, sizeof(char), size, file);  

    if (readBytes != size) {
        printf("Error to read file: %s\n", name.c_str());
        delete[] buffer;
        fclose(file);
        return false;
    }

	fclose(file);

    buffer[readBytes] = '\0';  

	contentRead = buffer;

	//printf("Content:\n%s\n",buffer);

	return true;
}
/* ---------------------------------------------------------------------------*/
bool SaveContent::save(string name, string content) {
	FILE* file;
	file = fopen(name.c_str(), "w");
	if (file == NULL) {
		return false;
	}
	fwrite(content.c_str(), 1, content.size(), file);

	fclose(file);
	return true;
}
/* ---------------------------------------------------------------------------*/
#endif