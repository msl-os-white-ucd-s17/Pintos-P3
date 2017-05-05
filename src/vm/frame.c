#include "frame.h"
#include "lib/debug.h"
#include "threads/palloc.h"
#include "threads/malloc.h"


/* Global frame_ct, the number of physical frames represented */
// TODO

/* Global hand, in a reference to clock hand, for the page eviction algo. */
// TODO

/* Frame Mangament function definitions */
// TODO

void frame_table_init() {
    lock_init(&scan_lock);
    list_init(&frame_table);
}

void *get_frame(enum palloc_flags flags) {
    void *f = palloc_get_page(flags);
    if (f == NULL) {
        PANIC("FUT@)*YRF*D^*T!!!@!");
    }
    else {
        lock_acquire(&scan_lock);
        struct frame *fe = malloc(sizeof(struct frame));
        ASSERT(fe != NULL);
        fe->base = f;
        lock_init(fe->lock);
        // Where does struct *page come from?
        // We don't create it here..?
        list_push_back(frame_table, fe->e);
        lock_release(&scan_lock);
    }
}
