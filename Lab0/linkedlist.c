#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"


struct list_item {
    int value;
    struct list_item * next;
};

void append(struct list_item *first, int x){
    struct list_item *index = first;
    while(index->next != NULL){
	index = index->next;
    }
    struct list_item *newNode = (struct list_item *) malloc(sizeof(struct list_item));
    newNode->value = x;
    newNode->next = NULL;
    index->next = newNode;
} /* puts x at the end of the list */

void prepend(struct list_item *first, int x){
    struct list_item *newNode = (struct list_item *) malloc(sizeof(struct list_item));
    newNode->value = x;
    newNode->next = first->next;
    first->next = newNode;
} /* puts x at the beginning of the list */

void print(struct list_item *first){
    struct list_item *index = first;
    while(index->next != NULL){
	printf("%d", index->next->value);
	index = index->next;
    }
}

 /* prints all elements in the list */
 /* input_sorted: find the first element in the list larger than x
 and input x right before that element */
void input_sorted(struct list_item *first, int x){
    struct list_item* index = first;
    while(index->next != NULL && index->next->value <= x){
	index = index->next;
    }
    struct list_item *newNode = (struct list_item *) malloc(sizeof(struct list_item));
    newNode->value = x;
    newNode->next = index->next;
    index->next = newNode;
}

void destroy(struct list_item *first){
    struct list_item *cur, *next;
    cur = first->next;
    while(cur != NULL){
	next = cur->next;
	free(cur);
	cur = next;
    }
    first->next = NULL;
} /* free everything dynamically allocated */

int main( int argc, char ** argv){
    struct list_item root;
    root.value = -1; /* This value is always ignored */
    root.next = NULL;
}

