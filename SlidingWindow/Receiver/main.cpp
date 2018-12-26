#include "Communication.h"
#include "fsmsystem.h"
#include "logfile.h"
#include "ReceiverAuto.h"


/* FSM system instance. */
static FSMSystem sys(1 /* max number of automates types */, 1 /* max number of msg boxes */);
int probabilityLoc;
int sentCountLoc;
int recvCountLoc;
extern bool  fsmEnd;
DWORD WINAPI SystemThread(void *data) {
	ReceiverAuto automate;

	/* Kernel buffer description block */
	/* number of buffer types */
	const uint8 buffClassNo =  4; 
	/* number of buffers of each buffer type */
	uint32 buffsCount[buffClassNo] = { 50 , 50, 50, 10 };
	/* buffer size for each buffer type */
 	uint32 buffsLength[buffClassNo] = { 128, 256, 512, 1024 };
	
	/* Logging setting - to a file in this case */
	LogFile lf("log.log" /*log file name*/, "./log.ini" /* message translator file */);
	LogAutomateNew::SetLogInterface(&lf);
	automate.probability = probabilityLoc;

	/* Mandatory kernel initialization */
	printf("[*] Initializing system...\n");
	sys.InitKernel(buffClassNo, buffsCount, buffsLength, 5);

	/* Add automates to the system */
	sys.Add(&automate, RECEIVER_FSM, 1 /* the number of automates that will be added */, true);

	/* Start the first automate - usually it sends the first message, 
	since only automates can send messages */
	automate.Start();

	/* Starts the system - dispatches system messages */
	printf("[*] Starting system...\n");
	sys.Start();

	//ouput details
	sentCountLoc=automate.sentCount;
	recvCountLoc=automate.recvCount;
	probabilityLoc=automate.probability;
	/* Finish thread */
	return 0;
}

int main(int argc, char *argv[]) {
	DWORD thread_id;
	HANDLE thread_handle;
	if (argv[1] != NULL)
	{
		probabilityLoc = atoi(argv[1]);
	}
	else
	{
		probabilityLoc = 3;
	}
	///////////////////////////////////////////////Creating a server socket,listening and accepting////////////////////////////////
	if (CreateSocket() < 0)
	{
		return -1;
	}
	if (BindSocket() < 0)
	{
		return -1;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/* Start operating thread. */
	thread_handle = CreateThread(NULL, 0, SystemThread, NULL, 0, &thread_id);

	/* Wait for keypress. */
	//getch();
	/* wait for the program to finish */
	while (!fsmEnd)
	{
	}
	/* Notify the system to stop - this causes the thread to finish */
	printf("[*] Stopping system...\n");
	sys.StopSystem();
	printf("Received messages count:%d \n",recvCountLoc);
	printf("Sent messages count:%d \n", sentCountLoc);
	printf("Probability of packet loss: \n", probabilityLoc);

	/* Free the thread handle */
	CloseHandle(thread_handle);
//just to see the output
	getch();
	return 0;
}
