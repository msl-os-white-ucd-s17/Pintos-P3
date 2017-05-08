#include "vm/swap.h"
#include "devices/block.h"
#include "lib/kernel/bitmap.h"
#include "threads/synch.h"
#include "threads/vaddr.h"


/* Global swap device (pointer to a struct block) */
static struct block *SWAP;

/* Global bitmap to keep track of open slots (pointer to struct bitmap) */
static struct bitmap *SWAP_TABLE;

/* Global lock to protect swap bitmap */
struct lock swap_lock;

/* PAGE_SECTORS, the number of sectors per page */
/* 8 sectors per page */
static unsigned PAGE_SECTORS = PGSIZE / BLOCK_SECTOR_SIZE;

/* Swap partition management function definitions */

void init_swap() {
    SWAP = block_get_role(BLOCK_SWAP);
    ASSERT (SWAP != NULL);
    lock_init(&swap_lock);
    SWAP_TABLE = bitmap_create(SWAP->size / PAGE_SECTORS);
    ASSERT (SWAP_TABLE != NULL);
}

void swap_in(struct page *pin) {

}

/* Probably should make a page/frame evict function that calls this one.*/
bool swap_out(struct page *pout) {
    ASSERT (pout != NULL);

    lock_acquire(&swap_lock);
    /* Returns index of the beginning of a continuous region that is at least 8 sectors long */
    osec = bitmap_scan_and_flip(SWAP_TABLE, 0, 1, 1);
    lock_release(&swap_lock);
    if (osec == BITMAP_ERROR) {
        return false;
    }

    pout->sector = (block_sector_t) osec;

    /* Actual writing of frame to disk. */
    for (unsigned int i = 0; i < PAGE_SECTORS; ++i) {
        block_sector_t sector_idx = pout->sector + i;
        void *frame_loc = p->frame->base + BLOCK_SECTOR_SIZE * i;
        block_write(SWAP, sector_idx, frame_loc);
    }
    return true;
}