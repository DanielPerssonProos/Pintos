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
    unsigned cur_size = size;
    if(fd == STDOUT_FILENO){
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
read_syscall(int fd, void *buffer, unsigned size){
    if(fd == STDIN_FILENO){
	unsigned size_left = size;
	char *text[size];
	char *cur = text;
	while(size_left > 0){
	    cur[0] = (char) input_getc();
	    cur++;
	    size_left--;
	}
	buffer = (void*)text;
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
    int fnd = bitmap_scan_and_flip(thread_current()->foomap,2,1,0);
    //Returns -1 if the bitmap is full.
    if(fnd != BITMAP_ERROR){
	thread_current()->files[fnd] = opened;
	return fnd;
    } else {
	return -1;
    }
}

void
exit_syscall(int status){
    int index = 2;
    index = bitmap_scan(thread_current()->foomap, index, 1, 1);
    while(index != BITMAP_ERROR){
	close_syscall(index);
	index = bitmap_scan(thread_current()->foomap, index, 1, 1);
    }
    bitmap_set_multiple(thread_current()->foomap, 2, 128, 1);
    thread_exit();
}

void
close_syscall(int fd){
    if(fd >= 2 && bitmap_test(thread_current()->foomap, fd)){
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
	result_int = read_syscall((int)args[1],(void*)args[2],(unsigned)args[3]);
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
  //thread_exit ();
}
