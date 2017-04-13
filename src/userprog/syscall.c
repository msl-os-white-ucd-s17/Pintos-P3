#include "userprog/syscall.h"

#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "syscall.h"

static void syscall_handler (struct intr_frame *);
static void exit_gracefully();
static int file_sys_ok();
static int user_memory_ok();
static int get_syscall_args(uint8_t*, struct user_syscall);
static int call_syscall(int, uint32_t[]);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static int user_memory_ok(uint8_t * stack_pointer){
  if(is_user_vaddr(stack_pointer) || (&stack_pointer) == NULL)
    return false;
  else
    return true;
}

static int get_syscall_args(uint8_t * stack_pointer, struct user_syscall new_syscall){
  for (int i=0; (!user_memory_ok(stack_pointer) || i == 3);i++){

  }
};

static uint32_t get_user_int32(uint8_t * stack_pointer){
  uint32_t * int32_address;
  uint8_t byte_array[4];

  for(int i = 0; i < 4; i++){
    byte_array[i] = get_user(stack_pointer);
    stack_pointer++;
  }
  int32_address = *(uint32_t *)byte_array;
}

static void
syscall_handler (struct intr_frame *f) {
  printf("system call!\n");

  if (!user_memory_ok(f)){
    printf("Don't gimme that crap");
    thread_exit();
  }

  uint8_t stack_pointer = f->esp;
  struct user_syscall new_syscall;

  new_syscall.syscall_index = get_user_int32(stack_pointer);
  stack_pointer++;

  uint8_t * sys_call_byte = get_user(f->esp);
  int sys_call_num = (int)sys_call_byte;

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
//    case SYS_MMAP :
//      printf("calling SYS_MMAP");
//      break;                   /* Map a file into memory. */
//    case SYS_MUNMAP :
//      printf("calling SYS_MUNMAP");
//      break;                 /* Remove a memory mapping. */
//
//      /* Project 4 only. */
//    case SYS_CHDIR :
//      printf("calling SYS_CHDIR");
//      break;                  /* Change the current directory. */
//    case SYS_MKDIR :
//      printf("calling SYS_MKDIR");
//      break;                  /* Create a directory. */
//    case SYS_READDIR :
//      printf("calling SYS_READDIR");
//      break;                /* Reads a directory entry. */
//    case SYS_ISDIR :
//      printf("calling SYS_ISDIR");
//      break;                  /* Tests if a fd represents a directory. */
//    case SYS_INUMBER:
//      printf("calling SYS_INUMBER");
//      break;

    default:
      break;

  }

//  void
//  halt(void) {
//
//    return;
//  }
//
//  void
//  exit(int status) {
//
//    return;
//  }
//
//  pid_t
//  exec(const char *file) {
//
//    return;
//  }
//
//  int
//  wait(pid_t pid) {
//
//    return;
//  }
//
//  bool
//  create(const char *file, unsigned initial_size) {
//
//    return;
//  }
//
//  bool
//  remove(const char *file) {
//    return;
//  }
//
//  int
//  open(const char *file) {
//    return;
//  }
//
//  int
//  filesize(int fd) {
//    return;
//  }
//
//  int
//  read(int fd, void *buffer, unsigned size) {
//    return;
//  }
//
//  int
//  write(int fd, const void *buffer, unsigned size) {
//    return;
//  }
//
//  void
//  seek(int fd, unsigned position) {
//    return;
//  }
//
//  unsigned
//  tell(int fd) {
//    return;
//  }
//
//  void
//  close(int fd) {
//    return;
//  }
//
//  mapid_t
//  mmap(int fd, void *addr) {
//    return;
//  }
//
//  void
//  munmap(mapid_t mapid) {
//    return;
//  }
//
//  bool
//  chdir(const char *dir) {
//    return;
//  }
//
//  bool
//  mkdir(const char *dir) {
//    return;
//  }
//
//  bool
//  readdir(int fd, char name[READDIR_MAX_LEN + 1]) {
//    return;
//  }
//
//  bool
//  isdir(int fd) {
//    return;
//  }
//
//  int
//  inumber(int fd) {
//    return;
//  }


  //TODO process which system call is being referenced in syscall-nr.h then call corresponding function here, passing *f (parse *f here? probly not, coupling n such
}

//TODO synchronization for file system calls
//TODO implement all the things darnit
//TODO memory cleanups for parent / child / grancchild deaths
//TODO use semaphors for parent child grandchild synchronization