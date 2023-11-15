#include "bitmap.h"
#include <assert.h>

void Bitmap_init(Bitmap *map, uint8_t *data, int bits)
{
    map->data = data;
    if (bits % 8)
    {
        map->data_size = bits / 8;
    }
    else
    {
        map->data_size = bits / 8 + 1;
    }
    map->bits = bits;
}

void Bitmap_setBit(Bitmap *bitmap, int bit)
{
    assert(bit < bitmap->bits || "Bit out of range");

    int byteIndex = bit / 8;
    int bitOffset = bit % 8;

    bitmap->data[byteIndex] |= (1 << bitOffset);
}

void clearBit(Bitmap *bitmap, int bit)
{
    assert(bit < bitmap->bits || "Bit out of range");

    int byteIndex = bit / 8;
    int bitOffset = bit % 8;

    bitmap->data[byteIndex] &= ~(1 << bitOffset);
}

int testBit(Bitmap *bitmap, int bit)
{
    assert(bit < bitmap->bits || "Bit out of range");

    int byteIndex = bit / 8;
    int bitOffset = bit % 8;

    int test = bitmap->data[byteIndex] & (1 << bitOffset);
    return test;
}