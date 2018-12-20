#include "Communication.h"
#include "ReceiverAuto.h"

ReceiverAuto::ReceiverAuto() : FiniteStateMachine(RECEIVER_FSM, RECEIVER_MBX_ID, 5, 5, 2) {
}

ReceiverAuto::~ReceiverAuto() {
}


uint8 ReceiverAuto::GetAutomate() {
	return RECEIVER_FSM;
}

/* This function actually connnects the ReceiverAutoe with the mailbox. */
uint8 ReceiverAuto::GetMbxId() {
	return RECEIVER_MBX_ID;
}

MessageInterface *ReceiverAuto::GetMessageInterface(uint32 id) {
  if(id == 0) 
	  return &StandardMsgCoding;
  throw TErrorObject( __LINE__, __FILE__, 0x01010400);
}

void ReceiverAuto::SetDefaultHeader(uint8 infoCoding) {
	SetMsgInfoCoding(infoCoding);
	SetMessageFromData();
}

void ReceiverAuto::SetDefaultFSMData() {

}

void ReceiverAuto::NoFreeInstances() {
	printf("[%d] ReceiverAuto::NoFreeInstances()\n", GetObjectId());
}

void ReceiverAuto::Initialize() {
  SetState(RECEIVER_IDLE);
  InitEventProc(RECEIVER_IDLE, MSG_CHANGE_STATE, (PROC_FUN_PTR)&ReceiverAuto::ChangeStateIdle);
  InitEventProc(RECEIVER_RECEIVED, MSG_SENT_STATE, (PROC_FUN_PTR)&ReceiverAuto::ChangeStateReceived);
  SetDefaultFSMData();
}


void ReceiverAuto::ChangeStateIdle() {
	//implement receiving messages and packet loss
	char buffer[MSG_LEN];
	if (windowMsg == true)
	{
		ReceiveWindow(buffer);
		ReceiverAuto:windowMsg = false;
	}

	while (true)
	{

	}

	SetState(RECEIVER_RECEIVED);
}
void ReceiverAuto::ChangeStateReceived() {
	//receive sent messages count and compare to the received count
	SetState(RECEIVER_IDLE);
}

/* Initial system message */
void ReceiverAuto::Start() {
	// sending a message to itself so that the Receiver Automate can change state   
	PrepareNewMessage(0x00, MSG_CHANGE_STATE);
	SetMsgToAutomate(RECEIVER_FSM);
	SetMsgObjectNumberTo(1);
	SendMessage(RECEIVER_MBX_ID);
}
