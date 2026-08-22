#include "bitmap.h"
#include "bit.h"

void Bitmap::clear() {
    for (uint64_t i = 0; i < count; i++) {
        bit_clear(bitmap, i);
    }
}

uint64_t Bitmap::find_free_range(uint64_t range, bool& success) {
    success = false;

    uint64_t foundFreeCounter = 0;
    uint64_t firstFreeOfSeries = 0;

    for (uint64_t i = 0; i < count; i++) {
        if (!bit_read(bitmap, i)) {
            if (foundFreeCounter == 0) {
                firstFreeOfSeries = i;
            }
            foundFreeCounter++;
            if (foundFreeCounter == range) {
                success = true;
                return firstFreeOfSeries;
            }
        } else {
            foundFreeCounter = 0;
        }
    }

    return 0;
}
