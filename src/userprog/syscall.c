#include "userprog/syscall.h"
#include "devices/shutdown.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/malloc.h"
#include "lib/string.h"
#include "list.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "process.h"
#include "syscall.h"

static void syscall_handler(struct intr_frame *f);

static int user_memory_ok(uint8_t *, int size);

static int file_sys_ok();

static uint32_t get_user_int32(void *);

static int get_syscall_args(void *, struct user_syscall *);


void
syscall_init(void) {
    intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static int user_memory_ok(uint8_t *stack_pointer, int size) {
    for (int i = 0; i < size; i++) {
        if (!is_user_vaddr(stack_pointer) || (&stack_pointer) == NULL)
            return false;
        else
            return true;
    }
}

static int get_syscall_args(void *stack_pointer, struct user_syscall *new_syscall) {
    if (!user_memory_ok(stack_pointer, 4)) { return false; }

    new_syscall->arg_count = 0;

    for (int i = 0; (user_memory_ok(stack_pointer, 4) && i < 3); i++) {
        new_syscall->args[i] = get_user_int32(stack_pointer);
        new_syscall->arg_count++;
        stack_pointer += 4;
    }
};

static uint32_t get_user_int32(void *stack_pointer) {
    uint32_t *int32_address;
    uint8_t byte_array[4];

    for (int i = 0; i < 4; i++) {
        byte_array[i] = get_user(stack_pointer);
        stack_pointer++;
    }

    int32_address = *(uint32_t *) byte_array;
}

void
exit_process_by_code(int code) {
    struct list_elem *e;

    for (e = list_begin(&thread_current()->parent->child_processes);
         e != list_end(&thread_current()->parent->child_processes); e = list_next(e)) {
        struct child_parent *c = list_entry(e,
        struct child_parent, elem);
        if (c->tid == thread_current()->tid) {
            c->has_exited = true;
            c->exit_code = code;
        }
    }

    thread_current()->exit_code = code;
    if (thread_current()->parent->waiting_on_thread == thread_current()->tid)
        sema_up(&thread_current()->parent->child_sema);
    thread_exit();
}


static void
syscall_handler(struct intr_frame *f) {
    printf("System call!\n");

    if (!user_memory_ok(f->esp, 1)) {
        printf("Don't gimme that crap Lena!");
        thread_exit();
    }

    void *stack_pointer = f->esp;
    struct user_syscall new_syscall;

    new_syscall.syscall_index = get_user_int32(stack_pointer);
    stack_pointer += 4;

    get_syscall_args(stack_pointer, &new_syscall);
    int sys_call_num = (int) new_syscall.syscall_index;

    switch (sys_call_num) {
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
            printf("AWW FACK");
            break;

    }

    // MODIFIED BY SHAWN JOHNSON
    void halt(void) {
        shutdown_power_off();
    }

    void
    exit(int status) {

        return;
    }

    pid_t
    exec(const char *file) {

        return;
    }

    int
    wait(pid_t pid) {

        return;
    }

    // MODIFIED BY SHAWN JOHNSON
    bool
    create(const char *file, unsigned initial_size) {
        file_lock_acquire();
        bool error = filesys_create(file, initial_size);
        file_lock_release();
        return error;
    }

    // MODIFIED BY SHAWN JOHNSON
    bool
    remove(const char *file) {
        file_lock_acquire();
        bool error = filesys_remove(file);
        file_lock_release();
        return error;
    }

    // MODIFIED BY SHAWN JOHNSON
    int
    open(const char *file) {
        file_lock_acquire();
        struct file *oFile = filesys_open(file);
        if (oFile == NULL) {
            file_lock_release();
            return -1;
        } else {
            int fd = process_affix_file(oFile);
            file_lock_release();
            return fd;
        }
    }

    int
    filesize(int fid) {
        file_lock_acquire();
        struct file *fs = process_get_file(fid);
        int f_size = -1;
        if (fs != NULL) {
            f_size = file_length(fs);
        }
        file_lock_release();
        return f_size;
    }

    int
    read(int fd, void *buffer, unsigned size) {
        return;
    }

    int
    write(int fd, const void *buffer, unsigned size) {
        return;
    }

    void
    seek(int fd, unsigned position) {
        return;
    }

    unsigned
    tell(int fd) {
        return;
    }

    void
    close(int fd) {
        return;
    }

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
