#include "frame.h"
#include "page.h"
#include "../lib/debug.h"
#include "../threads/palloc.h"
#include "../threads/malloc.h"
#include "../threads/loader.h"


/* Global frame_ct, the number of physical frames represented */
// TODO

/* Global hand, in a reference to clock hand, for the page eviction algo. */
// TODO

/* Frame Mangament function definitions */
// TODO

/* Called once on startup. */
static void frame_table_init() {
    lock_init(&scan_lock);
    frame_ct = 0;
    frame_table = malloc(sizeof(struct frame) * init_ram_pages);
    for (void *p = palloc_get_page(PAL_USER); p != NULL; p = palloc_get_page(PAL_USER), frame_ct++) {
        struct frame *nframe = &frame_table[frame_ct];
        lock_init(&nframe->lock);
        nframe->base = p;
        nframe->page = NULL;
    }

}

/* 'Allocates' a frame from our array of frames. */
struct frame *install_frame(enum palloc_flags flags, struct page *page) {
    // Assert flags == PAL_USER?
    ASSERT(page != NULL);
    lock_acquire(&scan_lock);

    struct frame *aframe = NULL;


    for (int i = 0; i < frame_ct; i++) {
        aframe = &frame_table[i];
        if (!lock_try_acquire(&aframe->lock)) {
            continue;
        }
        else {
            ASSERT(aframe->page == NULL);
            aframe->page = page;
            page->frame = aframe;
            lock_release(&scan_lock);
            return aframe;
        }

    }

    // Eviction


    lock_release(&scan_lock);
}

bool frame_lock(struct frame *fr) {

}

bool frame_unlock(struct frame *fr) {

}