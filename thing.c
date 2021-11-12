#include <stdlib.h>         // Standard C library: free(), NULL...  
#include <stdbool.h>        // Standard true, false, and bool type  
#include "lib/contracts.h"  // Our contracts library  
#include "lib/xalloc.h"     // Our allocation library  
#include "queue.h"          // The queue interface  


struct reading_header {
int x; // position in 3D
int y;
int z;
int heading; // direction: angle from North
};


typedef struct reading_node reading_list;

typedef struct reading_header reading;

struct reading_node {
reading *data;
reading_list *next;
};



int main() {
reading_list *honk = xmalloc(sizeof(reading_list));
honk->data = xcalloc(sizeof(reading), 1);
honk->next = xmalloc(sizeof(reading_list));
honk->next->next = xmalloc(sizeof(reading_list));
honk->next->next->data = xmalloc(sizeof(reading));
honk->next->next->data->x = 13;
honk->next->next->data->y = 7;
honk->next->next->data->heading = 122;
printf("%d", honk->next->next->data->x);
printf("%d",honk->next->next->data->y);
printf("%d", honk->next->next->data->heading);
/*
// Memory Diagram 1
free(honk->next->next->next);
free(honk->next->next);
free(honk->next->data);
free(honk->data);
free(honk);
*/
// Memory Diagram 2
return 0;
}
