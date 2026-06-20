#ifndef BIT_H
#define BIT_H

void bit_set(unsigned char* bitmap, const int page);
void bit_clear(unsigned char* bitmap, const int page);

void bit_write(unsigned char* bitmap, const int page, const bool value);
bool bit_read(unsigned char* bitmap, const int page);

#endif // BIT_H
