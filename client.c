#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

void storeText(int socket);


struct header{
	short sequence_number;
	short count;

};

int main(){

	int clientSock;
	struct sockaddr_in serverAddress;
	struct header sendHeader;

	char client_message[256];

	//gets the name of the file
	printf("Enter file name: ");
	scanf("%s", &client_message);

	sendHeader.sequence_number = 0;
	sendHeader.count = sizeof(client_message);
	
	clientSock = socket(PF_INET, SOCK_STREAM, 0);  //creates a socket

	if(clientSock< 0){
		printf("socket() failed");
		return 0;
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(15060);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	int connection = connect(clientSock, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

	if(connection < 0){
		printf("connect() failed");
		return 0;
	}

	send(clientSock, &sendHeader, sizeof(struct header), 0);
	send(clientSock, client_message, sizeof(client_message), 0);

	storeText(clientSock);

	close(clientSock);
	return 0;
}

void storeText(int socket){

	char serverResponce[80];

	int totalCount = 0;
	int totalPacket = 0;

	FILE * file;
	file = fopen("out.txt", "w"); //write to file out.txt


	//recieved struct
	struct header recv_header;

	// header fields
	int recv_sequence_number = 0;
	int recv_count = 0;
	int headerCount;
	int dataCount;

	while(1){
		//if(headerCount = recv(socket, &recv_header, sizeof(struct header), 0) < 0){
		//	printf("header() recv() failed");
		//}
		dataCount = recv(socket, serverResponce, sizeof(serverResponce), 0);

		if(!dataCount){
			break;
		}

		fputs(serverResponce, file);
		totalCount += dataCount;
		++totalPacket;

		printf("Packet %d received with %d data bytes\n", totalPacket,dataCount);
		
		//int headerCount = recv(socket, header, sizeof(header), 0);
		
		send(socket, "", sizeof(""), 0);
	}
	
	fclose(file);

	printf("Number of data packets received: %d\n", totalPacket);
	printf("Number of data bytes received: %d\n", totalCount);

}