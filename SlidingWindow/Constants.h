#pragma once


#define SENDER_MBX_ID    0
#define SENDER_FSM       0
#define MSG_CHANGE_STATE	  0x00
#define MSG_SENT_STATE		  0x01
#define MSG_LEN				  15
#define DEFAULT_PORT		  80					//protocol over wich we will be sending messages
#define SERVER_PORT			  27015					// Port number of server that will be used for communication with clients
#define BUFFER_SIZE			  30						// Size of buffer that will be used for sending and receiving messages to client
