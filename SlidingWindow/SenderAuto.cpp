#include "Communication.h"
#include "SenderAuto.h"

int windowSize;
char dataBuffer[BUFFER_SIZE];

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

	//calling sending message function,all connection to the server has alredy been configurated
	int i = 0;
	char ch;
	int sent = 0;

	printf("Window size: \n");

	//prihvatanje karaktera sa tastature	
	while ((ch = (char)getchar()) != '\n')
	{
		dataBuffer[i] = ch;
		 i++;
	}
	dataBuffer[i]= '\0';
	//set window size
	windowSize = atoi(dataBuffer);
	//send window size to the reciever
	Send(dataBuffer);

	while (SenderAuto::sentCount < windowSize)
	{
		printf("message: \n");
		SendMessageFun();
	}
	

	//printf("SenderAuto[%d]::ChangeStateIdle() - receive message !\n", GetObjectId());
	//send message so that initEventProc can be executed
	PrepareNewMessage(0x00, MSG_SENT_STATE);
	SetMsgToAutomate(SENDER_FSM);
	SetMsgObjectNumberTo(1);
	SendMessage(SENDER_MBX_ID);
	SetState(SENDER_SENT);
}
void SenderAuto::ChangeStateSent() {

	printf("SenderAuto[%d]::ChangeStateSent() - receive message !\n", GetObjectId());
	SenderAuto::sentCount++;
	Send(itoa(SenderAuto::sentCount,dataBuffer,10));	
	//send message so that initEventProc can be executed
	PrepareNewMessage(0x00, MSG_CHANGE_STATE);
	SetMsgToAutomate(SENDER_FSM);
	SetMsgObjectNumberTo(1);
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
	SetMsgObjectNumberTo(1); 
	SendMessage(SENDER_MBX_ID);
}
