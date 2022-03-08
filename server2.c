
////////////////////////////////////
//Server code
///////////////////////////////////
//This file is the code for the server
#include <netinet/in.h>  // for sockaddr_in
#include <sys/types.h>  // for socket
#include <sys/socket.h>  // for socket
#include <stdio.h>    // for printf
#include <stdlib.h>    // for exit
#include <string.h>    // for bzero
#include <time.h>        //for time_t and time
#define HELLO_WORLD_SERVER_PORT 7754
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
int main(int argc, char **argv)
{
//Set a socket address structure server_addr, representing the server Internet address, port
struct sockaddr_in server_addr;
bzero(&server_addr,sizeof(server_addr)); //Set all the contents of a section of memory to 0
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htons(INADDR_ANY);
server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
// time_t now;
FILE *stream;
//Create a streaming protocol (TCP)socket for the Internet, with server_socket representing the server socket
int server_socket = socket(AF_INET,SOCK_STREAM,0);
if( server_socket < 0)
{
printf("Create Socket Failed!");
exit(1);
}
//Connect the socket to the socket address structure
if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
{
printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT);
exit(1);
}
//Server_socket is used for listening
if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
{
printf("Server Listen Failed!");
exit(1);
}
while (1) //The server side runs all the time
{
struct sockaddr_in client_addr;
socklen_t length = sizeof(client_addr);
int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
if ( new_server_socket < 0)
{
printf("Server Accept Failed!n");
break;
}
char buffer[BUFFER_SIZE];
bzero(buffer, BUFFER_SIZE);
strcpy(buffer,"Hello,World!  From the server! ");
strcat(buffer,"n"); //C string concatenation
send(new_server_socket,buffer,BUFFER_SIZE,0);
bzero(buffer,BUFFER_SIZE);
//Receives information sent by the client to the buffer
length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
if (length < 0)
{
printf("Server Recieve Data Failed!n");
exit(1);
}
printf("n%s",buffer);
if((stream = fopen("data1","r"))==NULL)
{
printf("The file 'data1' was not opened! n");
exit(1);
}
else
printf("The file 'filename' was opened! n");
bzero(buffer,BUFFER_SIZE);
int lengsize = 0;
while((lengsize = fread(buffer,1,1024,stream)) > 0)
{
printf("lengsize = %dn",lengsize);
if(send(new_server_socket,buffer,lengsize,0)<0)
{
printf("Send File is Failedn");
break;
}
bzero(buffer, BUFFER_SIZE);
}
if(fclose(stream))
printf("The file 'data' was not closed! n");
exit(1);    
//Close the connection to the client
close(new_server_socket);    
}
//Close the socket for listening
close(server_socket);
return 0;
}

