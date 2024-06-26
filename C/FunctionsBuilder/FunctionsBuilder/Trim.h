#ifndef _TRIM_H
#define _TRIM_H
/* ---------------------------------------------------------------------------*/
#include <cstring>
using namespace std;
/* ---------------------------------------------------------------------------*/
char* ltrim(char* s) {
    while (isspace(*s)) s++;
    return s;
}
/* ---------------------------------------------------------------------------*/
char* rtrim(char* s) {
    char* back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}
/* ---------------------------------------------------------------------------*/
char* trim(char* s) {
    return rtrim(ltrim(s));
}
/* ---------------------------------------------------------------------------*/
string trim_str(string str) {
    return trim((char *)str.c_str());
}
/* ---------------------------------------------------------------------------*/
string ltrim_str(string str) {
    return ltrim((char*)str.c_str());
}
/* ---------------------------------------------------------------------------*/
string rtrim_str(string str) {
    return rtrim((char*)str.c_str());
}
/* ---------------------------------------------------------------------------*/
#endif