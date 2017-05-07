#ifndef __PAGE_H
#define __PAGE_H
#endif
#include <stdbool.h>
#include "frame.h"
#include <stdint.h>
#include "filesys/off_t.h"
#include "devices/block.h"
#include "lib/kernel/hash.h"

typedef enum source_type{FILE, SWAP, MMAP, MEM};
/* Virtual Page */
struct page
{
  /* Immutable members */
  void * addr;              /* User virtual address */
  bool read_only;          /* Read-only Page? */
  struct thread *thread;    /* Owning thread */
  uint32_t read_bytes;
  uint32_t zero_bytes;
  enum source_type source_type;
  bool loaded;

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

bool add_file(struct file *file, int32_t offset, uint8_t *user_page, uint32_t read_bytes, uint32_t zero_bytes, bool read_only);
//void *getPage(enum palloc_flags);
//void page_in(void*);
//
///* Declaration of a hash_hash_func for the pages hash table */
//unsigned compute_hash(const struct hash_elem *e, void *aux);
//
///* Declation fo a hash_less_func for the pages hash table */
//bool hash_compare(const struct hash_elem *a, const struct hash_elem *b, void *aux);

