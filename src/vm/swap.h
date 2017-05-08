#ifndef __SWAP_H
#define __SWAP_H

#include "vm/page.h"
#include <stdbool.h>
/* Forward declaration of struct page */
struct page;

/* Swap partition management function declarations */
void init_swap();
void swap_in(struct page *);
bool swap_out(struct page *);


#endif