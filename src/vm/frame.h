

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