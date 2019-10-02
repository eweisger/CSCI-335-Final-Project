#include "linkedList.h"
#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>

//Scanner function for IPv4
void my_scanner(node* n, char* service, char* data){

	int sockfd;

	//Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Error: creating socket: ");
		exit(1);
	}
		
	//Set up to attempt connection
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_UNSPEC;
	serv_addr.sin_port = htons(n->port);
	if(inet_pton(AF_INET, n->ip, &serv_addr.sin_addr) <= 0){
		linked_list_add_status(n, "invalid");
		return;
	}

	//Connection successful, found open port
	if((connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) >= 0){
		getnameinfo((struct sockaddr *) &serv_addr, sizeof(serv_addr), data, 255, service, 50, 0);
		linked_list_add_status(n, "open");
		linked_list_add_data(n, data);
		linked_list_add_service(n, service);
		//printf("Scan Success!\n");
	}
	//Connection failed
	else{
		linked_list_add_status(n, "closed");
		//printf("Scan failed\n");
	}
	close(sockfd);
	n->check = 1;

	return;
}
