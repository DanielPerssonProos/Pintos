#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include <list.h>
#include "threads/synch.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct child_process {
    char *file_name;
    tid_t tid;
    struct semaphore s;
    unsigned ref_cnt;
    int exit_status;
    struct list_elem elem;
};
struct child_process *reference;
struct list *child_processes;
#endif /* userprog/process.h */
