#include <stdio.h>
#include "pool_allocator.h"

#define NUM_ITEMS 16
#define ITEM_SIZE 4096
#define BUFFER_SIZE NUM_ITEMS *(ITEM_SIZE * sizeof(int))

char buffer[BUFFER_SIZE];
PoolAllocator alloc;

int main(int argc, char const *argv[])
{
    printf("Starting!\n");
    printf("Initialziong poll allocator ");
    PoolAllocatorResult res = PoolAllocator_init(&alloc, ITEM_SIZE, NUM_ITEMS, buffer, BUFFER_SIZE);
    printf("result: %s\n", PoolAllocator_strerror(res));

    // allocation of bigger memory than expected
    printf("Allocating memory\n");
    void *test[NUM_ITEMS + 10];
    for (int i = 0; i < NUM_ITEMS + 10; i++)
    {
        test[i] = PoolAllocator_getBlock(&alloc);
        printf("Allocation %d, block %p, free blocks %d\n", i, test[i], alloc.size);
    }

    // releasing memory
    printf("\nReleasing memory\n");
    for (int i = 0; i < NUM_ITEMS + 10; i++)
    {
        if (test[i])
        {
            printf("Releasing block %d, block %p, free blocks %d ", i, test[i], alloc.size);
            res = PoolAllocator_releaseBlock(&alloc, test[i]);
            printf("Result: %s\n", PoolAllocator_strerror(res));
        }
    }

    // releasing memory again
    printf("\nReleasing memory again\n");
    for (int i = 0; i < NUM_ITEMS + 10; i++)
    {
        if (test[i])
        {
            printf("Releasing block %d, block %p, free blocks %d ", i, test[i], alloc.size);
            res = PoolAllocator_releaseBlock(&alloc, test[i]);
            printf("Result: %s\n", PoolAllocator_strerror(res));
        }
    }

    printf("\nAllocating memory and relaesing in reverse order\n");
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        test[i] = PoolAllocator_getBlock(&alloc);
        printf("Allocation %d, block %p, free blocks %d\n", i, test[i], alloc.size);
    }

    for (int i = NUM_ITEMS - 1; i >= 0; i--)
    {
        if (test[i])
        {
            printf("Releasing block %d, block %p, free blocks %d ", i, test[i], alloc.size);
            res = PoolAllocator_releaseBlock(&alloc, test[i]);
            printf("Result: %s\n", PoolAllocator_strerror(res));
        }
    }

    printf("\nAllocating memory and relaesing odd positions first, then even in reverse order\n");
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        test[i] = PoolAllocator_getBlock(&alloc);
        printf("Allocation %d, block %p, free blocks %d\n", i, test[i], alloc.size);
    }

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        if (test[i] && (i % 2))
        {
            printf("Releasing block %d, block %p, free blocks %d ", i, test[i], alloc.size);
            res = PoolAllocator_releaseBlock(&alloc, test[i]);
            printf("Result: %s\n", PoolAllocator_strerror(res));
        }
    }

    for (int i = NUM_ITEMS - 1; i >= 0; i--)
    {
        if (test[i] && !(i % 2))
        {
            printf("Releasing block %d, block %p, free blocks %d ", i, test[i], alloc.size);
            res = PoolAllocator_releaseBlock(&alloc, test[i]);
            printf("Result: %s\n", PoolAllocator_strerror(res));
        }
    }
}