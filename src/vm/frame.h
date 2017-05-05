#ifndef __FRAME_H
#define __FRAME_H

#include "lib/kernel/list.h"

// Both are initialized by frame_table_init
struct lock scan_lock;
struct list frame_table;



/* A physical frame */
struct frame
{
  struct lock lock;   /* Prevent simultaneous access */
   void *base;        /* Kernel virtual base address */
   struct page *page; /* Mapped process page, if any */
    struct list_elem e;
};

// * Frame management funtion declarations *//
// TODO 
void frame_table_init();
void *get_frame(enum palloc_flags flags);
bool insert_frame(struct frame *iframe);

#endif