


/* A physical frame */
struct frame
{
  struct lock lock;   /* Prevent simultaneous access */
   void *base;        /* Kernel virtual base address */
   struct page *page; /* Mapped process page, if any */
};

/ * Frame management funtion declarations */
// TODO 
