#include <stdlib.h> // Standard C library: free(), NULL...
#include <stdbool.h> // Standard true, false, and bool type
#include "lib/contracts.h" // Our contracts library
#include "lib/xalloc.h" // Our allocation library
#include "queue.h" // The queue interface
void free_int(void* x) {
	if (x != NULL){
		printf("%d", *((int*)x)); 

    }
}
int main() {

	queue_t Q = queue_new();
	int *in;

	for (int i = 0; i < 20; i++) {
		in = xmalloc(sizeof(int));
		*in = i;
		enq(Q, (void*)in);
	}
	assert(*(int*)queue_peek(Q, 10) == 10);
	
	queue_free(Q, &free_int);


	return 0;
}