#ifndef __PAGE_H
#define __PAGE_H

#include <stdbool.h>
#include "frame.h"
#include "lib/kernel/hash.h"
#include <stdint.h>

/* Virtual Page */
struct page
{
  /* Immutable members */
   void *addr;              /* User virtual address */
   bool read_only;          /* Read-only Page? */
   struct thread *thread;    /* Owning thread */
   
    /* Accessed only in owning process context. */
    struct hash_elem hash_elem; /* struct thread `pages' hash element. */

    /* Set only in owning process context with frame->lock held.
       Cleared only with scan_lock and frame->lock held. */
    struct frame *frame;        /* Page frame. */

    /* Swap information, protected by frame->lock. */
    block_sector_t sector;       /* Starting sector of swap area, or -1. */
    
    /* Memory-mapped file information, protected by frame->lock. */
    bool private;               /* False to write back to file,
                                   true to write back to swap. */
    struct file *file;          /* File. */
    off_t file_offset;          /* Offset in file. */
    off_t file_bytes;           /* Bytes to read/write, 1...PGSIZE. */
};

/* User page managment function declarations */

void *getPage(enum palloc_flags flags);
void page_in(void *addr);

/* Declaration of a hash_hash_func for the pages hash table */

hash_hash_func compute_hash(const struct hash_elem *e, void *aux);

/* Declation fo a hash_less_func for the pages hash table */

hash_less_func hash_compare(const struct hash_elem *a, const struct hash_elem *b, void *aux);

#endif