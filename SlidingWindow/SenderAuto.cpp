#include "Communication.h"
#include "SenderAuto.h"

int windowSize;
char* dataBuffer[20];

SenderAuto::SenderAuto() : FiniteStateMachine(SENDER_FSM, SENDER_MBX_ID, 5, 5, 2) {
}

SenderAuto::~SenderAuto() {
}


uint8 SenderAuto::GetAutomate() {
	return SENDER_FSM;
}

/* This function actually connnects the AutoExamplee with the mailbox. */
uint8 SenderAuto::GetMbxId() {
	return SENDER_MBX_ID;
}

MessageInterface *SenderAuto::GetMessageInterface(uint32 id) {
  if(id == 0) 
	  return &StandardMsgCoding;
  throw TErrorObject( __LINE__, __FILE__, 0x01010400);
}

void SenderAuto::SetDefaultHeader(uint8 infoCoding) {
	SetMsgInfoCoding(infoCoding);
	SetMessageFromData();
}

void SenderAuto::SetDefaultFSMData() {

}

void SenderAuto::NoFreeInstances() {
	printf("[%d] SenderAuto::NoFreeInstances()\n", GetObjectId());
}

void SenderAuto::ChangeStateIdle() {
	int i = 0;
	//calling sending message function,all connection to the server has alredy been configurated
	char ch;
	/////////////////////////////////Sending the window Size/////////////////////////////////////////////////
	printf("Window size: \n");
	//dynamically allocate the memory for the first string
	dataBuffer[0] = (char*)malloc(BUFFER_SIZE * sizeof(char));
	//prihvatanje karaktera sa tastature	
	while ((ch = (char)getchar()) != '\n')
	{
		*dataBuffer[0] = ch;
		 dataBuffer[0]++;
	}

	*dataBuffer[0]= '\0';
	//set window size
	windowSize = atoi(dataBuffer[0]);
	//send window size to the reciever
	Send(dataBuffer[0]);
	///////////////////////////////////////Sending the packets////////////////////////////////////////////////////////
	//reading input from file
	FILE* f = fopen("Message.txt", "r");
	if (f == NULL)
	{
		printf("Could not open file!");
	}

	//read from file and put it the buffer
	while ((fgets(dataBuffer[i], 100, f) != NULL) && (*dataBuffer[i] != '\n'))
	{
		++i;
		//dynamically alocate the memory for all other strings except the first one
		dataBuffer[i]=(char*)malloc(BUFFER_SIZE*sizeof(char));
	}
	i = 0;
	//send the messages
	while (SenderAuto::sentCount < windowSize)
	{
		Send(dataBuffer[i]);
		//dealloc the memory 
		free(dataBuffer[i]);
		++i;
	}
	//printf("SenderAuto[%d]::ChangeStateIdle() - receive message !\n", GetObjectId());
	//send message so that initEventProc can be executed
	PrepareNewMessage(0x00, MSG_SENT_STATE);
	SetMsgToAutomate(SENDER_FSM);
	SetMsgObjectNumberTo(0);
	SendMessage(SENDER_MBX_ID);
	SetState(SENDER_SENT);
}
void SenderAuto::ChangeStateSent() {

	printf("SenderAuto[%d]::ChangeStateSent() - receive message !\n", GetObjectId());
	SenderAuto::sentCount++;
	Send(itoa(SenderAuto::sentCount,dataBuffer[0],10));	
	//send message so that initEventProc can be executed
	PrepareNewMessage(0x00, MSG_CHANGE_STATE);
	SetMsgToAutomate(SENDER_FSM);
	SetMsgObjectNumberTo(0);
	SendMessage(SENDER_MBX_ID);
	SetState(SENDER_IDLE);

}

void SenderAuto::Initialize() {
	//se default state in IDLE 
  SetState(SENDER_IDLE);
  //all event change 
  InitEventProc(SENDER_IDLE, MSG_CHANGE_STATE,(PROC_FUN_PTR)&SenderAuto::ChangeStateIdle);
  InitEventProc(SENDER_SENT, MSG_SENT_STATE, (PROC_FUN_PTR)&SenderAuto::ChangeStateSent);
  SetDefaultFSMData();
}

/* Initial system message */
void SenderAuto::Start() {

	// sending a message to itself so that the Sender Automate can change state   
	PrepareNewMessage(0x00, MSG_CHANGE_STATE);
	SetMsgToAutomate(SENDER_FSM); 
	SetMsgObjectNumberTo(0); 
	SendMessage(SENDER_MBX_ID);
}
