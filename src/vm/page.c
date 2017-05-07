#include "page.h"
#include "../userprog/process.h"
#include "threads/vaddr.h"
#include "lib/kernel/hash.h"
#include "../threads/palloc.h"

/* STACK_MAX (e.g 1M) */

// TODO

/* User page management functions definitions */

// TODO


unsigned compute_hash(const struct hash_elem *e, void *aux UNUSED) {
    struct page *p = hash_entry(e, struct page, hash_elem);
    void *a = pg_round_down(p->addr);
    uintptr_t page_no = pg_no(a);
    return hash_int(page_no);
}


bool hash_compare(const struct hash_elem *a, const struct hash_elem *b, void *aux UNUSED) {
    struct page *p1 = hash_entry(a, struct page, hash_elem);
    struct page *p2 = hash_entry(b, struct page, hash_elem);
    return p1->addr < p2->addr;
}

void *getPage(enum palloc_flags flags) {

}

void page_in(void *addr) {
  void *naddr = pg_round_down(addr);
}

bool add_file(struct file *file, int32_t offset, uint8_t *user_page, uint32_t read_bytes, uint32_t zero_bytes, bool read_only){
  struct page *page = malloc(sizeof(struct page));
  if(!page)
    return false;

  page->file = file;
  page->file_offset = offset;
  page->addr = user_page;
  page->read_bytes = read_bytes;
  page->zero_bytes = zero_bytes;
  page->loaded = false;
  page->source_type = FILE;
  page->read_only = !read_only;

  return(hash_insert(&thread_current()->pages, &page->hash_elem) == NULL);
}

bool add_page_to_stack(void *addr){
  //TODO: check enough available memory
  struct page *stack_page = malloc(sizeof(struct page));
  if (!stack_page){return false;}

  stack_page->addr = pg_round_down(addr);
  stack_page->loaded = true;
  stack_page->read_only = false;
  stack_page->source_type = MEM;

  void * frame = install_frame(PAL_USER, stack_page);
  if (!frame){
    free(stack_page);
    return false;
  }

  if (!install_page(stack_page->addr, stack_page->frame->base, !(stack_page->read_only))){
    free(stack_page);
    //TODO free frame??;
    return false;
  }

  return(hash_insert(&thread_current()->pages, &stack_page->hash_elem)==NULL);

}