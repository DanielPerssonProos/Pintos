struct list_item;

void append(struct list_item *first, int x); /* puts x at the end of the list */

void prepend(struct list_item *first, int x); /* puts x at the beginning of the list */

void print(struct list_item *first);  /* prints all elements in the list */
  /* input_sorted: find the first element in the list larger than x
     and input x right before that element */

void input_sorted(struct list_item *first, int x);

void destroy(struct list_item *first); /* free everything dynamically allocated */
