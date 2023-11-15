#include <assert.h>
#include <stdio.h>
#include "bitmap.h"

void Bitmap_init(Bitmap *map, uint8_t *data, int bits)
{
    if (bits < 0)
    {
        printf("Init error: Bit number can't be negative\n");
        return;
    }

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
    if (bit < 0 || bit >= bitmap->bits)
    {
        printf("Set error: Bit out of range\n");
        return;
    }

    int byteIndex = bit / 8;
    int bitOffset = bit % 8;

    bitmap->data[byteIndex] |= (1 << bitOffset);
}

void Bitmap_clearBit(Bitmap *bitmap, int bit)
{
    if (bit < 0 || bit >= bitmap->bits)
    {
        printf("Clear error: Bit out of range\n");
        return;
    }

    int byteIndex = bit / 8;
    int bitOffset = bit % 8;

    bitmap->data[byteIndex] &= ~(1 << bitOffset);
}

int Bitmap_testBit(Bitmap *bitmap, int bit)
{
    if (bit < 0 || bit >= bitmap->bits)
    {
        printf("Test error: Bit out of range\n");
        return -1;
    }

    int byteIndex = bit / 8;
    int bitOffset = bit % 8;

    // Crea una maschera bit per isolare il bit specifico
    uint8_t bitMask = (uint8_t)(1 << bitOffset);

    // Applica la maschera e verifica lo stato del bit
    return (bitmap->data[byteIndex] & bitMask) != 0;
}

int Bitmap_findFreeIndex(Bitmap *bitmap)
{
    for (int i = 0; i < bitmap->bits; i++)
    {
        if (!Bitmap_testBit(bitmap, i))
            return i;
    }
    return -1;
}