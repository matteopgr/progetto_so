#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "buddy_allocator.h"

int levelIdx(int idx)
{
    return (int)floor(log2(idx));
}

int first_level_index(int level)
{
    return 1 << level;
}

int idx_offset(int idx)
{
    return idx - first_level_index(levelIdx(idx));
}

int buddy_index(int idx)
{
    return idx % 2 ? idx - 1 : idx + 1;
}

int parent_idx(int idx)
{
    return idx / 2;
}

// cerca il primo blocco libero nel livello
int free_level_idx(BuddyAllocator *alloc, int lvl)
{
    int start = first_level_index(lvl);
    int end = first_level_index(lvl + 1) - 1;

    for (int i = start; i <= end; i++)
    {
        if (!Bitmap_testBit(alloc->bitmap, i))
            return i;
    }
    return -1;
}

void BuddyAllocator_init(BuddyAllocator *alloc, int num_levels, Bitmap *bitmap, char *memory, int min_bucket_size)
{
    alloc->bitmap = bitmap;
    alloc->memory = memory;
    alloc->min_bucket_size = min_bucket_size;
    alloc->num_levels = num_levels;
}

// cerco il primo buddy libero
int BuddyAllocator_getBuddy(BuddyAllocator *alloc, int level)
{
    assert(level >= 0 && level <= alloc->num_levels);

    // cerco un blocco libero su questo livello
    int idx = free_level_idx(alloc, level);
    if (idx != -1)
    {
        Bitmap_setBit(alloc->bitmap, idx);
        return idx;
    }

    // cerco ricorsivamente un blocco libero al livello superiore
    int parent = BuddyAllocator_findBuddy(alloc, level - 1);

    // livello non trovato
    if (parent == -1)
        return -1;

    int left_idx = parent * 2;

    Bitmap_setBit(alloc->bitmap, left_idx);
    return left_idx;
}

void BuddyAllocator_releaseBuddy(BuddyAllocator *alloc, int idx)
{
    if (idx < 0 || idx > (1 << alloc->num_levels) - 1)
    {
        printf("Invalid index\n");
        return;
    }
    Bitmap_clearBit(alloc->bitmap, idx);

    int buddy_idx = buddy_index(idx);
    int parent_idx = idx / 2;

    if (!parent_idx)
        return;

    if (!Bitmap_testBit(alloc->bitmap, buddy_idx))
    {
        printf("Merge %d with %d", idx > buddy_idx ? buddy_idx : idx, idx > buddy_idx ? idx : buddy_idx);
        Bitmap_clearBit(alloc->bitmap, buddy_idx);
        BuddyAllocator_releaseBuddy(alloc, parent_idx);
    }
}

void *BuddyAllocator_malloc(BuddyAllocator *alloc, int size)
{
    int mem_size = 1 << alloc->num_levels * (alloc->min_bucket_size + 4);

    assert(size <= mem_size);

    int level = floor(log(mem_size / size));
    if (level > alloc->num_levels)
        level = alloc->num_levels;
    printf("Requested %d bytes of memory, level %d\n", size, level);

    int idx = BuddyAllocator_getBuddy(alloc, level);
    int offset = idx_offset(idx);
    int buddy_size = mem_size / (1 << level);

    char *address = alloc->memory + buddy_size * offset;

    char **start = address;
    *start = idx;
    return start + 4;
}