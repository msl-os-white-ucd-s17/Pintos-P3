#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

struct user_syscall{
  enum  syscall_index;
  void  * function;
  void  ** args;
  int   arg_count;
};
#endif /* userprog/syscall.h */
