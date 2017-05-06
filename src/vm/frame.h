#ifndef __FRAME_H
#define __FRAME_H

#include "lib/kernel/list.h"

static struct lock scan_lock;
static struct frame *frame_table;
static int frame_ct;
static int hand;

/* A physical frame */
struct frame
{
  struct lock lock;   /* Prevent simultaneous access */
   void *base;        /* Kernel virtual base address */
   struct page *page; /* Mapped process page, if any */
};

// * Frame management funtion declarations *//
// TODO 
static void frame_table_init();
struct frame *install_frame(enum palloc_flags flags, struct page *page);



#endif