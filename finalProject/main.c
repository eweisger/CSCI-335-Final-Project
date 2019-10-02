#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <errno.h>

#include "linkedList.h"
#include "scanner.h"
#include "myThreads.h"
#include "checks.h"

/* Emma Weisgerber
* CSCI 335 Dr. Brian Drawert
* Final Project 5/8/18
* A port scanner that with scan select ips, a range of ips, select host names, and the current network and long with select ports or a range of ports for each ip. Scans each to find an open port and reports back by creating a file, which is sorted to have open ports at the top.
*/

int main(int argc, char**argv){
	
	//Initialize variables
	char input[50];
	char* fix_newline;
	FILE* report;
	struct in_addr addr_struct;
	linked_list *myll = create_linked_list();
	//Variables for IPv4 range from 0.0.0.0 to 255.255.255.255
	char ip_addresses[1024];
	char *ip_start;
	char *ip_end;
	char *ip_current;
	//Variables for Host Names
	char host_names[1024];
	char *host_start;
	char *host_current;
	//Variables for Ports Range from 0 to 65535
	char ports[1024];
	char *temp;
	char *port_start;
	char *port_end;
	char *port_current;
	int current_port; //Used for range of ports
	//Variables for threads
	pthread_t thread_handle[1000];
	
	//UI
	printf("-----Port Scanner------------------------------------------\n"
		"* Would you like to scan IP Addresses, Host Names, or the Current Network?\n"
		"*\n"
		"* 1. IP Addresses\n"
		"* 2. Host Names\n"
		"* 3. Current Network\n"
		"*\n"
		"----------------------------------------------------------\n"
		"Please select an option: ");
		fgets(input, 50, stdin);
		
	if(validate_input(input)){
		printf("Error: Incorrect input, please input the option's number\n");
		destroy_linked_list(myll);
		exit(1);
	}

	//IP Adress
	if(1 == atoi(input)){
		printf("-----Port Scanner------------------------------------------\n"
			"* Would you like to input Select IP Addresses or a Range?\n"
			"*\n"
			"* 1. Select IP Addresses\n"
			"* 2. Range of IP Addresses\n"
			"*\n"
			"----------------------------------------------------------\n"
			"Please select an option: ");
			fgets(input, 50, stdin);
		
		if(validate_input(input)){
			printf("Error: Incorrect input, please input the option's number\n");
			destroy_linked_list(myll);
			exit(1);
		}

		//Select IP address
		if(1 == atoi(input)){
			printf("Please input your IP Addresses: ");
			fgets(ip_addresses, 1024, stdin);

			//Removes the new line character from user input
			fix_newline = index(ip_addresses, '\n');
			*fix_newline = '\0';
			
			printf("-----Port Scanner------------------------------------------\n"
				"* Would you like to input Select Ports or a Range?\n"
				"*\n"
				"* 1. Select Ports\n"
				"* 2. Range of Ports\n"
				"*\n"
				"----------------------------------------------------------\n"
				"Please select an option: ");
				fgets(input, 50, stdin);
			
			if(validate_input(input)){
				printf("Error: Incorrect input, please input the option's number\n");
				destroy_linked_list(myll);
				exit(1);
			}
	
				//Select Ports
				if(1 == atoi(input)){
					printf("Please input your Ports: ");
					fgets(ports, 1024, stdin);
				
					//Removes the new line character from user input	
					fix_newline = index(ports, '\n');
					*fix_newline = '\0';

					printf("Creating list...\n");
					//Place select IPs and select Ports in linked list
					ip_start = ip_addresses;
					while(ip_current = strtok_r(ip_start, " ", &ip_start)){
						//Check if the ip is valid
						if(validate_ip(ip_current)){
							printf("Invalid IP: %s\n", ip_current);
							destroy_linked_list(myll);	
							free(temp);
							exit(1);
						}

						temp = (char*) malloc(sizeof(char) * (strlen(ports)+1));
						strcpy(temp, ports);
						port_start = temp;
						while(port_current = strtok_r(port_start, " ", &port_start)){
							//Check if the port is valid
							if(validate_port(port_current)){
								printf("Invalid Port: %s\n", port_current);
								destroy_linked_list(myll);
								free(temp);
								exit(1);
							}

							//printf("Test 1,1,1 IP: %s Port:%i\n", ip_current, atoi(port_current));
							linked_list_add(myll, ip_current, atoi(port_current));
						}
						free(temp);
					}
				}
				
				//Range of Ports
				else if(2 == atoi(input)){
					printf("Please input your Starting Port followed by your Ending Port: ");
					fgets(ports, 1024, stdin);

					//Removes the new line character from user input	
					fix_newline = index(ports, '\n');
					*fix_newline = '\0';

					printf("Creating list...\n");
					//Place select IPs and Range of Ports into linked list
					port_start = ports;
					port_current = strtok_r(port_start, " ", &port_start);
					port_end = strtok_r(port_start, " ", &port_start);
										
					//Check if start and end port are valid
					if(validate_port(port_current)){
						printf("Invalid Start Port: %s\n", port_current);
						destroy_linked_list(myll);
						exit(1);
					}
					if(validate_port(port_end)){
						printf("Invalid End Port: %s\n", port_end);
						destroy_linked_list(myll);
						exit(1);
					}
					if(atoi(port_current) > atoi(port_end)){
						printf("Invalid Port Range\n");
						destroy_linked_list(myll);
						exit(1);
					}

					ip_start = ip_addresses;
					while(ip_current = strtok_r(ip_start, " ", &ip_start)){
						//Check if IP is valid
						if(validate_ip(ip_current)){
							printf("Invalid IP: %s\n", ip_current);
							destroy_linked_list(myll);
							exit(1);
						} 
	
						for(current_port = atoi(port_current); current_port <= atoi(port_end); current_port++){
							//printf("Test 1,1,2 IP:%s Port:%i\n", ip_current, current_port);
							linked_list_add(myll, ip_current, current_port);
						}
					}
				}
				
				//Incorrect Input
				else{
					printf("Error: Incorrect input, please input the option's number\n");
					destroy_linked_list(myll);
					exit(1);
				}
		}
		
		//Range of IP addresses
		else if(2 == atoi(input)){
			printf("Please input your Starting IP Address followed by your Ending IP Address: ");
			fgets(ip_addresses, 1024, stdin);

			//Removes the new line character from user input	
			fix_newline = index(ip_addresses, '\n');
			*fix_newline = '\0';

			printf("-----Port Scanner------------------------------------------\n"
				"* Would you like to input Select Ports or a Range?\n"
				"*\n"
				"* 1. Select Ports\n"
				"* 2. Range of Ports\n"
				"*\n"
				"----------------------------------------------------------\n"
				"Please select an option: ");
				fgets(input, 50, stdin);
				
			if(validate_input(input)){
				printf("Error: Incorrect input, please input the option's number\n");
				destroy_linked_list(myll);
				exit(1);
			}

				//Select Ports
				if(1 == atoi(input)){
					printf("Please input your Ports: ");
					fgets(ports, 1024, stdin);
					
					//Removes the new line character from user input	
					fix_newline = index(ports, '\n');
					*fix_newline = '\0';
	
					printf("Creating list...\n");
					//Place Range of IPs and select ports
					struct in_addr addr_struct;
					in_addr_t address;

					ip_start = ip_addresses;
					ip_current = strtok_r(ip_start, " ", &ip_start);
					ip_end = strtok_r(ip_start, " ", &ip_start);

					int a,b,c,d;
					int w,x,y,z;

					sscanf(ip_end, "%i.%i.%i.%i", &a, &b, &c, &d);
					sscanf(ip_current, "%i.%i.%i.%i", &w, &x, &y, &z);

					//Check if start and end IP are valid
					if(validate_ip(ip_current)){
						printf("Invalid Start IP: %s\n", ip_current);
						destroy_linked_list(myll);
						exit(1);
					}
					if(validate_ip(ip_end)){
						printf("Invalid End IP: %s\n", ip_end);
						destroy_linked_list(myll);
						exit(1);
					}
					if((w > a) || ((w == a) && (x > b)) || ((w == a) && (x == b) && (y > c)) || ((w == a) && (x == b) && (y == c) && (z > d))){
						printf("Invalid IP Range\n");
						destroy_linked_list(myll);
						exit(1);
					}
					
					while((a != w) || (b != x) || (c != y) || (d != z)){
						temp = (char*) malloc(sizeof(char) * (strlen(ports)+1));
						strcpy(temp, ports);
						port_start = temp;
						while(port_current = strtok_r(port_start, " ", &port_start)){
							//Check if the port is valid
							if(validate_port(port_current)){
								printf("Invalid Port: %s\n", port_current);
								destroy_linked_list(myll);
								free(temp);
								exit(1);
							}

							//printf("Test 1,2,1 IP: %s Port:%i\n", ip_current, atoi(port_current));
							linked_list_add(myll, ip_current, atoi(port_current));
						}
						free(temp);

						address = inet_addr(ip_current);
						address = htonl(ntohl(address) + 1);
						addr_struct.s_addr = address;
						ip_current = inet_ntoa(addr_struct);
						sscanf(ip_current, "%i.%i.%i.%i", &w, &x, &y, &z);
					}
					//Add last IP
					temp = (char*) malloc(sizeof(char) * (strlen(ports)+1));
					strcpy(temp, ports);
					port_start = temp;
					while(port_current = strtok_r(port_start, " ", &port_start)){
						//printf("Test 1,2,1 IP: %s Port:%i\n", ip_current, atoi(port_current));
						linked_list_add(myll, ip_current, atoi(port_current));
					}
					free(temp);
				}
				
				//Range of Ports
				else if(2 == atoi(input)){
					printf("Please input your Starting Port followed by your Ending Port: ");
					fgets(ports, 1024, stdin);
			
					//Removes the new line character from user input	
					fix_newline = index(ports, '\n');
					*fix_newline = '\0';

					printf("Creating list...\n");
					//Place Range of IPs and Range of Ports
					port_start = ports;
					port_current = strtok_r(port_start, " ", &port_start);
					port_end = strtok_r(port_start, " ", &port_start);

					struct in_addr addr_struct;
					in_addr_t address;

					ip_start = ip_addresses;
					ip_current = strtok_r(ip_start, " ", &ip_start);
					ip_end = strtok_r(ip_start, " ", &ip_start);

					int a,b,c,d;
					int w,x,y,z;

					sscanf(ip_end, "%i.%i.%i.%i", &a, &b, &c, &d);
					sscanf(ip_current, "%i.%i.%i.%i", &w, &x, &y, &z);
					
					//Check if start and end IP are valid
					if(validate_ip(ip_current)){
						printf("Invalid Start IP: %s\n", ip_current);
						destroy_linked_list(myll);
						exit(1);
					}
					if(validate_ip(ip_end)){
						printf("Invalid End IP: %s\n", ip_end);
						destroy_linked_list(myll);
						exit(1);
					}
					if((w > a) || ((w == a) && (x > b)) || ((w == a) && (x == b) && (y > c)) || ((w == a) && (x == b) && (y == c) && (z > d))){
						printf("Invalid IP Range\n");
						destroy_linked_list(myll);
						exit(1);
					}
	
					//Check if start and end port are valid
					if(validate_port(port_current)){
						printf("Invalid Start Port: %s\n", port_current);
						destroy_linked_list(myll);
						exit(1);
					}
					if(validate_port(port_end)){
						printf("Invalid End Port: %s\n", port_end);
						destroy_linked_list(myll);
						exit(1);
					}
					if(atoi(port_current) > atoi(port_end)){
						printf("Invalid Port Range\n");
						destroy_linked_list(myll);
						exit(1);
					}

					while((a != w) || (b != x) || (c != y) || (d != z)){
						for(current_port = atoi(port_current); current_port <= atoi(port_end); current_port++){
							//printf("Test 1,2,1 IP:%s Port:%i\n", ip_current, current_port);
							linked_list_add(myll, ip_current, current_port);
						}
						address = inet_addr(ip_current);
						address = htonl(ntohl(address) + 1);
						addr_struct.s_addr = address;
						ip_current = inet_ntoa(addr_struct);
						sscanf(ip_current, "%i.%i.%i.%i", &w, &x, &y, &z);
					}
					//Add last IP
					for(current_port = atoi(port_current); current_port <= atoi(port_end); current_port++){
						//printf("Test 1,2,1 IP:%s Port:%i\n", ip_current, current_port);
						linked_list_add(myll, ip_current, current_port);
					}
	
				}
				
				//Incorrect Input
				else{
					printf("Error: Incorrect input, please input the option's number\n");
					destroy_linked_list(myll);
					exit(1);
				}
		}

		//Incorrect input
		else{
			printf("Error: Incorrect input, please input the option's number\n");
			destroy_linked_list(myll);
			exit(1);
		}
	}
	
	//Host name
	else if(2 == atoi(input)){
		printf("Please input your Host Names: ");
		fgets(host_names, 1024, stdin);

		//Removes the new line character from user input	
		fix_newline = index(host_names, '\n');
		*fix_newline = '\0';

		printf("-----Port Scanner------------------------------------------\n"
				"* Would you like to input Select Ports or a Range?\n"
				"*\n"
				"* 1. Select Ports\n"
				"* 2. Range of Ports\n"
				"*\n"
				"----------------------------------------------------------\n"
				"Please select an option: ");
				fgets(input, 50, stdin);
				
		if(validate_input(input)){
			printf("Error: Incorrect input, please input the option's number\n");
			destroy_linked_list(myll);
			exit(1);
		}
	

		//Select Ports
		if(1 == atoi(input)){
			printf("Please input your Ports: ");
			fgets(ports, 1024, stdin);

			//Removes the new line character from user input	
			fix_newline = index(ports, '\n');
			*fix_newline = '\0';

			printf("Creating list...\n");
			//Place IPs associated with provided hostnames and select ports
			struct addrinfo hints;
			struct addrinfo *p;
			struct addrinfo *listp;
					
			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			host_start = host_names;
			while(host_current = strtok_r(host_start, " ", &host_start)){
				if(getaddrinfo(host_current, NULL, &hints, &listp)){
					printf("Invalid Host Name: %s\n", host_current);
					destroy_linked_list(myll);
					freeaddrinfo(listp);
					free(temp);
					exit(1);
				}
				for(p = listp; p; p = p->ai_next){
					if(getnameinfo(p->ai_addr, p->ai_addrlen, ip_addresses, 1024, NULL, 0, NI_NUMERICHOST)){
						perror("Error: getnameinfo retrieving address from host list: ");
						continue;
					}

					temp = (char*) malloc(sizeof(char) * (strlen(ports)+1));
					strcpy(temp, ports);
					port_start = temp;
					while(port_current = strtok_r(port_start, " ", &port_start)){
						//Check if valid port
						if(validate_port(port_current)){
							printf("Invalid Port: %s\n", port_current);
							destroy_linked_list(myll);
							freeaddrinfo(listp);
							free(temp);
							exit(1);
						}

						//printf("Test 2,1 Host:%s IP:%s Port:%i\n", host_current, ip_addresses, atoi(port_current));
						linked_list_add(myll, ip_addresses, atoi(port_current));
					}
					free(temp);
				}
			}
			freeaddrinfo(listp);
		}
				
		//Range of Ports
		else if(2 == atoi(input)){
			printf("Please input your Starting Port followed by your Ending Port: ");
			fgets(ports, 1024, stdin);

			//Removes the new line character from user input	
			fix_newline = index(ports, '\n');
			*fix_newline = '\0';

			printf("Creating list...\n");
			//Place IPs associated with provided hostnames and a range of ports
			port_start = ports;
			port_current = strtok_r(port_start, " ", &port_start);
			port_end = strtok_r(port_start, " ", &port_start);
	
			//Check if start and end port are valid
			if(validate_port(port_current)){
				printf("Invalid Start Port: %s\n", port_current);
				destroy_linked_list(myll);
				exit(1);
			}
			if(validate_port(port_end)){
				printf("Invalid End Port: %s\n", port_end);
				destroy_linked_list(myll);
				exit(1);
			}
			if(atoi(port_current) > atoi(port_end)){
				printf("Invalid Port Range\n");
				destroy_linked_list(myll);
				exit(1);
			}

			struct addrinfo hints;
			struct addrinfo *p;
			struct addrinfo *listp;
				
			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			host_start = host_names;
			while(host_current = strtok_r(host_start, " ", &host_start)){
				if(getaddrinfo(host_current, NULL, &hints, &listp)){
					printf("Invalid Host Name: %s\n", host_current);
					destroy_linked_list(myll);
					freeaddrinfo(listp);
					exit(1);
				}
				for(p = listp; p; p = p->ai_next){
					if(getnameinfo(p->ai_addr, p->ai_addrlen, ip_addresses, 1024, NULL, 0, NI_NUMERICHOST)){
						perror("Error: getnameinfo retrieving address from host list: ");
						continue;
					}

					for(current_port = atoi(port_current); current_port <= atoi(port_end); current_port++){
						//printf("Test 2,2 Host:%s IP:%s Port:%i\n", host_current, ip_addresses, current_port);
						linked_list_add(myll, ip_addresses, current_port);
					}
				}
			}
			freeaddrinfo(listp);
		}
				
		//Incorrect Input
		else{
			printf("Error: Incorrect input, please input the option's number\n");
			destroy_linked_list(myll);
			exit(1);
		}
	}
	
	//Current Network
	else if(3 == atoi(input)){
		printf("-----Port Scanner------------------------------------------\n"
				"* Would you like to input Select Ports or a Range?\n"
				"*\n"
				"* 1. Select Ports\n"
				"* 2. Range of Ports\n"
				"*\n"
				"----------------------------------------------------------\n"
				"Please select an option: ");
				fgets(input, 50, stdin);
	
		if(validate_input(input)){
			printf("Error: Incorrect input, please input the option's number\n");
			destroy_linked_list(myll);
			exit(1);
		}

		//Select Ports
		if(1 == atoi(input)){
			printf("Please input your Ports: ");
			fgets(ports, 1024, stdin);

			//Removes the new line character from user input	
			fix_newline = index(ports, '\n');
			*fix_newline = '\0';

			printf("Creating list...\n");
			//Place IPs associated with current network and select ports
			struct ifaddrs *ifaddr;
			struct ifaddrs *ifa;

			//Get list of current ips assocaited with local machine
			if(getifaddrs(&ifaddr) == 1){
				perror("Error: getifaddrs getting list of IPs from current network: ");
				destroy_linked_list(myll);
				freeifaddrs(ifaddr);
				exit(1);
			}
			
			//Go through the list of ips assocaited with local machine
			for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
				//Skip if the entry has an empty ip field
				if(ifa->ifa_addr == NULL){
					continue;
				}

				//Check for valid IPv4 addresses only
				if(ifa->ifa_addr->sa_family == AF_INET){
					if(getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), ip_addresses, 1024, NULL, 0, NI_NUMERICHOST)){
						perror("Error: getnameinfo retrieving address from local machine list: ");
						continue;
					}
				
					//Ignore if ip is 127.0.0.1
					if(strcmp(ip_addresses, "127.0.0.1")){
						temp = (char*) malloc(sizeof(char) * (strlen(ports)+1));
						strcpy(temp, ports);
						port_start = temp;
						while(port_current = strtok_r(port_start, " ", &port_start)){
							//Check if port is valid
							if(validate_port(port_current)){
								printf("Invalid Port: %s\n", port_current);
								destroy_linked_list(myll);
								freeifaddrs(ifaddr);
								free(temp);
								exit(1);
							}
			
							//printf("Test 3,1 Host:%s IP:%s Port:%i\n", " loacl machine", ip_addresses, atoi(port_current));
							linked_list_add(myll, ip_addresses, atoi(port_current));
						}
						free(temp);
					}
				}
			}
			freeifaddrs(ifaddr);
		}
				
		//Range of Ports
		else if(2 == atoi(input)){
			printf("Please input your Starting Port followed by your Ending Port: ");
			fgets(ports, 1024, stdin);

			//Removes the new line character from user input	
			fix_newline = index(ports, '\n');
			*fix_newline = '\0';

			printf("Creating list...\n");
			//Place IPs associated with current network and a range of ports
			port_start = ports;
			port_current = strtok_r(port_start, " ", &port_start);
			port_end = strtok_r(port_start, " ", &port_start);
	
			//Check if start and end port are valid
			if(validate_port(port_current)){
				printf("Invalid Start Port: %s\n", port_current);
				destroy_linked_list(myll);
				exit(1);
			}
			if(validate_port(port_end)){
				printf("Invalid End Port: %s\n", port_end);
				destroy_linked_list(myll);
				exit(1);
			}
			if(atoi(port_current) > atoi(port_end)){
				printf("Invalid Port Range\n");
				destroy_linked_list(myll);
				exit(1);
			}
	
			struct ifaddrs *ifaddr;
			struct ifaddrs *ifa;

			//Get list of current ips assocaited with local machine
			if(getifaddrs(&ifaddr) == 1){
				perror("Error: getifaddrs getting list of IPs from current network: ");
				destroy_linked_list(myll);
				freeifaddrs(ifaddr);
				exit(1);
			}
			
			//Go through the list of ips assocaited with local machine
			for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
				//Skip if the entry has an empty address field
				if(ifa->ifa_addr == NULL){
					continue;
				}

				//Check for IPv4 addresses onlu
				if(ifa->ifa_addr->sa_family == AF_INET){
					if(getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), ip_addresses, 1024, NULL, 0, NI_NUMERICHOST)){
						perror("Error: getnameinfo retrieving address from local machine list: ");
						continue;
					}

					//Ignore if ip is 127.0.0.1
					if(strcmp(ip_addresses, "127.0.0.1")){
						for(current_port = atoi(port_current); current_port <= atoi(port_end); current_port++){
							//printf("Test 3,2 Host:%s IP:%s Port:%i\n", " local machine", ip_addresses, current_port);
							linked_list_add(myll, ip_addresses, current_port);
						}
					}
				}
			}
			freeifaddrs(ifaddr);
		}
				
		//Incorrect Input
		else{
			printf("Error: Incorrect input, please input the option's number\n");
			destroy_linked_list(myll);
			exit(1);
		}
	}
	
	//Incorrect input
	else{
		printf("Error: Incorrect input, please input the option's number\n");
		destroy_linked_list(myll);
		exit(1);
	}




	//Checking
	/*node* n;
	printf("List before scanner\n");
	for(n = myll->head; n != NULL; n = n->next){
		printf("ip:%s port:%i scanned:%i\n", n->ip, n->port, n->check);
	}
	printf("Linked list count: %i\n", myll->count);*/

	//Scanning
	printf("Scanning... Please wait\n");
	int i;
	for(i = 0; i < 1000; i++){
		pthread_create(&thread_handle[i], NULL, my_thread, myll);
	}
	for(i = 0; i < 1000; i++){
		pthread_join(thread_handle[i], NULL);
	}

	//Checking
	/*printf("List after scanner\n");
	for(n = myll->head; n != NULL; n = n->next){
		printf("ip:%s port:%i scanned:%i\n", n->ip, n->port, n->check);
	}
	printf("Linked list count: %i\n", myll->count);*/

	printf("Creating report...\n");
	//Sort the scanned list
	//printf("Calling sort\n");
	linked_list_sort(myll);

	//Print report
	//printf("printing report\n");
	node* n;
	report = fopen("report.txt", "w");
	for(n = myll->head; n != NULL; n = n->next){
		if(!strcmp(n->status, "closed")){
			//printf("%s : %i \t %s \t\n", n->ip, n->port, n->status);
			fprintf(report, "%s : %i \t %s \t\n", n->ip, n->port, n->status);
		}
		else{
			//printf("%s : %i \t %s \t %s : %s\n", n->ip, n->port, n->status, n->data, n->service);
			fprintf(report, "%s : %i \t %s \t %s : %s\n", n->ip, n->port, n->status, n->data, n->service);
		}
	}
	fclose(report);

	//Destroy list
	//printf("Destroying linked list\n");
	destroy_linked_list(myll);
	printf("Done!\n");

	exit(0);
}
