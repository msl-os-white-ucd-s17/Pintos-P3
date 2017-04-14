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
void exit_process_by_code(int code);


struct process_file {
	struct file* file_ptr;
	int fid;
	struct list_elem elem;
};


void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  	int * stack_ptr = f->esp;
 	int system_call = *stack_ptr;
	switch (system_call)
	{
		case SYS_HALT:
		shutdown_power_off();
		break;

		case SYS_EXIT:
		exit_process_by_code(*(stack_ptr+1));
		break;

		case SYS_EXEC:
		break;

		case SYS_WAIT:
		break;

		case SYS_CREATE:	
		break;

		case SYS_REMOVE:	
		break;

		case SYS_FILESIZE:		
		break;

		case SYS_WRITE:
		break;

		case SYS_SEEK:	
		break;

		case SYS_TELL:	
		break;

		case SYS_CLOSE:
		break;

		default:
		printf("Default %d\n",*stack_ptr);
`	}
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





