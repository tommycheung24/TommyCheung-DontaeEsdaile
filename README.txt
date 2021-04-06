# TommyCheung-DontaeEsdaile

Github Link : https://github.com/tommycheung24/TommyCheung-DontaeEsdaile/tree/f7338df6e1af7c8f83e5df742bb2d37c78ad82cd

- List of files:
client.c 
server.c
README.TXT

- Compilation:
Prepare file in server (s1) location.

gcc -o server.c s1
gcc -o client.c c1

- Running:


- Server: 
On execution of "s1", the server will first create a socket and then binds with the server address. After that the server will wait for any connections. After a connection is made it will connect to the client. The server will recieve the header then it will then recieve the file-name from the client is data. The server will then read the file get a single line and store it in a buffer. Unused space will be removed from the buffer before sending it. The server will then send the header to the client and then wait for a confirm before moving on. After rececing the confirmation the first line of the file will be sent to the client. After the first line is sent the client in a packet the, server will increment the sequence number and keep track of all sent bytes. The server will then wait for a confimation from the client before moving on. After the confirmation is received the server will repeat this process for each line in the file and send them in following packets. After all lines are sent in the file, the end-of-transmission packet will be sent to the client. The server will then print the number of data packets and the total number of bytes transmitted. When the file transfer is complete the server will close connection and terminate execution.

Running: 
To run the server executable you open another terminal and run "./s1" with no arguments after compliation.

- Client:
On execution of c1, the client will create a socket and connect to the server. The client executable will first ask for the name of the file to be transferred and then establishes a TCP connection with the server. After the connection is established the client will send the header with the size of the filename and a sequence number of 0. After that the client will send the filename to the server. After the server recieves the packet it will send the file line by line in a series of packets of headers and data, to the client and stores it in out.txt. It will also send confirmation packets(empty packets) to server after every header and data received. When the file transfer and the ETO is sent to the client, it will close connection and terminate execution.

Running: 
To run the client executable you open a seperate terminal and run "./c1" with no arguments after complilation. The client will then prompt the user to enter the file-name.

