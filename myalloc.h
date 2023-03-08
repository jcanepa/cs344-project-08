#include <sys/mman.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ALIGNMENT 16
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x)-1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void *)((char *)(p) + (offset)))

struct block
{
    struct block *next;
    // struct block *previous;
    int size;
    int in_use;
};

void *myalloc(int size);
void mergeblocks(void);
void myfree(void *p);
void print_data(void);