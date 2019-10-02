#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
#include "linkedList.h"

// constructor
linked_list* create_linked_list(){
	linked_list* ll = (linked_list*) malloc( sizeof(linked_list));
	ll->count = 0;
	ll->head = NULL;
	return ll;
}

//deconstructor
void destroy_linked_list( linked_list* ll){
	//empty the linked list
	while(ll->count > 0){
		linked_list_delete(ll, ll->head );
	}
	//un-allocate the memory
	free(ll);
}

// add a new node to the end of the linked list
void linked_list_add( linked_list* ll, char* ip_in, int port_in){
	node* n = (node *) malloc(sizeof(node));
	n->ip = (char*) malloc(sizeof(char) * (strlen(ip_in)+1));
	strcpy(n->ip, ip_in);

	n->port = port_in;

	//Defaults
	n->check = 0;
	n->data = NULL;
	n->service = NULL;
	n->status = NULL;
	//Mutex for threads
	pthread_mutex_init(&n->mutex, NULL);

	n->next = NULL;
	//traverse the list
	node* t = ll->head;
	if(t == NULL){
		ll->head =n;
	}else{
		while( t->next != NULL){
			t = t->next;
		}		
		t->next = n;
	}
	//increase the size
	ll->count++;
}

//allocates memory for node's data if port is open
void linked_list_add_data(node* n, char* data_in){
	n->data = (char*) malloc(sizeof(char) * (strlen(data_in)+1));
	strcpy(n->data, data_in);
}

//allocates memory for node's service if port is open
void linked_list_add_service( node* n, char* service_in){
	n->service = (char*) malloc(sizeof(char) * (strlen(service_in)+1));
	strcpy(n->service, service_in);
}

//allocate memory for node's status once determined
void linked_list_add_status(node* n, char* status_in){
	n->status = (char*) malloc(sizeof(char) * (strlen(status_in)+1));
	strcpy(n->status, status_in);
}

// delete a node from the linked list
void linked_list_delete( linked_list* ll, node* to_delete){
	node* prev_node;
	if( ll-> head == NULL){
		printf("Error, linked_list_delete() empty list\n");
		return;
	}else if(to_delete == ll->head){
		ll->head = ll->head->next;
	}else{
		for(prev_node = ll->head; prev_node->next!=NULL; prev_node = prev_node->next){
			if(prev_node->next == to_delete){
				break;
			}
		}
		if(prev_node->next == NULL){
			printf("Error, linked_list_delete(), could not find item in list\n");
			return;
		}
		prev_node->next = to_delete->next; // connect the list
	}
	//free and reduce size
	//printf("freeing linked list node: %i\n", ll->count);
	ll->count--;
	free(to_delete->ip);
	free(to_delete->status);
	free(to_delete->data);
	free(to_delete->service);
	//Delete mutex for threads
	pthread_mutex_destroy(&to_delete->mutex);
	free(to_delete);
}

//Sort the linked list bringing open ports to the top of the list
void linked_list_sort(linked_list* ll){
	node* n;
	node* toSwap = NULL;
	node* toSwapPrev = NULL;
	
	//The list is empty
	if(ll->head == NULL){
		return;
	}

	//The list only has one element
	if(ll->head->next == NULL){
		return;
	}
	
	int count = 0;
	
	//Walk through list, if n->next has a status of open, make n->next the new head
	for(n = ll->head; n->next != NULL; n = n->next){
		//printf("checking node %i\n", count);
		//fflush(stdout);
		if(!strcmp(n->next->status, "open")){
			toSwapPrev = n;
			toSwap = n->next;
			n->next = toSwap->next;

			toSwap->next = ll->head;
			ll->head = toSwap;
		}
		count++;
	}

	return;
}


