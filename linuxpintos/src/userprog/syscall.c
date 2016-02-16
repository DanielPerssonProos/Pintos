#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "devices/input.h"


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


void
halt_syscall(){
    power_off();
}

bool
create_syscall(const char *file, unsigned initial_size){
    bool success = filesys_create(file, (off_t)initial_size);
    return success;
}

int 
write_syscall(int fd, const void *buffer, unsigned size) {
    if(fd == STDOUT_FILENO){ 
        unsigned cur_size = size;
	while(cur_size > 1000) {
	    putbuf((char*)buffer,(size_t) 1000);
	    cur_size -= 1000;
	}
	putbuf((char*)buffer,(size_t)cur_size);
	return size;
    }else if(fd < 130 && fd >= 2 && bitmap_test(thread_current()->foomap, fd)){
	struct file* write_file = thread_current()->files[fd];
	return file_write(write_file, buffer, size);
    }else{
	return -1;
    }
}

int
read_syscall(int fd, char *buffer, unsigned size){
    if(fd == STDIN_FILENO){
      int i = 0;
      for (i; i < size; i++)
	  buffer[i] = input_getc();
      return size;
    }else if(fd < 130 && fd >= 2 && bitmap_test(thread_current()->foomap, fd)){
	struct file* read_file = thread_current()->files[fd];
	return file_read(read_file, buffer, size);
    }else{
	return -1;
    }
}

int
open_syscall(const char *file){
    struct file* opened = filesys_open(file);
    if(opened == NULL) {
	return -1;
    }
    int fd = bitmap_scan_and_flip(thread_current()->foomap,2,1,0);
    if(fd != BITMAP_ERROR){
	thread_current()->files[fd] = opened;
	return fd;
    } else {
        file_close(opened);
	return -1;
    }
}

void
exit_syscall(int status){
    thread_exit();
}

void
close_syscall(int fd){
    if(fd >= 2 && fd < 130 && bitmap_test(thread_current()->foomap, fd)){
	file_close(thread_current()->files[fd]);
	bitmap_flip(thread_current()->foomap, fd);
    }
}

static void
syscall_handler (struct intr_frame *f) 
{
    int* args = (int*) f->esp;
    int call = args[0];
    int result_int;
    bool result_bool;
    switch(call){
    case SYS_HALT:
	halt_syscall();
	break;
    case SYS_CREATE:
	result_bool = create_syscall((char*)args[1],(unsigned)args[2]);
	f->eax = result_bool;
	break;
    case SYS_WRITE:
	result_int = write_syscall((int)args[1],(void*)args[2],(unsigned)args[3]);
	f->eax = result_int;
	break;
    case SYS_READ:
	result_int = read_syscall((int)args[1],(char*)args[2],(unsigned)args[3]);
	f->eax = result_int;
	break;
    case SYS_OPEN:
	result_int = open_syscall((char*)args[1]);
	f->eax = result_int;
	break;
    case SYS_CLOSE:
	close_syscall((int)args[1]);
	break;
    case SYS_EXIT:
	exit_syscall((int)args[1]);
	break;
    default:
	break;
    }
}
