#include <sys/dispatch.h>
#include <vector>
#include <libc.h>
#include <sys/netmgr.h>
#include <iostream>

using namespace std;

struct messageStruct
{
    int pid;
    int chid;
    int period;
    int priority;
};

messageStruct message;
messageStruct reply;
int serverCoid;
int chid;

void signalHandler(int signal)
{
	if(signal!=SIGINT)
		return;

	cerr<<"Client: quit"<<endl;

	ChannelDestroy(chid);
	name_close(serverCoid);

	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
	signal(SIGINT, signalHandler);

	if (argc < 4)
	{
		cerr<<"Propper Invocation: ./Lab04_client server_name period priority"<<endl;
		return EXIT_FAILURE;
	}

	if ((serverCoid = name_open(argv[1], 0)) == -1)
	{
		cerr<<"Client: name_open error"<<endl;
		return EXIT_FAILURE;
	}

    message.pid = getpid();
    message.chid = ChannelCreate(0);
    chid=message.chid;
    message.period = atoi(argv[2]);
    message.priority = atoi(argv[3]);

    MsgSend(serverCoid, &message, sizeof(message), &reply, sizeof(reply));
    cerr<<"Client: msg sent";

    while (true)
    {
    	MsgReceive(chid, NULL, 0, NULL);
    	cerr<<"Client: received pulse from: "<<message.pid;
    }

    return EXIT_SUCCESS;
}
