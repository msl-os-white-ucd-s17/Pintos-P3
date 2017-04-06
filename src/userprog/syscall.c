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
syscall_handler (struct intr_frame *f UNUSED) 
{
  //TODO check all the pointers n such: NOT NULL..., below PHYS_BASE, anything else?
  //TODO process which system call is being referenced in syscall-nr.h then call in users/syscall.c
  //TODO create data structure that stores identifier, function, arguments for syscalls
  printf ("system call!\n");
  thread_exit ();
}

//TODO implement all the fucking things dammit
