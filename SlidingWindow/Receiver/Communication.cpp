#include <stdlib.h>
#include "Communication.h"

int sock;
int clientSock;
static struct sockaddr_in server,client;
char message[MSG_LEN];
int readSize; //citanje duzine bajtova u primeljenoj poruci 
int counter = 0;
int v = 0;


int CreateSocket()
{
	WSADATA wsaData;
	int i = 0;
	char *ch;

	// Initialize windows sockets for this process
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Check if library is succesfully initialized
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//IPv4 family,//socket datatgram,IP UDP protocol
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == -1)
	{
		perror("Could not create server socket\n");
		WSACleanup();
		return -1;
	}

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORT);

	return 1;
}

int CloseSocket()
{
	if (closesocket(sock) < 0)
	{
		perror("Could not close client socket\n");
		WSACleanup();
		return -1;
	}
	WSACleanup();
	return 1;
}

int  ListenConnection()
{
	if (listen(sock, 1) < 0)
	{
		perror("Listen connection from Client is not possible\n");
		return -1;
	}

	return 1;
}
//binding a scoket with the ip adress
int BindSocket()
{
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("bind failed. Error");
		return -1;
	}
	puts("bind done");
	return 1;
}

int AcceptConnection()
{
	int c;
	puts("Waiting for incomming connnections ...");
	c = sizeof(struct sockaddr_in);

	//accept connection from an incoming client
	clientSock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c);
	if (clientSock < 0)
	{
		perror("accept failed");
		return -1;
	}
	puts("Connection accepted");
	return 1;
}


//funkcija za prijem poruka
int ReceiveMessage()
{

	while ((readSize = recv(sock, message, MSG_LEN, 0))>0)
	{
		message[readSize] = '\0';
		puts(message);
		return 1;
	}

	perror("Transfer form  sender failed \n");
	return -1;

}
//funkcija za prijem poruka
int ReceiveMsg(char *msg)
{

	while ((readSize = recv(sock, msg, MSG_LEN, 0))>0)
	{
		msg[readSize] = '\0';
		puts(msg);
		return 1;
	}

	perror("Transfer form  sender failed \n");
	WSACleanup();
	return -1;

}
