#include <stdio.h>
#include "bitmap.h"

#define N_BITS 10

Bitmap bitmap;
uint8_t data[(N_BITS + 7) / 8] = {0};

int main(int argc, char const *argv[])
{
    printf("Start, initializing %d bits bitmap\n", N_BITS);
    Bitmap_init(&bitmap, data, N_BITS);
    printf("Initialized bitmap with %d bits, with a buffer of %d bytes, starting at %p\n", bitmap.bits, bitmap.data_size, bitmap.data);

    printf("Setting bit #3\n");
    Bitmap_setBit(&bitmap, 3);
    printf("Bit #3 set %d\n", Bitmap_testBit(&bitmap, 3));

    printf("Setting bit #3 another time\n");
    Bitmap_setBit(&bitmap, 3);
    printf("Bit #3 set %d\n", Bitmap_testBit(&bitmap, 3));

    printf("Setting bit #1\n");
    Bitmap_setBit(&bitmap, 1);
    printf("Bit #1 set %d\n", Bitmap_testBit(&bitmap, 1));

    printf("Setting bit #11\n");
    Bitmap_setBit(&bitmap, 11);
    printf("Bit #11 set %d\n", Bitmap_testBit(&bitmap, 11));

    printf("Clearing bit #3\n");
    Bitmap_clearBit(&bitmap, 3);
    printf("Bit #3 clear %d\n", Bitmap_testBit(&bitmap, 3));

    printf("Clearing bit #3 another time\n");
    Bitmap_clearBit(&bitmap, 3);
    printf("Bit #3 clear %d\n", Bitmap_testBit(&bitmap, 3));

    int index = Bitmap_findFreeIndex(&bitmap);
    if (index >= 0)
        printf("Finding first free index: %d\n", index);
    else
        printf("No free index found\n");

    printf("Setting bit #0\n");
    Bitmap_setBit(&bitmap, 0);
    printf("Bit #0 set %d\n", Bitmap_testBit(&bitmap, 0));

    index = Bitmap_findFreeIndex(&bitmap);
    if (index >= 0)
        printf("Finding first free index: %d\n", index);
    else
        printf("No free index found\n");
}
