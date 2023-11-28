#pragma once

#include <sys/mman.h>
#include <errno.h>
#include "linked_list.h"
#include "pool_allocator.h"
#include "buddy_allocator.h"

#define PAGE_SIZE 4 * 1024 * 1024 // 4 Mb
#define MAX_ALLOCATIONS 1000

typedef struct Allocator
{
    BuddyAllocator buddy; // Buddy allocator needed for small allocations
    PoolAllocator pool;   // Pool allocator for allocations info
    int num_allocations;  // Number of allocations
    ListHead allocations; // List of allocations using mmap
} Allocator;

typedef struct AllocationInfo
{
    ListItem item;
    char *mem;
    int size;
} AllocationInfo;

void Allocator_init(Allocator *alloc, BuddyAllocator buddy, char *buffer);

void *Allocator_malloc(Allocator *alloc, int size);

void Allocator_free(Allocator *alloc, void *mem);