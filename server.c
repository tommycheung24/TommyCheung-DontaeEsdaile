#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

int sendText(int socket,char* textName);
void sendHeader(int socket, short count, short sequenceNumber);

int main(){

	char server_message[256], clientResponce[256];

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
	servAddress.sin_port = htons(19044);
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

	//recieves the filename
	if(recv(clientSock, clientResponce, sizeof(clientResponce), 0) < 0){
		printf("recv() failed");
		return 0;
	}

	sendText(clientSock, clientResponce);

	close(serverSock);

	return 0;
}

int sendText(int socket,char* textName){

	char line[80];
	
	FILE* file;
	file= fopen(textName, "r"); // read file with name textName

	short sequenceNumber = 1;

	while(fgets(line, sizeof(line), file)){ // gets the a single line
		//the count, 
		short count = (short) sizeof(line);
		
		sendHeader(socket, count, sequenceNumber);
		send(socket, line, sizeof(line), 0);

		++sequenceNumber;
	}

	fclose(file);

	return 1;
}

void sendHeader(int socket, short count, short sequenceNumber){
	unsigned char header[4];

	header[0] = count >> 8;
	header[1] = count;
	header[2] = sequenceNumber >> 8;
	header[3] = sequenceNumber;

	//sends the header and prints the sequence nunber and number of data bytes
	send(socket, header, sizeof(header), 0);
	printf("Packet %d is transmitted with %d data bytes\n", sequenceNumber, count);

}

