#include <stdio.h>
#include "buddy_allocator.h"

#define NUM_LVL 9
#define MEM_SIZE 1024 * 1024
#define MIN_BUCKET_SIZE (MEM_SIZE >> (NUM_LVL))
#define BUFFER_SIZE 1 << (NUM_LVL + 1)

Bitmap bitmap;
BuddyAllocator alloc;
uint8_t buffer[((BUFFER_SIZE) + 7) / 8] = {0};
char memory[MEM_SIZE];

int main(int argc, char const *argv[])
{
    printf("Creating bitmap\n");
    Bitmap_init(&bitmap, buffer, BUFFER_SIZE);
    printf("Done, num bits: %d\n", bitmap.bits);
    printf("Creating Buddy allocator: %d levels\n", NUM_LVL);
    BuddyAllocator_init(&alloc, NUM_LVL, &bitmap, memory, MIN_BUCKET_SIZE);
    printf("Done, memory size: %d, min bucket size: %d\n", MEM_SIZE, MIN_BUCKET_SIZE);

    printf("Requesting memory\n");

    void *p0 = BuddyAllocator_malloc(&alloc, MEM_SIZE + 1);
    void *p1 = BuddyAllocator_malloc(&alloc, 1000);
    void *p2 = BuddyAllocator_malloc(&alloc, MIN_BUCKET_SIZE);
    void *p3 = BuddyAllocator_malloc(&alloc, 2 * MIN_BUCKET_SIZE);
    void *p4 = BuddyAllocator_malloc(&alloc, MEM_SIZE / 2);
    void *p5 = BuddyAllocator_malloc(&alloc, MEM_SIZE / 2);
    void *p6 = BuddyAllocator_malloc(&alloc, MEM_SIZE / 4);
    void *p7 = BuddyAllocator_malloc(&alloc, MEM_SIZE / 4);
    void *p8 = BuddyAllocator_malloc(&alloc, 1);

    /*     for (int i = 0; i < bitmap.bits; i++)
        {
            if (Bitmap_testBit(&bitmap, i))
                printf("Bit %d taken\n", i);
        } */

    printf("Releasing memory\n");
    BuddyAllocator_free(&alloc, p1);
    BuddyAllocator_free(&alloc, p2);
    BuddyAllocator_free(&alloc, p3);
    BuddyAllocator_free(&alloc, p4);
    BuddyAllocator_free(&alloc, p5);
    BuddyAllocator_free(&alloc, p6);
    BuddyAllocator_free(&alloc, p7);
    BuddyAllocator_free(&alloc, p8);

    printf("End\n");
}