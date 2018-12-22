#pragma once
#pragma once
#include <stdio.h>
#include <conio.h>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Constants.h"

//bool msgIsWindow=true ;


int CreateSocket();
int  ListenConnection();
int AcceptConnection();
int BindSocket();
int CloseSocket();
int ConnectToServer();
int ReceiveMessage();
int SendMessageFun();
int ReceiveWindow(char *);