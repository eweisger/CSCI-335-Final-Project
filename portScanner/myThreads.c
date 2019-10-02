#include <pthread.h>
#include "myThreads.h"
#include "linkedList.h"
#include "scanner.h"

void *my_thread(linked_list* ll){

	char service[50];
	char data[255];

	node* n;
	for(n = ll->head; n!= NULL; n = n->next){
		//Check to see if the node is locked
		if(!pthread_mutex_trylock(&n->mutex)){
			my_scanner(n, service, data);
		}
	}
	pthread_exit(0);
}
