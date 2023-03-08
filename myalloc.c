#include "myalloc.h"

struct block *head = NULL;

/**
 *  Allocate a given number of bytes & return a pointer to it.
 */
void *myalloc(int size)
{
    int padded_block_size = PADDED_SIZE(sizeof(struct block));

    // initialize the heap
    if (head == NULL)
    {
        head = mmap(
            NULL, 1024, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        head->size = 1024 - padded_block_size;
        head->in_use = 0;
        head->next = NULL;
    }

    struct block *b = head;

    // traverse the linked list
    while (b != NULL)
    {
        bool is_free = !b->in_use;
        bool has_enough_room = (b->size >= size);

        if (is_free && has_enough_room)
        {
            int padded_requested_space = PADDED_SIZE(size);
            int block_to_block_offset = padded_requested_space + padded_block_size;

            // space is big enough to split if current block size is greater than:
            // the requested size + the padded block + a minimum allocation of 16
            int required_space = (block_to_block_offset + ALIGNMENT);
            bool block_splittable = (bool)(b->size >= required_space);

            if (block_splittable)
            {
                // create new block for the remaining unused space
                int new_size = (b->size - block_to_block_offset);
                struct block new = {
                    .next = NULL,
                    .size = new_size,
                    .in_use = 0};

                // "wire" new block node into the linked list
                b[block_to_block_offset] = new;        // place in memory at a relative location
                b->next = &(b[block_to_block_offset]); // set new block as current block's next pointer
                b->size = padded_requested_space;
            }

            b->in_use = 1;

            return PTR_OFFSET(b, padded_block_size);
        }
        b = b->next;
    }
    return NULL;
}

void mergeblocks(void)
{
    return;
}

/**
 * Free allocated memory of a given pointer.
 */
void myfree(void *p)
{
    if (p == NULL)
        return;

    // navigate to header block of given pointer
    struct block *b = (p - sizeof(struct block));
    b->in_use = 0;
}

/**
 * Print allocated memory address space.
 */
void print_data(void)
{
    struct block *b = head;

    if (b == NULL)
    {
        printf("[empty]\n");
        return;
    }

    while (b != NULL)
    {
        printf("[%d,%s]", b->size, b->in_use ? "used" : "free");
        if (b->next != NULL)
            printf(" -> ");
        b = b->next;
    }
    printf("\n");
}

/**
 * Program entrypoint.
 */
int main(void)
{
    // ====== Example 1 ======
    void *p;

    p = myalloc(10);
    print_data();

    myfree(p);
    print_data();

    // ====== Example 2 ======
    void *q;

    p = myalloc(10);
    print_data();
    q = myalloc(20);
    print_data();

    myfree(p);
    print_data();
    myfree(q);
    print_data();

    // ====== Example 3 ======
    p = myalloc(10);
    print_data();
    q = myalloc(20);
    print_data();

    myfree(q);
    print_data();
    myfree(p);
    print_data();

    // ====== Example 4 ======
    void *r, *s;

    p = myalloc(10);
    print_data();
    q = myalloc(20);
    print_data();
    r = myalloc(30);
    print_data();
    s = myalloc(40);
    print_data();

    myfree(q);
    print_data();
    myfree(p);
    print_data();
    myfree(s);
    print_data();
    myfree(r);
    print_data();
}
