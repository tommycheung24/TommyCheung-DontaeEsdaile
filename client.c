#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

void storeText(int socket);
void sendHeader(int socket, unsigned short count);

int main(){

	int clientSock;
	struct sockaddr_in serverAddress;

	char client_message[256], confirm[1];

	//gets the name of the file from user
	printf("Enter file name: ");
	scanf("%s", &client_message);
	
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

	//sends the header, with the size of the filename and sequence number 0
	sendHeader(clientSock, (unsigned short) sizeof(client_message));

	recv(clientSock, confirm, sizeof(confirm), 0);
	//sends the file name to server
	send(clientSock, client_message, sizeof(client_message), 0);

	//store incoming data for file
	storeText(clientSock);

	close(clientSock);
	return 0;
}

void storeText(int socket){

	char serverResponce[80];
	unsigned char header[4];

	int totalCount = 0;
	int totalPacket = 0;

	FILE * file;
	file = fopen("out.txt", "w"); //write to file out.txt

	int headerCount;
	int dataCount;

	while(1){
		//clears out the char array
		bzero(header, 4);
		//recieves the header
		recv(socket, header, sizeof(header), 0);
		
		//reassemble the char array into two shorts(count and sequence number)
		unsigned short count = header[0] + (header[1] << 8);
		unsigned short seq = header[2] + (header[3] << 8);

		//sends a blank message to server to signal to move on
		send(socket, "", sizeof(""), 0);

		//clears out the char array
		bzero(serverResponce, 80);
		//recieves the data
		dataCount = recv(socket, serverResponce, sizeof(serverResponce), 0);

		//if the length of the data is 0 and count is zero, then it's the end of transmission
		if(!strlen(serverResponce) && (count == 0)){
			printf("End of Transmission Packet with sequence number %d received with %d data bytes\n", seq, count);
			break;
		}
		//puts the data into the file
		fputs(serverResponce, file);
		//update value
		totalCount += count;
		++totalPacket;
		printf("Packet %d received with %d data bytes\n", seq, count);		
		
		//signal to move on
		send(socket, "", sizeof(""), 0);

	}
	//close file
	fclose(file);

	printf("Number of data packets received: %d\n", totalPacket);
	printf("Number of data bytes received: %d\n", totalCount);

}

void sendHeader(int socket, unsigned short count){
	unsigned char header[4];
	unsigned short seq = 0;

	//dissambles count and sequence number into a 4 bytes char array
	header[0] = count;
	header[1] = count >> 8;
	header[2] = seq;
	header[3] = seq >> 8;

	send(socket, header, sizeof(header), 0);

}