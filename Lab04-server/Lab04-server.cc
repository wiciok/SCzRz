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
name_attach_t *attachedServerName=nullptr;
std::vector<pthread_t> threadsVector;
std::vector<messageStruct> threadDataVector;

void* threadFunction(void *arg)
{
	cerr<<"Server: thread: "<<pthread_self()<<endl;
	
	messageStruct messToSend = threadDataVector[pthread_self() - 2];
	
	int coid = ConnectAttach(ND_LOCAL_NODE, messToSend.pid, messToSend.chid, _NTO_SIDE_CHANNEL, 0);
	
	while(true)
	{
		MsgSendPulse(coid, messToSend.priority, 0, getpid());
		sleep(messToSend.period);
	}
	return 0;
}


void signalHandler(int signal)
{
	if(signal!=SIGINT)
		return;

	for(auto el : threadsVector)
	{
		pthread_join(el,NULL);
	}
	cerr<<"Server: quit"<<endl;
	name_detach(attachedServerName, 0);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signalHandler);

    if (argc < 2)
    {
        fprintf(stderr, "Propper Invocation: ./Lab04_server server_name \n");
        return EXIT_FAILURE;
    }

    attachedServerName = name_attach(NULL, argv[1], 0);

    if (attachedServerName == NULL)
    {
    	cerr<<"Server: name_attach error"<<endl;
		return EXIT_FAILURE;
    }

    //creating threads
    while (true)
    {
    	//rcvid = receive identifier
    	int rcvid= MsgReceive(attachedServerName->chid, &message, sizeof(message), NULL);
    	cerr<<"Server: client connected"<<endl;

    	pthread_t tid;
    	pthread_create(&tid,NULL,threadFunction,NULL);
    	threadsVector.push_back(tid);
    	threadDataVector.push_back(message);
    	MsgReply(rcvid, EOK, &message, sizeof(message));
    }

    return EXIT_SUCCESS;
}
