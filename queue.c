#include <stdlib.h>         // Standard C library: free(), NULL...  
#include <stdbool.h>        // Standard true, false, and bool type  
#include "lib/contracts.h"  // Our contracts library  
#include "lib/xalloc.h"     // Our allocation library  
#include "queue.h"          // The queue interface  

typedef struct list_node list;
struct list_node {
    void *data;
    list *next;
};

typedef struct queue_header queue;
struct queue_header {
    list *front;
    list *back;
    int size;
};

bool is_inclusive_segment(list *start, list *end, size_t i) {
    /* FIX ME */
    if (start == NULL) return i == 0;
    else if (start == end) return start->next == NULL && i == 1;
    else return is_inclusive_segment(start->next,end,i-1);
}

bool is_queue(queue_t Q) {
    /* FIX ME */
    return Q != NULL && is_inclusive_segment(Q->front, Q->back, Q->size);
}

queue_t queue_new()
/*@ensures is_queue(\result); @*/ 
{
    queue_t Q = xmalloc(sizeof(queue));
    Q->front = NULL;
    Q->back = NULL;
    Q->size = 0;
    ENSURES(is_queue(Q));
    return Q;
}

size_t queue_size(queue_t Q)
/*@requires is_queue(Q); @*/
/*@ensures \result >= 0 && is_queue(Q); @*/ 
{
    REQUIRES(is_queue(Q));
    ENSURES(is_queue(Q));
    return Q->size;
}

void enq(queue_t Q, void* x)
/*@requires is_queue(Q); @*/
/*@ensures is_queue(Q); @*/ 
{
    REQUIRES(is_queue(Q));
    list *new = xmalloc(sizeof(list));
    new->next = NULL;
    new->data = x;
    if (queue_size(Q) == 0) Q->front = new;
    else Q->back->next = new;
    Q->back = new;
    Q->size = Q->size + 1;
    ENSURES(is_queue(Q));
}
void* deq(queue_t Q)
/*@requires is_queue(Q) && queue_size(Q) > 0; @*/ 
//@ensures is_queue(Q);
{
    REQUIRES(is_queue(Q) && 0 < queue_size(Q));
    list *x = Q->front;
    void *d = x->data;
    Q->front = Q->front->next;
    Q->size = Q->size - 1;
    free(x);
    ENSURES(is_queue(Q));
    return d;
}
void* queue_peek(queue_t Q, size_t i)
/*@requires is_queue(Q) && 0 <= i && i < queue_size(Q); @*/ 
//@ensures is_queue(Q);
{
    REQUIRES(is_queue(Q) && i < queue_size(Q));
    list *temp = Q->front;
    for (size_t j = 0; j < i; j++) 
    {
        temp = temp->next;
    }
    ENSURES(is_queue(Q));
    return temp->data;
}
void queue_reverse(queue_t Q)
/*@requires is_queue(Q); @*/ 
//@ensures is_queue(Q);
{
    REQUIRES(is_queue(Q));
    if (queue_size(Q) <= 1) return;
    else if (Q->size == 2) 
    {
        list *temp = Q->front;
        Q->front = Q->back;
        Q->back = temp;
        Q->front->next = Q->back;
        Q->back->next = NULL;
        ENSURES(is_queue(Q));
        return;
    }
    list *end = Q->front;list *current = end->next;
	list *next = current->next;
	while (current != NULL) 
    {
		next = current->next;
		current->next = end;
		end = current;
		current = next;
	}
	list *b = Q->back;
	Q->back = Q->front;
	Q->front = b;
	Q->back->next = NULL;
    ENSURES(is_queue(Q));
}

bool queue_all(queue_t Q, check_property_fn *P) 
/*@requires is_queue(Q) && P != NULL; @*/
//@ensures is_queue(Q);
{
    REQUIRES(is_queue(Q) && P != NULL);
    if (queue_size(Q) <= 0) return true;
    list *temp = Q->front;
    while (temp != NULL) 
    {
        if (!(*P)(temp->data)) return false;
        temp = temp->next;
    } 
    ENSURES(is_queue(Q));
    return true;
}


void* queue_iterate(queue_t Q, void* base, iterate_fn *F)
/*@requires is_queue(Q) && F != NULL; @*/ 
//@ensures is_queue(Q);
{
    REQUIRES(is_queue(Q) && F != NULL);
    if (queue_size(Q) <= 0) return base;
    list *temp = Q->front;
    void *x = base;
    while (temp != NULL) 
    {
        x = (*F)(x, temp->data);
        temp = temp->next;
    }
    ENSURES(is_queue(Q));
    return x;
}

void queue_free(queue_t Q, free_fn *F) 
{
    REQUIRES(Q != NULL);
    REQUIRES(is_queue(Q));
    if(queue_size(Q) > 0) {
        if (F == NULL) {
            list *c = Q->front;
            while (c != Q->back) {
                list *temp = c;
                c = c->next;
                free(temp->data);
                free(temp);
            }
            free(c->data);
            free(c);
        }
        else {
            list *d = Q->front;
            while(d != Q->back) {
                list *temp2 = d; 
                (*F)(d->data);
                d = d->next;      
                free(temp2->data);
                free(temp2);
            }
            (*F)(d->data);
            free(d->data);
            free(d);
            

        } 
        free(Q);   
    
    }
      
} 