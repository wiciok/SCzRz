#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include <cstdlib>
#include <iostream>
#define SIZE 256

typedef struct  {
   int  typ;        // typ komunikatu
   int  from;       // nr procesu który wys³a³ komunikat
   int  ile;        // ile by³o malych liter
   char text[SIZE]; // tekst komunikatu
 } mss_t;

 using namespace std;

int main(int argc, char* argv[])
{
	int coid;         //Connection ID to server
	mss_t msg;
	int status;       //status return value used for ConnectAttach and MsgSend
	int server_pid;   //server's process ID
	int server_chid;  //server's channel ID


	if(3 != argc) {
		printf("This program must be started with commandline arguments, for example:\n\n");
		printf("   cli 482834 1 abcdefghi    \n\n");
		printf(" 1st arg(482834): server's pid\n");
		printf(" 2nd arg(1): server's chid\n");
		exit(EXIT_FAILURE);
	}

	server_pid = atoi(argv[1]);
	server_chid = atoi(argv[2]);

	printf("attempting to establish connection with server pid: %d, chid %d\n", server_pid, server_chid);

	coid = ConnectAttach(ND_LOCAL_NODE, server_pid, server_chid, _NTO_SIDE_CHANNEL, 0);
	if(-1 == coid) {   //was there an error attaching to server?
		perror("ConnectAttach"); //look up error code and print
		exit(EXIT_FAILURE);
	}



	cout<<"wprowadz wiadomosc do wyslania\n";
	cin>>msg.text;

	char reply[SIZE]="";


	status = MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));
	if(-1 == status)
	{
		perror("MsgSend");
		exit(EXIT_FAILURE);
	}

	//small issue - trash chars added to beginning of string
	cout<<"received reply from server: "<<reply<<endl;

	return EXIT_SUCCESS;
}




