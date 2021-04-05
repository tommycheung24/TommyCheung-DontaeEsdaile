#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

void sendText(int socket,char* textName);
void sendHeader(int socket, short count, short sequenceNumber, int end);

struct Header{
	short count;
	short sequenceNumber;
};

int main(){

	struct Header header;
	char clientResponce[256];

	int serverSock, clientSock ;
	int queueLimit = 1;
	struct sockaddr_in servAddress, clientAddress; 

	//creates a socket, PF_INET = ipv4 protocol, SOCK_STREAM = connection based service, 0 = TCP
	serverSock = socket(PF_INET, SOCK_STREAM, 0);
	if(serverSock< 0){
		printf("socket() failed");
		return 0;
	}

	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(15044);
	servAddress.sin_addr.s_addr = INADDR_ANY;

	//binds the socket with the server Address
	if(bind(serverSock, (struct sockaddr *) &servAddress, sizeof(servAddress)) < 0){
		printf("bind() failed");
		return 0;
	}

	//listen for any connections
	if(listen(serverSock, queueLimit) < 0){
		printf("listen() failed");
		return 0;
	}

	//infinity waits for a connection
	for(;;){
		int clientLength= sizeof(clientAddress);	
		clientSock =accept(serverSock, (struct sockaddr *) &clientAddress, &clientLength);

		if(clientSock < 0){
			printf("accept() failed");
			return 0;
		} 
		break;
	}
	if(recv(cleintSock, header, sizeof(struct Header), 0) < 0){
		printf("recv() failed");
	}

	//recieves the filename
	if(recv(clientSock, clientResponce, sizeof(clientResponce), 0) < 0){
		printf("recv() failed");
		return 0;
	}

	sendText(clientSock, clientResponce);

	close(serverSock);

	return 0;
}

void sendText(int socket,char* textName){
	
	char line_buffer[80], confirm[1];

	FILE* file;
	file= fopen(textName, "r"); // read file with name textName

	short sequenceNumber = 1;
	int totalCount = 0;

	while(fgets(line_buffer, sizeof(line_buffer), file)){ // gets the a single line

		char newLine[strlen(line_buffer)];
		strcpy(newLine, line_buffer);
		short count = (short) sizeof(newLine);

		sendHeader(socket, count, sequenceNumber, 0);
		send(socket, newLine, sizeof(newLine), 0);

		++sequenceNumber;
		totalCount += count;
	}

	sendHeader(socket, 0, sequenceNumber, 1);
	send(socket, "", sizeof(""), 0);

	printf("Number of data packets transmitted: %d\n", sequenceNumber -1);
	printf("Total number of bytes transmitted: %d\n", totalCount);

	fclose(file);
}


void sendHeader(int socket,short count, short sequenceNumber, int end){
	struct Header header = {count, sequenceNumber};

	send(socket, &header, sizeof(struct Header), 0);
	
	if(end){
		printf("End of Transmission Packet with sequence number %d transmitted with %d data bytes\n", sequenceNumber, count);
	}else{
		printf("Packet %d is transmitted with %d data bytes\n", sequenceNumber, count);
	}

}
