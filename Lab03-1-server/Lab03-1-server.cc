#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <process.h>

#include <ctype.h>
#define SIZE 256

typedef struct  {
   int  typ;        // typ komunikatu
   int  from;       // nr procesu który wys³a³ komunikat
   int  ile;        // ile by³o malych liter
   char text[SIZE]; // tekst komunikatu
 } mss_t;

 using namespace std;

int
main(void) {
	int chid;
	int pid;
	int rcvid;
	mss_t msg;
	int status;

	chid = ChannelCreate(0); //PUT CODE HERE to create a channel
	if(-1 == chid) {                //was there an error creating the channel?
		perror("ChannelCreate()");  //look up the errno code and print
		exit(EXIT_FAILURE);
	}

	pid = getpid();                 //get our own pid
	printf("Server's pid: %d, chid: %d\n", pid, chid); //print our pid/chid so
	                                             //client can be told where to
	                                             //connect
	while(1)
	{
		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);

		cout<<"received message: "<<msg.text<<endl;

		for(size_t i=0; i<strlen(msg.text);i++)
		{
			msg.text[i]=toupper(msg.text[i]);
		}

		cout<<"sending back message: "<<msg.text<<endl;
		status = MsgReply(rcvid, EOK, &msg, sizeof(msg));

		if(rcvid == -1)
		{
			perror("MsgReceive");
			break;
		}
	}
	return 0;
}
