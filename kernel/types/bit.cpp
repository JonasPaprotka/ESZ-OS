// File Updated with AI / Assisted by AI

void bit_set(unsigned char* bitmap, int page) {
    bitmap[page / 8] |= (1 << (page % 8));
}

void bit_clear(unsigned char* bitmap, int page) {
    bitmap[page / 8] &= ~(1 << (page % 8));
}

void bit_write(unsigned char* bitmap, int page, bool value) {
    if (value) bit_set(bitmap, page);
    else bit_clear(bitmap, page);
}

bool bit_read(unsigned char* bitmap, int page) {
    return (bitmap[page / 8] >> (page % 8)) & 1;
}
