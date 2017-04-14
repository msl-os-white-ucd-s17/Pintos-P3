#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/malloc.h"
#include "lib/string.h"
#include "list.h"
#include "process.h"

static void syscall_handler (struct intr_frame *);

void* verify_address(const void*);
struct process_file* get_file(struct list* files, int fid);
void close_all_files(struct list* files);
void close_current_file(struct list* files, int fid);
void exit_process_by_code(int code);
int execute_process(char *file_name);

struct process_file {
	struct file* file_ptr;
	int fid;
	struct list_elem elem;
};

extern bool running;

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int * stack_ptr = f->esp;

	verify_address(stack_ptr);

  int system_call = *stack_ptr;
	switch (system_call)
	{
		case SYS_HALT:
		shutdown_power_off();
		break;

		case SYS_EXIT:
		verify_address(stack_ptr+1);
		exit_process_by_code(*(stack_ptr+1));
		break;

		case SYS_EXEC:
		verify_address(stack_ptr+1);
		verify_address(*(stack_ptr+1));
		f->eax = execute_process(*(stack_ptr+1));
		break;

		case SYS_WAIT:
		verify_address(stack_ptr+1);
		f->eax = process_wait(*(stack_ptr+1));
		break;

		case SYS_CREATE:
		verify_address(stack_ptr+5);
		verify_address(*(stack_ptr+4));
		file_lock_acquire();
		f->eax = filesys_create(*(stack_ptr+4),*(stack_ptr+5));
		file_lock_release();
		break;

		case SYS_REMOVE:
		verify_address(stack_ptr+1);
		verify_address(*(stack_ptr+1));
		file_lock_acquire();
		if(filesys_remove(*(stack_ptr+1))==NULL)
			f->eax = false;
		else
			f->eax = true;
		file_lock_release();
		break;

		case SYS_OPEN:
		verify_address(stack_ptr+1);
		verify_address(*(stack_ptr+1));

		file_lock_acquire();
		struct file* pf = filesys_open (*(stack_ptr+1));
		file_lock_release();
		if(pf==NULL)
			f->eax = -1;
		else
		{
			struct process_file *pfile = malloc(sizeof(*pfile));
			pfile->file_ptr = pf;
			pfile->fid = thread_current()->fid_count;
			thread_current()->fid_count++;
			list_push_back (&thread_current()->files, &pfile->elem);
			f->eax = pfile->fid;

		}
		break;

		case SYS_FILESIZE:
		verify_address(stack_ptr+1);
		file_lock_acquire();
		f->eax = file_length (get_file(&thread_current()->files, *(stack_ptr+1))->file_ptr);
		file_lock_release();
		break;

		case SYS_READ:
		verify_address(stack_ptr+7);
		verify_address(*(stack_ptr+6));
		if(*(stack_ptr+5)==0)
		{
			int i;
			uint8_t* buffer = *(stack_ptr+6);
			for(i=0;i<*(stack_ptr+7);i++)
				buffer[i] = input_getc();
			f->eax = *(stack_ptr+7);
		}
		else
		{
			struct process_file* pf = get_file(&thread_current()->files, *(stack_ptr+5));
			if(pf==NULL)
				f->eax=-1;
			else
			{
				file_lock_acquire();
				f->eax = file_read (pf->file_ptr, *(stack_ptr+6), *(stack_ptr+7));
				file_lock_release();
			}
		}
		break;

		case SYS_WRITE:
		verify_address(stack_ptr+7);
		verify_address(*(stack_ptr+6));
		if(*(stack_ptr+5)==1)
		{
			putbuf(*(stack_ptr+6),*(stack_ptr+7));
			f->eax = *(stack_ptr+7);
		}
		else
		{
			struct process_file* pf = get_file(&thread_current()->files, *(stack_ptr+5));
			if(f==NULL)
				f->eax=-1;
			else
			{
				file_lock_acquire();
				f->eax = file_write (pf->file_ptr, *(stack_ptr+6), *(stack_ptr+7));
				file_lock_release();
			}
		}
		break;

		case SYS_SEEK:
		verify_address(stack_ptr+5);
		file_lock_acquire();
		file_seek(get_file(&thread_current()->files, *(stack_ptr+4))->file_ptr,*(stack_ptr+5));
		file_lock_release();
		break;

		case SYS_TELL:
		verify_address(stack_ptr+1);
		file_lock_acquire();
		f->eax = file_tell(get_file(&thread_current()->files, *(stack_ptr+1))->file_ptr);
		file_lock_release();
		break;

		case SYS_CLOSE:
		verify_address(stack_ptr+1);
		file_lock_acquire();
		close_current_file(&thread_current()->files,*(stack_ptr+1));
		file_lock_release();
		break;


		default:
		printf("Default %d\n",*stack_ptr);
}
}

void* 
verify_address(const void* vaddr)
{
	if(!is_user_vaddr(vaddr))
	{
		exit_process_by_code(-1);
		return 0;
	}
	void *ptr = pagedir_get_page(thread_current()->pagedir, vaddr);
	if(!ptr)
	{
		exit_process_by_code(-1);
		return 0;
	}
	return ptr;
}

struct 
process_file* get_file(struct list* files, int fid)
{
	struct list_elem *e;

	for(e = list_begin (files); e != list_end (files); e = list_next (e))
	{
		struct process_file *f = list_entry(e, struct process_file, elem);
		if(f->fid == fid)
			return f;
	}

	return NULL;
}

void 
close_all_files(struct list* files)
{
		struct list_elem *e;

		while(!list_empty(files))
		{
			e = list_pop_front (files);

			struct process_file *f = list_entry (e, struct process_file, elem);

			file_close (f->file_ptr);
			list_remove(e);
			free(f);
		}
}

void 
close_current_file(struct list* files, int fid)
{
	struct list_elem *e;
	struct process_file *f;

	for (e = list_begin (files); e != list_end (files); e = list_next(e))
	{
		f = list_entry (e, struct process_file, elem);
		if(f->fid == fid)
		{
			file_close(f->file_ptr);
			list_remove(e);
		}
	}

	free(f);
}

void 
exit_process_by_code(int code)
{
		struct list_elem *e;	

		for (e = list_begin (&thread_current()->parent->child_processes); e != list_end (&thread_current()->parent->child_processes); e = list_next(e))
		{
			struct child_parent *c = list_entry (e, struct child_parent, elem);
			if(c->tid == thread_current()->tid)
			{
					c->has_exited = true;
					c->exit_code = code;
			}
		}

		thread_current()->exit_code = code;

		if(thread_current()->parent->waiting_on_thread == thread_current()->tid)
			sema_up(&thread_current()->parent->child_sema);

		thread_exit();
}

int
execute_process(char *file_name)
{
	file_lock_acquire();
	char* f_copy = malloc(strlen(file_name)+1);
	strlcpy(f_copy, file_name, strlen(file_name)+1);

	char* save_ptr;
	f_copy = strtok_r(f_copy," ",&save_ptr);

	struct file* f = filesys_open(f_copy);

	if(f == NULL)
	{
		file_lock_release();
		return -1;
	}
	else
	{
		file_close(f);
		file_lock_release();
		return process_execute(file_name);
	}
}



