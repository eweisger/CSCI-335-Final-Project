#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Returns 1 if the ip is invaid or 0 if valid
int validate_ip(char* ip){

	int a, b, c, d, x;
	//Check for proper format and ensure each octet is an int
	if(sscanf(ip, "%i.%i.%i.%i.%i", &a, &b, &c, &d, &x) != 4){
		return 1;
	}
		
	//check to make sure each octet is between 0 and 255
	if((a < 0) || (a > 255)){
		return 1;
	}
	if((b < 0) || (b > 255)){
		return 1;
	}
	if((c < 0) || (c > 255)){
		return 1;
	}
	if((d < 0) || (d > 255)){
		return 1;
	}

	return 0;
}

//Returns 1 if the port is invalid or 0 if valid
int validate_port(char* port){

	int i;
	//Check to make sure the port is a digit
	for(i = 0; i < strlen(port); i++){
		if(!isdigit(port[i])){
			return 1;
		}
	}

	//Check to make sure port is between 0 and 65535
	if((atoi(port) < 0) || (atoi(port) > 65535)){
		return 1;
	}
	
	return 0;
}

//Returns 1 if input is invalid or 0 if valid
int validate_input(char* input){
	
	int i;
	//Check to make sure input is a digit
	for(i = 0; i < (strlen(input) - 1); i++){
		if(!isdigit(input[i])){
			return 1;
		}
	}

	return 0;
}
