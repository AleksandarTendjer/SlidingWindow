#ifndef _AUTOMAT_H_
#define _AUTOMAT_H_

#include <fsm.h>
#include <fsmsystem.h>
#include "Constants.h"

#include "../kernel/stdMsgpc16pl16.h"
typedef stdMsg_pc16_pl16 StandardMessage;

class ReceiverAuto : public FiniteStateMachine {
	enum ReceiverAutoStates { RECEIVER_IDLE,RECEIVER_RECEIVED };

	StandardMessage StandardMsgCoding;

	/* FiniteStateMachine abstract functions */
	MessageInterface *GetMessageInterface(uint32 id);
	void	SetDefaultHeader(uint8 infoCoding);
	void	SetDefaultFSMData();
	void	NoFreeInstances();
	uint8	GetMbxId();
	uint8	GetAutomate();

public:
	ReceiverAuto();
	~ReceiverAuto();
	bool windowMsg=true;
	int windowSize=0;
	int recvCount = 0;
	int sentCount = 0;
	void Initialize();
	void Start();
	void ChangeStateIdle();
	void ChangeStateReceived();
};

#endif /* _AUTOMAT_H */