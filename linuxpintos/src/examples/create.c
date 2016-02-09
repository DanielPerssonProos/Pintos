/* The following program should be killed by the kernel because the
   string sent with create starts at one page (the BSS page) and ends
   outside of it in a page that is not allocated.

   If user memory accesses are not handled correctly by the kernel,
   then pintos will crash with a page fault exception.
   
   Author: Mattias Eriksson
*/

#include <syscall.h>
#include <stdio.h>
#include <string.h>

#define PGS 4096 /* page size */
#define PMASK 0xfffff000

static char inbss;

int
main (void)
{
  create("file", 1);
  exit(0);
}
