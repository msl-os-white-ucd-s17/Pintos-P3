#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "user/syscall.h"
#include "../lib/stdint.h"

void syscall_init (void);

struct user_syscall{
  uint32_t syscall_index;
  void  * function;
  uint32_t args[3];
  int   arg_count;
};

//semaphor for determining whether
enum using_file_system{
  FILE_SYS_IN_USE = true,
  FILE_SYS_FREE = false
};

enum using_file_system is_in_use;
#endif /* userprog/syscall.h */

void halt (void);
void exit (int status);
pid_t exec (const char *file);
int wait (pid_t pid);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);
/*
 * Not used in Project 2
mapid_t mmap (int fd, void *addr);
void munmap (mapid_t mapid);
bool chdir (const char *dir);
bool mkdir (const char *dir);
bool readdir (int fd, char name[READDIR_MAX_LEN + 1]);
bool isdir (int fd);
int inumber (int fd);
 */
