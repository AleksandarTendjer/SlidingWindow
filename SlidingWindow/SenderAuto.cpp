#include <stdio.h>
#include "SenderAuto.h"


SenderAuto::SenderAuto() : FiniteStateMachine(AUTOEXAMPLE_FSM, AUTOEXAMPLE_MBX_ID, 5, 5, 2) {
}

SenderAuto::~SenderAuto() {
}


uint8 SenderAuto::GetAutomate() {
	return AUTOEXAMPLE_FSM;
}

/* This function actually connnects the AutoExamplee with the mailbox. */
uint8 SenderAuto::GetMbxId() {
	return AUTOEXAMPLE_MBX_ID;
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
	printf("SenderAuto[%d]::ChangeStateIdle() - receive message !\n", GetObjectId());
	///procesing function(preparing messages,sending messages)
	//set state to SENDER_SENT

	SetState(SENDER_SENT);
}

void SenderAuto::Initialize() {
	//se default state in IDLE 
  SetState(SENDER_IDLE);
  //all event change 
  InitEventProc(SENDER_IDLE, MSG_CHANGE_STATE,(PROC_FUN_PTR)&SenderAuto::ChangeStateIdle);
  SetDefaultFSMData();
}

/* Initial system message */
void SenderAuto::Start() {
}
