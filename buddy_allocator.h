#pragma once

#include "bitmap.h"

typedef struct BuddyAllocator
{
    int num_levels;
    char *memory;        // the memory area to be managed
    int min_bucket_size; // the minimum page of RAM that can be returned

    // Aggiunta della bitmap per il buddy allocator
    Bitmap *bitmap;
} BuddyAllocator;

void BuddyAllocator_init(BuddyAllocator *alloc, int num_levels, Bitmap *bitmap, char *memory, int min_bucket_size);

int BuddyAllocator_getBuddy(BuddyAllocator *alloc, int level);

void BuddyAllocator_releaseBuddy(BuddyAllocator *alloc, int idx);

void *BuddyAllocator_malloc(BuddyAllocator *alloc, int size);

void BuddyAllocator_free(BuddyAllocator *alloc, void *mem);