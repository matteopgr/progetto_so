#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "allocator.h"

AllocationInfo *findAllocationFromAddress(Allocator *alloc, char *address)
{
    printf("Searching allocation info for %p\n", address);
    ListItem *aux = alloc->allocations.first;

    while (aux)
    {
        AllocationInfo *info = (AllocationInfo *)aux;
        if (info->mem == address)
        {
            printf("Found allocation info for %p\n", info->mem);
            return info;
        }
        aux = aux->next;
    }

    // L'indirizzo non è stato trovato
    return 0;
}

AllocationInfo *createAllocationInfo(Allocator *alloc, char *mem, int size)
{
    AllocationInfo *info = (AllocationInfo *)PoolAllocator_getBlock(&alloc->pool);
    info->mem = mem;
    info->size = size;
    List_pushFront(&alloc->allocations, (ListItem *)info);
    alloc->num_allocations++;
    printf("Creating allocation info for %p - size %d\n", info->mem, info->size);
    return info;
}

void destroyAllocationInfo(Allocator *alloc, AllocationInfo *info)
{
    List_detach(&alloc->allocations, (ListItem *)info);
    printf("Destroying allocation info for %p - size %d\n", info->mem, info->size);
    PoolAllocatorResult res = PoolAllocator_releaseBlock(&alloc->pool, info);
    alloc->num_allocations--;
    assert(res == Success);
}

void Allocator_init(Allocator *alloc, BuddyAllocator buddy, char *buffer)
{
    int pool_size = MAX_ALLOCATIONS * (sizeof(AllocationInfo) + sizeof(int));

    alloc->buddy = buddy;
    alloc->num_allocations = 0;
    List_init(&alloc->allocations);
    PoolAllocator_init(&alloc->pool, sizeof(AllocationInfo), MAX_ALLOCATIONS, buffer, pool_size);
}

void *Allocator_malloc(Allocator *alloc, int size)
{
    printf("Requested %d bytes of memory\n", size);
    if (size < 0)
    {
        printf("Error Malloc: size must be positive\n");
        return 0;
    }
    if (size < PAGE_SIZE / 4)
    {
        printf("Size < page size / 4 - Using buddy allocator\n");
        char *mem = (char *)BuddyAllocator_malloc(&alloc->buddy, size);
        if (!mem)
        {
            printf("Error Malloc buddy\n");
            return 0;
        }
        printf("Malloc success buddy\n");
        alloc->num_allocations++;
        return mem;
    }
    else
    {
        printf("Size >= page size / 4 - Using mmap\n");
        char *mem = (char *)mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mem == MAP_FAILED)
        {
            printf("Error Malloc mmap: %s\n", strerror(errno));
            return 0;
        }
        AllocationInfo *info = createAllocationInfo(alloc, mem, size);
        printf("Malloc success mmap - start %p end %p\n", info->mem, info->mem + size - 1);
        return mem;
    }
}

void Allocator_free(Allocator *alloc, void *mem)
{
    // controllo se l'indirizzo di mem è all'interno del buddy
    char *p = mem;
    int size = (1 << alloc->buddy.num_levels) * (alloc->buddy.min_bucket_size + 4);
    if (p >= alloc->buddy.memory && p <= alloc->buddy.memory + size)
    {
        printf("Memory address is inside buddy allocator\n");
        BuddyAllocator_free(&alloc->buddy, mem);
        alloc->num_allocations--;
    }
    else
    {
        // Cerco l'indirizzo all'interno della lista delle allocazioni
        printf("Memory address is outside buddy allocator - unmapping memory\n");
        AllocationInfo *info = findAllocationFromAddress(alloc, mem);
        if (!info)
        {
            printf("Error Free mmap: can't find allocation info\n");
            return;
        }
        destroyAllocationInfo(alloc, info);
        int res = munmap(mem, info->size);
        if (res == -1)
        {
            printf("Error Free mmap: %s\n", strerror(errno));
            return;
        }
        printf("Done unmapping\n");
    }
}