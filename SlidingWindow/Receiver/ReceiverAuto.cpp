#include "Communication.h"
#include "ReceiverAuto.h"
extern bool fsmEnd = false;
bool firstPass = true;
char buffer[MSG_LEN];
int percentage;
int windowCount = 0;
int sentCountSender = 0;

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
	if (id == 0)
		return &StandardMsgCoding;
	throw TErrorObject(__LINE__, __FILE__, 0x01010400);
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
	InitEventProc(RECEIVER_RECEIVED, MSG_RECEIVED_STATE, (PROC_FUN_PTR)&ReceiverAuto::ChangeStateReceived);
	SetDefaultFSMData();
}


void ReceiverAuto::ChangeStateIdle() {
	//implement receiving messages and packet loss
	if (firstPass)
	{
		if (ReceiverAuto::windowMsg == true)
		{
			ReceiveMsg(buffer);
		ReceiverAuto:windowMsg = false;
		}
		windowSize = atoi(buffer);
		//percentage of data loss
		//every &percentage packet will be lost
		//probability=number of packets we want to lose,
		//every percentage packet will be lost in the window
		percentage = round(windowSize / probability);
		ReceiveMsg(buffer);
		//receive windowCount
		windowCount = atoi(buffer);
		ReceiverAuto::sentCount++;
		
	}
	if (windowCount == 0)
	{
		fsmEnd = true;
		SysSetLogFlag(LOG_END);
	}

	//if percentage is 
	if (!fsmEnd)
		if ((ReceiverAuto::sentCount%percentage) != 0)
		{
			if (firstPass == true)
			{
				ReceiverAuto::sentCount = 0;
				firstPass = false;
			}
			//receiving the message
			ReceiveMsg(buffer);
			ReceiverAuto::sentCount++;
			SetState(RECEIVER_RECEIVED);
			// sending a message to itself so that the Receiver Automate can change state
			PrepareNewMessage(0x00, MSG_RECEIVED_STATE);
			SetMsgToAutomate(RECEIVER_FSM);
			SetMsgObjectNumberTo(0);
			SendMessage(RECEIVER_MBX_ID);

		}
		else //get back to idle state
		{
			ReceiverAuto::sentCount++;
			SetState(RECEIVER_IDLE);
			// sending a message to itself so that the Receiver Automate can change state
			PrepareNewMessage(0x00, MSG_CHANGE_STATE);
			SetMsgToAutomate(RECEIVER_FSM);
			SetMsgObjectNumberTo(0);
			SendMessage(RECEIVER_MBX_ID);

		}
}
void ReceiverAuto::ChangeStateReceived() {

	//the packet is received 
	++ReceiverAuto::recvCount;
	ReceiveMsg(buffer);
	//same as the 
	sentCountSender = atoi(buffer);
	ReceiverAuto::difference = ReceiverAuto::sentCount - ReceiverAuto::recvCount;

	if (ReceiverAuto::sentCount % 12 == 0)
		--windowCount;

	//receive sent messages count and compare to the received count
	SetState(RECEIVER_IDLE);
	PrepareNewMessage(0x00, MSG_CHANGE_STATE);
	SetMsgToAutomate(RECEIVER_FSM);
	SetMsgObjectNumberTo(0);
	SendMessage(RECEIVER_MBX_ID);
}

/* Initial system message */
void ReceiverAuto::Start() {
	// sending a message to itself so that the Receiver Automate can change state   
	PrepareNewMessage(0x00, MSG_CHANGE_STATE);
	SetMsgToAutomate(RECEIVER_FSM);
	SetMsgObjectNumberTo(0);
	SendMessage(RECEIVER_MBX_ID);
}
