#ifndef DEVICES_TIMER_H
#define DEVICES_TIMER_H

#include <round.h>
#include <stdint.h>
#include "list.h"
#include "threads/thread.h"
<<<<<<< HEAD
#include "threads/synch.h"
=======
>>>>>>> 66fd1f80f5a37b19f1c445facc5b2bae5c59d24d

/* Number of timer interrupts per second. */
#define TIMER_FREQ 100

void timer_init (void);
void timer_calibrate (void);

int64_t timer_ticks (void);
int64_t timer_elapsed (int64_t);

void timer_sleep (int64_t ticks);
void timer_msleep (int64_t milliseconds);
void timer_usleep (int64_t microseconds);
void timer_nsleep (int64_t nanoseconds);

void timer_print_stats (void);


struct sleeping_thread{
  struct list_elem elem;
  uint64_t wakeup_time;
  struct thread* t;
};



#endif /* devices/timer.h */

struct sleeping_thread
{
  struct list_elem elem;
  int64_t wakeup_time;
  struct semaphore s;
};
