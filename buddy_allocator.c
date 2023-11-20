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

void set_parent(Bitmap *map, int idx)
{
    if (!idx)
        return;
    Bitmap_setBit(map, idx);
    if (Bitmap_testBit(map, parent_idx(idx)))
        return;
    set_parent(map, parent_idx(idx));
    printf("Split lvl %d idx: %d into buddies %d - %d\n", levelIdx(idx), idx, idx * 2, idx * 2 + 1);
}

void set_children(Bitmap *map, int idx)
{
    if (idx >= map->bits)
        return;
    Bitmap_setBit(map, idx);
    int left_child = idx * 2;
    int right_child = left_child + 1;
    set_children(map, left_child);
    set_children(map, right_child);
}

void clear_children(Bitmap *map, int idx)
{
    if (idx >= map->bits)
        return;
    Bitmap_clearBit(map, idx);
    int left_child = idx * 2;
    int right_child = left_child + 1;
    clear_children(map, left_child);
    clear_children(map, right_child);
}

// cerca il primo blocco libero nel livello
int free_level_idx(BuddyAllocator *alloc, int lvl)
{
    int start = first_level_index(lvl);
    int end = first_level_index(lvl + 1);

    for (int i = start; i < end; i++)
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
        int parent = parent_idx(idx);
        if (Bitmap_testBit(alloc->bitmap, parent))
        {
            printf("Setting children\n");
            set_children(alloc->bitmap, idx);
            printf("Requested buddy idx: %d, level: %d\n", idx, levelIdx(idx));
            return idx;
        }
        printf("Setting parent\n");
        set_parent(alloc->bitmap, parent);
        printf("Setting children\n");
        set_children(alloc->bitmap, idx);
        printf("Requested buddy idx: %d, level: %d\n", idx, levelIdx(idx));
        return idx;
    }
    return -1;
}

void BuddyAllocator_releaseBuddy(BuddyAllocator *alloc, int idx)
{
    if (idx < 0 || idx > (1 << (alloc->num_levels + 1)))
    {
        printf("Error Release: Invalid index\n");
        return;
    }
    clear_children(alloc->bitmap, idx);

    int buddy_idx = buddy_index(idx);
    int parent_idx = idx / 2;

    if (!parent_idx)
        return;

    if (!Bitmap_testBit(alloc->bitmap, buddy_idx))
    {
        int mem_size = (1 << alloc->num_levels) * (alloc->min_bucket_size + 4);
        int level = levelIdx(idx);
        int buddy_size = mem_size / (1 << level);
        printf("Merge lvl %d buddies %d - %d, size: %d into parent %d\n", levelIdx(idx), idx > buddy_idx ? buddy_idx : idx, idx > buddy_idx ? idx : buddy_idx, buddy_size, parent_idx);
        BuddyAllocator_releaseBuddy(alloc, parent_idx);
    }
}

void *BuddyAllocator_malloc(BuddyAllocator *alloc, int size)
{
    int mem_size = (1 << alloc->num_levels) * (alloc->min_bucket_size + 4);
    int max_size = (1 << alloc->num_levels) * (alloc->min_bucket_size);

    if (size > max_size)
    {
        printf("Error Malloc: Requested %d bytes of memory, Not enough memory\n", max_size);
        return 0;
    }

    int level = (int)floor(log2(mem_size / size));
    if (level > alloc->num_levels)
        level = alloc->num_levels;
    printf("Requested %d bytes of memory, level %d\n", size, level);

    int idx = BuddyAllocator_getBuddy(alloc, level);
    if (idx == -1)
    {
        printf("Error Malloc: Not enough memory\n");
        return 0;
    }
    int offset = idx_offset(idx);
    int buddy_size = mem_size / (1 << level);

    char *address = alloc->memory + buddy_size * offset;

    int *start = (int *)address;
    *start = idx;
    printf("Done malloc start: %p, end: %p\n", start + 1, (start + 1 + buddy_size / 4) - 1);
    return start + 1;
}

void BuddyAllocator_free(BuddyAllocator *alloc, void *mem)
{
    if (!mem)
    {
        printf("Error Free: Memory not allocated\n");
        return;
    }
    printf("Free %p\n", mem);
    int *i = (int *)mem - 1;
    int idx = *i;
    int mem_size = (1 << alloc->num_levels) * (alloc->min_bucket_size + 4);
    int level = levelIdx(idx);
    int buddy_size = mem_size / (1 << level);
    printf("Free idx %d,level: %d, size: %d\n", idx, level, buddy_size);
    if (idx < 0 || idx > (1 << (alloc->num_levels + 1)))
    {
        printf("Error: Free %p -> Invalid index\n", mem);
        return;
    }
    BuddyAllocator_releaseBuddy(alloc, idx);
    printf("Done free\n");
}