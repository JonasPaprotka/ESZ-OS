#ifndef BIT_H
#define BIT_H

void bit_set(unsigned char* bitmap, int page);
void bit_clear(unsigned char* bitmap, int page);
void bit_write(unsigned char* bitmap, int page, bool value);

bool bit_read(unsigned char* bitmap, int page);

#endif
