#include "Communication.h"
#include "SenderAuto.h"

uint16 windowSize;
char dataBuffer[25][BUFFER_SIZE];
char string[30];
extern bool fsmEnd = false;
uint16 windowCount;
uint16 msgCount;
double lastWindow;
bool firstPass = true;
int i = 0;
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
	if (id == 0)
		return &StandardMsgCoding;
	throw TErrorObject(__LINE__, __FILE__, 0x01010400);
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
	int j = 0;
	FILE* f;
	
	//calling sending message function,all connection to the server has alredy been configurated
	if (firstPass)
	{
		i = 0;
		/////////////////////////////////Sending the window Size/////////////////////////////////////////////////
		printf("Window size: \n");
		scanf("%s", string);

		//set window size
		windowSize = (uint16)atoi(string);
		//send window size to the reciever
		Send(string);
		///////////////////////////////////////Sending the packets////////////////////////////////////////////////////////
		//reading input from file
		 f = fopen("Message.txt", "r");
		if (f == NULL)
		{
			printf("Could not open file!");
		}

		//read from file and put it the buffer
		while ((dataBuffer[i][j] = fgetc(f))!= EOF)
		{
			
			if (dataBuffer[i][j] == '\n')
			{
				//move to the next message
				i++;
				j = 0;
			}
			else
			{
				//move to the next character inside the message 
				j++;
			}
		}
		//close the file safely
		fclose(f);
		msgCount = i;
		lastWindow = i%windowSize;
		//if the division residue is > 0 than there should be a plus one window for those messages 
		if (lastWindow > 0)
		{
			windowCount = round(i / windowSize) + 1;
		}
		else
		{
			windowCount = round(i / windowSize);
			//last window is the same size as the windowSize
			lastWindow = windowSize;
		}
		//send window count to receiver
		sprintf(string,"%d",windowCount);
		Send(string);
		firstPass = false;
		i = 0;
	}

	//because the messages go from one to the end of the file,and the windowCount goes in the oposite direction
	switch (windowCount)
	{
	default:
	{
		if (SenderAuto::sentCount < windowSize)
		{
			Send(dataBuffer[i]);
			++i;
			break;
		}
		else {
			//the window is sent
			SenderAuto::sentCount = 0;
			//++i;
			--windowCount;
		}

	}
	case(1):
	{
		//for the last window
		if (SenderAuto::sentCount < lastWindow)
		{
			Send(dataBuffer[i]);
			++i;
		}
		else {
			//the window is sent
			SenderAuto::sentCount = 0;
			--windowCount;
		}
		break;
	}
	case(0):
	{
		//exit the fsm
		fsmEnd = true;
		break;
	}
	}
	if (!fsmEnd)
	{
		PrepareNewMessage(0x00, MSG_SENT_STATE);
		SetMsgToAutomate(SENDER_FSM);
		SetMsgObjectNumberTo(0);
		SendMessage(SENDER_MBX_ID);
		SetState(SENDER_SENT);
	}
	else
	{
		SysSetLogFlag(LOG_END);
	}
}
void SenderAuto::ChangeStateSent() {

	printf("SenderAuto[%d]::ChangeStateSent()  !\n", GetObjectId());
	SenderAuto::sentCount++;
	//send message to 
	Send(itoa(SenderAuto::sentCount, dataBuffer[0], 10));
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
	InitEventProc(SENDER_IDLE, MSG_CHANGE_STATE, (PROC_FUN_PTR)&SenderAuto::ChangeStateIdle);
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
