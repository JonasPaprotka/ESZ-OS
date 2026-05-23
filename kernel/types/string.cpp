#include "string.h"

int strlen(const string str) {
    int i = 0;
    while (str[i] != 0) i++;
    return i;
}

// int strcmp(const string a, const string b) { // TODO return 0; }
// void strcpy(string dest, const string src) { // TODO}
// void memset(void* ptr, int value, int size) {// TODO}
