#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct user_program{
  const char *file_name;
  char **args;
  int arg_count;

  //dont think a list is necessary, only grandchildren required (check for grandness with parent.parent = NULL)
  void *parent;
  void *child;

  enum child_status {
    BEGIN,
    BUSY,
    FIN
  };
};

#endif /* userprog/process.h */
