#ifndef linked_list_h //only include f not included already to prevent loops/repeats
#define linked_list_h

// Createdata structures for a node of the list

#include <pthread.h>

struct node_t  {
	char *ip;
	int port;

	char *status;
	char *data;
	char *service;

	int check;
	pthread_mutex_t mutex;

	struct node_t * next;
};
typedef struct node_t node;

// Data structure for the linked list type

typedef struct {
	node*head;
	int count;
} linked_list;

// Functions to manipulate the linked list

// constructor
linked_list* create_linked_list();

// destructor
void destroy_linked_list( linked_list* ll );

// add a new node to the end of the linked list
void linked_list_add( linked_list* ll, char* ip_in, int port_in);

//allocates memory for node's data if port is open
void linked_list_add_data(node* n, char* data_in);

//allocates memory for node's service if port is open
void linked_list_add_service( node* n, char* service_in);

//allocates memeory for node's status once determined
void linked_list_add_status(node* n, char* status_in);

// delete a node from the linked list
void linked_list_delete( linked_list* ll, node* to_delete);

// merge sort the linked list
void linked_list_sort( linked_list* ll);

#endif /* linked_list_h*/
