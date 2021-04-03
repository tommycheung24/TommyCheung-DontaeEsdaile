#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

void storeText(int socket);

int main(){

	int clientSock;
	struct sockaddr_in serverAddress;

	char client_message[256], server_responce[256];

	//gets the name of the file
	printf("Enter file name: ");
	scanf("%s", &client_message);

	clientSock = socket(PF_INET, SOCK_STREAM, 0);  //creates a socket

	if(clientSock< 0){
		printf("socket() failed");
		return 0;
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(19044);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	int connection = connect(clientSock, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

	if(connection < 0){
		printf("connect() failed");
		return 0;
	}

	send(clientSock, client_message, sizeof(client_message), 0);

	storeText(clientSock);

	close(clientSock);
	return 0;
}

void storeText(int socket){
	
	char serverResponce[80];

	FILE * file;
	file = fopen("out.txt", "w");

	while(1){
		int r = recv(socket, serverResponce, sizeof(serverResponce), 0);
		if(r <= 0){
			break;
		}
		fprintf(file, serverResponce);
	}

	fclose(file);

}