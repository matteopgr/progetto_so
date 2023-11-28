#include "pool_allocator.h"

const char *PoolAllocator_strerrors[] = {"Success", "NotEnoughMemory", "UnalignedFree", "OutOfRange", "DoubleFree", 0};

const char *PoolAllocator_strerror(PoolAllocatorResult result)
{
    return PoolAllocator_strerrors[-result];
}

PoolAllocatorResult PoolAllocator_init(PoolAllocator *allocator, int item_size, int num_items, char *memory_block, int memory_size)
{
    int mem = num_items * (item_size + sizeof(int));
    if (mem > memory_size)
    {
        return NotEnoughMemory;
    }

    allocator->item_size = item_size;
    allocator->size = num_items;
    allocator->size_max = num_items;
    allocator->buffer_size = num_items * item_size;
    allocator->buffer = memory_block;
    allocator->free_list = (int *)(memory_block + allocator->buffer_size);

    for (int i = 0; i < num_items - 1; i++)
    {
        allocator->free_list[i] = i + 1;
    }
    allocator->free_list[num_items - 1] = -1;
    allocator->first_idx = 0;

    return Success;
}

void *PoolAllocator_getBlock(PoolAllocator *allocator)
{
    if (allocator->first_idx == -1)
        return 0;

    // detatch block from the head

    int block_idx = allocator->first_idx;
    allocator->first_idx = allocator->free_list[block_idx];
    allocator->size--;

    allocator->free_list[block_idx] = -2;

    char *block_address = allocator->buffer + (block_idx * allocator->item_size);
    return block_address;
}

PoolAllocatorResult PoolAllocator_releaseBlock(PoolAllocator *allocator, void *block)
{
    char *block_rel = (char *)block;
    int offset = block_rel - allocator->buffer;

    // block is not aligned
    if (offset % allocator->item_size)
        return UnalignedFree;

    int idx = offset / allocator->item_size;

    // block index is out of range
    if (idx > allocator->size_max)
        return OutOfRange;

    // block was already released
    if (allocator->free_list[idx] != -2)
        return DoubleFree;

    // insert the block at the head
    int next_idx = allocator->first_idx;
    allocator->first_idx = idx;
    allocator->free_list[idx] = next_idx;
    allocator->size++;

    return Success;
}