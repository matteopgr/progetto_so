#include <stdio.h>
#include "allocator.h"

#define NUM_LVL 9
#define MEM_SIZE 1024 * 1024
#define MIN_BUCKET_SIZE (MEM_SIZE >> (NUM_LVL))
#define BUFFER_SIZE 1 << (NUM_LVL + 1)
#define POOL_SIZE MAX_ALLOCATIONS * (sizeof(AllocationInfo) + sizeof(int))

BuddyAllocator buddy;
Bitmap bitmap;
Allocator alloc;

char pool_buffer[POOL_SIZE];
uint8_t buffer[((BUFFER_SIZE) + 7) / 8] = {0};
char memory[MEM_SIZE];

int main(int argc, char const *argv[])
{
    printf("Creating bitmap\n");
    Bitmap_init(&bitmap, buffer, BUFFER_SIZE);
    printf("Done, num bits: %d\n", bitmap.bits);

    printf("Creating Buddy allocator: %d levels\n", NUM_LVL);
    BuddyAllocator_init(&buddy, NUM_LVL, &bitmap, memory, MIN_BUCKET_SIZE);
    printf("Done, memory size: %d, min bucket size: %d\n", MEM_SIZE, MIN_BUCKET_SIZE);

    printf("Creating allocator\n");
    Allocator_init(&alloc, buddy, pool_buffer);
    printf("Done\n");

    printf("Requesting memory\n");

    void *p1 = Allocator_malloc(&alloc, (MEM_SIZE - 1));
    void *p3 = Allocator_malloc(&alloc, (MEM_SIZE - 1) / 2);
    void *p2 = Allocator_malloc(&alloc, PAGE_SIZE / 3);
    void *p4 = Allocator_malloc(&alloc, PAGE_SIZE / 2);
    void *p5 = Allocator_malloc(&alloc, sizeof(int));

    printf("Num allocations %d\n", alloc.num_allocations);
    printf("Releasing memory\n");

    Allocator_free(&alloc, p1);
    Allocator_free(&alloc, p2);
    Allocator_free(&alloc, p3);
    Allocator_free(&alloc, p4);
    Allocator_free(&alloc, p5);

    printf("Num allocations %d\n", alloc.num_allocations);

    printf("End\n");
}