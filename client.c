#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>


int main(){

	int clientSock;
	struct sockaddr_in clientAddress;

	clientSock = socket(PF_INET, SOCK_STREAM, 0);  //creaetes a socket

	if(clientSock< 0){
		printf("socket() failed");
		return 0;
	}

	clientAddress.sin_family = AF_INET;
	clientAddress.sin_port = htons(9002);
	clientAddress.sin_addr.s_addr = INADDR_ANY;

	int connection = connect(clientSock, (struct sockaddr *) &clientAddress, sizeof(clientAddress));

	if(connection < 0){
		printf("connect() failed");
		return 0;
	}

	char server_responce[256];

	recv(clientSock, &server_responce, sizeof(server_responce), 0);

	printf("%s",server_responce);

	close(clientSock);
	

	return 0;
}