#include "page.h"
#include "threads/vaddr.h"

/* STACK_MAX (e.g 1M) */

// TODO

/* User page management functions definitions */

// TODO

void *getPage(enum palloc_flags flags) {

}

void page_in(void *addr) {
    void *naddr = pg_round_down(addr);

}

hash_hash_func compute_hash(const struct hash_elem *e, void *aux) {
    struct page *p = hash_entry(e, struct page, hash_elem);
    void *a = pg_round_down(p->addr);
    uintptr_t page_no = pg_no(a);
    return hash_int(page_no);
}


hash_less_func hash_compare(const struct hash_elem *a, const struct hash_elem *b, void *aux) {
    struct page *p1 = hash_entry(a, struct page, hash_elem);
    struct page *p2 = hash_entry(b, struct page, hash_elem);
    return p1->addr < p2->addr;
}
