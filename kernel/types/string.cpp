#include "string.h"

int strlen(const string str) {
    int i = 0;
    while (str[i] != 0) i++;
    return i;
}

bool strcmp(const string a, const string b) {
    int a_len = strlen(a);
    int b_len = strlen(b);
    
    if (a_len != b_len) { return false; }

    for (int i = 0; i < a_len; ++i) {
        if (a[i] != b[i]) { return false; }
    }

    return true;
}

// void strcpy(string dest, const string src) { // TODO}
// void memset(void* ptr, int value, int size) {// TODO}
