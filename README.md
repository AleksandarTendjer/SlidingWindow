------------------------------------SLIDING WINDOW------------------------------------

A sliding window mechanism, based on a client-server architecture implemented  with the UDP.
Simulator, that implements packet loss, so that we can do a estimation how many sent packets have been lost over some time interval.

Both on the client( Sender ) and the server( Receiver ), FSM (Finit State Machines) have been enforced.
										Sender states:
1. IDLE-state in which the Sender is when sending the message and doing all the processing.
2. RECEIVER_SENT-state in which the Sender is when the message is sent.

										Receiver states:
1. IDLE-state in which the Receiver is when receiving the message and doing all the processing.
2. RECEIVER_SENT-state in which the Receiver is when the message is received.

---------------------------------------------------------------------------------------  
