#include <stdlib.h>
#include "Communication.h"

int sock;
static struct sockaddr_in server;
char* serverAddressIP;
char message[MSG_LEN];
int readSize; //citanje duzine bajtova u primeljenoj poruci 
int counter = 0;
int v = 0;


int CreateSocket()
{
	WSADATA wsaData;
	int i = 0;
	char ch;

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
		perror("Could not create client socket\n");
		WSACleanup();
		return -1;
	}

	// Initialize memory for address structure
	memset((char*)&server, 0, sizeof(server));

	//get the server IP
	printf("IP adress of receiver: \n");
	//prihvatanje karaktera sa tastature	
	while ((ch = (char)getchar()) != '\n')
	{
		*serverAddressIP = ch;
		serverAddressIP++;
	}
	*serverAddressIP = '\0';

	server.sin_addr.s_addr = inet_addr(serverAddressIP);
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORT);

	return 1;
}

int CloseSocket(int sock)
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

int ConnectToServer()
{
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Connection to  server failed\n");
		WSACleanup();
		return -1;
	}
	printf("Connected\n");

	return 1;
}

//funkcija za prijem poruka
int ReceiveMessage()
{

	while ((readSize = recv(sock, message, MSG_LEN, 0))>0)
	{
		// printf("%s\n",message);
		message[readSize] = '\0';
		puts(message);
		return 1;
	}

	perror("Transfer form  server failed \n");
	return -1;

}
//funkcija za generisanje iz terminala i slanje poruka
int SendMessageFun()
{
	int i = 0;
	char ch;
	//printf("Your answer: \n");
	//prihvatanje karaktera sa tastature	
	while ((ch = (char)getchar()) != '\n')
	{
		message[i] = ch;
		i++;
	}
	message[i] = '\0';

	if (send(sock, (char*)&message, strlen(message), 0) < 0)
	{
		perror("Send failed\n");
		WSACleanup();
		return -1;
	}
	return 1;
}
//funkcija za generisanje iz terminala i slanje poruka
int SendWindow(char* msg)
{
	
	if (send(sock,msg,strlen(msg), 0) < 0)
	{
		perror("Send failed\n");
		WSACleanup();
		return -1;
	}
	return 1;
}