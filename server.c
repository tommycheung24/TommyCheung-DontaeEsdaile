#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

int main(){

	char server_message[256] = "You Made it bitch";

	int serverSock;
	int queueLimit = 1;
	struct sockaddr_in servAddress;  //the value of server
	struct sockaddr_in clientAddress; //the value of the client, used in accept

	serverSock = socket(PF_INET, SOCK_STREAM, 0);

	if( serverSock< 0){ 
		printf("socket() failed");
		return 0;
	}

	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(9002);
	servAddress.sin_addr.s_addr = INADDR_ANY;

	if(bind(serverSock, (struct sockaddr *) &servAddress, sizeof(servAddress)) < 0){
		printf("bind() failed");
		return 0;
	}

	if(listen(serverSock, queueLimit) < 0){
		printf("listen() failed");
		return 0;
	}

	for(;;){

		int clientLength = sizeof(clientAddress);
		
		int clientSock= accept(serverSock, (struct sockaddr *) &clientAddress, &clientLength);

		if(clientSock< 0){
			printf("accept() failed");
			return 0;
		} 
		printf("%u\n", clientAddress.sin_addr.s_addr);

		send(clientSock, server_message, sizeof(server_message), 0);

		break;
	}

	close(serverSock);

	return 0;
}