#ifndef _SAVE_CONTENT_H
#define _SAVE_CONTENT_H
/* ---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

class SaveContent {
public:
	bool save(string name, string content);
};
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