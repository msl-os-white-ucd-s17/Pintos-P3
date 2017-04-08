#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) {
  printf("system call!\n");

  //TODO check all the pointers n such: NOT NULL..., anything else?
  //TODO need to check pointer to each arg as well?
  if (!is_user_vaddr(f.esp)){
    thread_exit();
  }
  //TODO process which system call is being referenced in syscall-nr.h then call corresponding function here, passing *f (parse *f here? probly not, coupling n such


}

//TODO implement all the things darnit
//TODO memory cleanups for parent / child / grancchild deaths
//TODO use semaphors for parent child grandchild synchronization