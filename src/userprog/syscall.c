#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) {
  printf("system call!\n");

  //TODO check all the pointers n such: NOT NULL..., anything else?
  //TODO need to check pointer to each arg as well?
  if (!is_user_vaddr(f->esp)){
    printf("Don't gimme that crap");
    thread_exit();
  }

  uint8_t * sys_call_num = get_user(f->esp);
  
  switch(sys_call_num){
    case SYS_HALT :
      printf("calling SYS_HALT");
      break;                   /* Halt the operating system. */
    case SYS_EXIT :
      printf("calling SYS_EXIT");
      break;                   /* Terminate this process. */
    case SYS_EXEC :
      printf("calling SYS_EXEC");
      break;                   /* Start another process. */
    case SYS_WAIT :
      printf("calling SYS_WAIT");
      break;                   /* Wait for a child process to die. */
    case SYS_CREATE :
      printf("calling SYS_CREATE");
      break;                 /* Create a file. */
    case SYS_REMOVE :
      printf("calling SYS_REMOVE");
      break;                 /* Delete a file. */
    case SYS_OPEN :
      printf("calling SYS_OPEN");
      break;                   /* Open a file. */
    case SYS_FILESIZE :
      printf("calling SYS_FILESIZE");
      break;               /* Obtain a file's size. */
    case SYS_READ :
      printf("calling SYS_READ");
      break;                   /* Read from a file. */
    case SYS_WRITE :
      printf("calling SYS_WRITE");
      break;                  /* Write to a file. */
    case SYS_SEEK :
      printf("calling SYS_SEEK");
      break;                   /* Change position in a file. */
    case SYS_TELL :
      printf("calling SYS_TELL");
      break;                   /* Report current position in a file. */
    case SYS_CLOSE :
      printf("calling SYS_CLOSE");
      break;                  /* Close a file. */

      /* Project 3 and optionally project 4. */
    case SYS_MMAP :
      printf("calling SYS_MMAP");
      break;                   /* Map a file into memory. */
    case SYS_MUNMAP :
      printf("calling SYS_MUNMAP");
      break;                 /* Remove a memory mapping. */

      /* Project 4 only. */
    case SYS_CHDIR :
      printf("calling SYS_CHDIR");
      break;                  /* Change the current directory. */
    case SYS_MKDIR :
      printf("calling SYS_MKDIR");
      break;                  /* Create a directory. */
    case SYS_READDIR :
      printf("calling SYS_READDIR");
      break;                /* Reads a directory entry. */
    case SYS_ISDIR :
      printf("calling SYS_ISDIR");
      break;                  /* Tests if a fd represents a directory. */
    case SYS_INUMBER:
      printf("calling SYS_INUMBER");
      break;
    default:
      break;
      
    
  }



  //TODO process which system call is being referenced in syscall-nr.h then call corresponding function here, passing *f (parse *f here? probly not, coupling n such


}

//TODO implement all the things darnit
//TODO memory cleanups for parent / child / grancchild deaths
//TODO use semaphors for parent child grandchild synchronization