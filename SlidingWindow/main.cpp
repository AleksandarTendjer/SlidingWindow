#include "Communication.h"
#include "fsmsystem.h"
#include "logfile.h"
#include "SenderAuto.h"

//char* dataBuffer[20];
int i = 0;

/* FSM system instance. */
static FSMSystem sys(1 /* max number of automates types */, 1 /* max number of msg boxes */);




DWORD WINAPI SystemThread(void *data) {
	SenderAuto automate;

	/* Kernel buffer description block */
	/* number of buffer types */
	const uint8 buffClassNo =  4; 
	/* number of buffers of each buffer type */
	uint32 buffsCount[buffClassNo] = { 50, 50, 50, 10 }; 
	/* buffer size for each buffer type */
 	uint32 buffsLength[buffClassNo] = { 128, 256, 512, 1024}; 
	
	/* Logging setting - to a file in this case */
	LogFile lf("log.log" /*log file name*/, "./log.ini" /* message translator file */);
	LogAutomateNew::SetLogInterface(&lf);

	/* Mandatory kernel initialization */
	printf("[*] Initializing system...\n");
	sys.InitKernel(buffClassNo, buffsCount, buffsLength, 5);

	/* Add automates to the system */
	sys.Add(&automate, SENDER_FSM, 1 /* the number of automates that will be added */, true);

	/* Start the first automate - usually it sends the first message, 
	since only automates can send messages */
	automate.Start();

	/* Starts the system - dispatches system messages */
	printf("[*] Starting system...\n");
	sys.Start();

	/* Finish thread */
	return 0;
}

int  main() {
	DWORD thread_id;
	HANDLE thread_handle;
	//char dataBuffer[BUFFER_SIZE];
	//reading input from file
	
	//while (fgets(readLine, 100, f) != NULL)
	//fscanf(f, "%s", dataBuffer);
	//printf("%s", dataBuffer);
	//FILE* f = fopen("Message.txt", "r");
	//if (f == NULL)
	//{
	//	printf("Could not open file!");
	//}
	//dataBuffer[0] = (char*)malloc(BUFFER_SIZE * sizeof(char));
	//while ((fgets(dataBuffer[i], 100, f) != NULL) && (*dataBuffer[i] != '\n'))
	//{
	//	++i;
	//	//dynamically alocate the memory for all other strings
	//	dataBuffer[i] = (char*)malloc(BUFFER_SIZE * sizeof(char));
	//}

	//printf("%s", dataBuffer[i]);
	////////////////////////connecting//////////////////////////////////////////
	/*if (CreateSocket() == -1)
		return 1;
	if (ConnectToServer() == -1)
		return 1;*/
	///////////////////////////////////////////////////////////////////////////


	/* Start operating thread. */
	thread_handle = CreateThread(NULL, 0, SystemThread, NULL, 0, &thread_id);

	/* Wait for keypress. */
	getch();

	/* Notify the system to stop - this causes the thread to finish */
	printf("[*] Stopping system...\n");
	sys.StopSystem();

	CloseSocket();
	/* Free the thread handle */
	CloseHandle(thread_handle);
}
