#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

#define NULL_BYTE ((char) 0) //ADDED BY SHAWN JOHNSON

struct process_file {
    struct file* file_ptr;
    int fid;
    struct list_elem elem;
};

typedef struct {
    const char *file_name;
    char *args[24];
    int arg_count;

    //dont think a list is necessary, only grandchildren required (check for grandness with parent.parent = NULL)
    void *parent;
    void *child;

    enum child_status {
        BEGIN,
        BUSY,
        FIN
    };
} user_program;

tid_t process_execute(const char *file_name);
int process_wait(tid_t);
void process_exit(void);
void process_activate(void);
static bool install_page(void *upage, void *kpage, bool writable);

//ADDED BY SHAWN JOHNSON
int process_affix_file(struct file *f);
struct file *process_get_file(int fid);

#endif /* userprog/process.h */
