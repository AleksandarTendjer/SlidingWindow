#ifndef _AUTOMAT_H_
#define _AUTOMAT_H_

#include <fsm.h>
#include <fsmsystem.h>

#define AUTOEXAMPLE_MBX_ID    0
#define AUTOEXAMPLE_FSM       0
#define MSG_CHANGE_STATE		0x00


#include "../kernel/stdMsgpc16pl16.h"
typedef stdMsg_pc16_pl16 StandardMessage;

class SenderAuto : public FiniteStateMachine {
	enum SenderAutoStates { SENDER_IDLE,SENDER_SENT };

	StandardMessage StandardMsgCoding;

	/* FiniteStateMachine abstract functions */
	MessageInterface *GetMessageInterface(uint32 id);
	void	SetDefaultHeader(uint8 infoCoding);
	void	SetDefaultFSMData();
	void	NoFreeInstances();
	uint8	GetMbxId();
	uint8	GetAutomate();
	
public:
	SenderAuto();
	~SenderAuto();
	void ChangeStateIdle();
	void Initialize();
	void Start();
};

#endif /* _AUTOMAT_H */