#include <stdbool.h>
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

// TODO

/* Declaration of a hash_hash_func for the pages hash table */

// TODO

/* Declation fo a hash_less_func for the pages hash table */

// TODO
