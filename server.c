#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

void sendText(int socket,char* textName);
void sendHeader(int socket, unsigned short count, unsigned short sequenceNumber, int end);

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
	servAddress.sin_port = htons(15060);
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

void sendText(int socket,char* textName){
	
	//line_buffer is the line in the file, confirm is the response from client 
	char line_buffer[80], confirm[1];

	FILE* file;
	file= fopen(textName, "r"); // read file with name textName

	unsigned short sequenceNumber = 1;
	int totalCount = 0;

	//gets a single line in the file and stores it in line_buffer
	while(fgets(line_buffer, sizeof(line_buffer), file)){

		//cuts the unused space away before sending the data
		char newLine[strlen(line_buffer)];
		strcpy(newLine, line_buffer);
		
		//gets the size in bytes of the new char array
		unsigned short count = (unsigned short) sizeof(newLine);
		
		//sends the header with info, count and sequence number
		sendHeader(socket, count, sequenceNumber, 0);
		//recieves a confirmation from client so the server don't move on until the client is ready
		recv(socket, confirm, sizeof(confirm), 0);
		//sends the actually data after the server recieves confirmation
		send(socket, newLine, sizeof(newLine), 0);

		//add 1 to sequence number, add count to totalCount(total bytes send)
		++sequenceNumber;
		totalCount += count;

		//waits again for confirmation to move on from client
		recv(socket, confirm, sizeof(confirm), 0);
	}

	//since it's out of loop it means that there's no more lines to grab from file
	//time to send the End of Transmission packet
	sendHeader(socket, 0, sequenceNumber, 1);
	//sends an empty data 
	send(socket, "", sizeof(""), 0);

	printf("Number of data packets transmitted: %d\n", sequenceNumber -1);
	printf("Total number of bytes transmitted: %d\n", totalCount);

	fclose(file);
}

void sendHeader(int socket,unsigned short count, unsigned short sequenceNumber, int end){
	unsigned char header[4];

	//putting the two shorts into a 4 bytes char array 
	header[0] = count;
	header[1] = count >> 8;
	header[2] = sequenceNumber;
	header[3] = sequenceNumber >> 8;

	send(socket, header, sizeof(header), 0);
	
	//end means if it is an End of Transmission Packet
	if(end){
		printf("End of Transmission Packet with sequence number %d transmitted with %d data bytes\n", sequenceNumber, count);
	}else{
		printf("Packet %d is transmitted with %d data bytes\n", sequenceNumber, count);
	}
}

