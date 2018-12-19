#include "Communication.h"
#include "SenderAuto.h"
//extern bool msgIsWindow;
int windowSize;

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
	char dataBuffer[BUFFER_SIZE];
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
	SendWindow(dataBuffer);

	while (sent < windowSize)
	{
		printf("message: \n");
		SendMessageFun();
		++sent;
	}
	
	//prihvatanje karaktera sa tastature	
	printf("SenderAuto[%d]::ChangeStateIdle() - receive message !\n", GetObjectId());
	SetState(SENDER_SENT);
}
void SenderAuto::ChangeStateSent() {

	printf("SenderAuto[%d]::ChangeStateSent() - receive message !\n", GetObjectId());
	SetState(SENDER_IDLE);

}

void SenderAuto::Initialize() {
	//se default state in IDLE 
  SetState(SENDER_IDLE);
  //all event change 
  InitEventProc(SENDER_IDLE, MSG_CHANGE_STATE,(PROC_FUN_PTR)&SenderAuto::ChangeStateIdle);
  InitEventProc(SENDER_SENT, MSG_CHANGE_STATE, (PROC_FUN_PTR)&SenderAuto::ChangeStateSent);
  SetDefaultFSMData();
}

/* Initial system message */
void SenderAuto::Start() {

	//slanje poruke za kraj promene stanja 
	PrepareNewMessage(0x00, MSG_CHANGE_STATE);
	SetMsgToAutomate(SENDER_FSM); 
	SetMsgObjectNumberTo(1); 
	SendMessage(SENDER_MBX_ID);
}
