#include <cstdlib>
#include <iostream>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>

using namespace std;

	int remote_pid;
	int remote_chid;
	int coid;
	int bounceNumber=0;
	int chid, pid;


	_pulse mypulse;

	void play();

int main(int argc, char *argv[])
{

	chid = ChannelCreate(0);
	if(-1 == chid)
	{
		perror("ChannelCreate()");
	    exit(EXIT_FAILURE);
	}

	pid = getpid();
	printf("Server's pid: %d, chid: %d\n", pid, chid);



	cout<<"enter remote pid"<<endl;
	cin>>remote_pid;
	cout<<"enter remote chid"<<endl;
	cin>>remote_chid;

	coid = ConnectAttach(ND_LOCAL_NODE, remote_pid, remote_chid, _NTO_SIDE_CHANNEL, 0);
	if(-1 == coid)
	{
	    perror("ConnectAttach");
	    exit(EXIT_FAILURE);
	}



	cout<<"Wprowadz s jesli zaczynasz lub w jesli czekasz na ruch przeciwnika\n";
	char choice;
	cin>>choice;
	switch(choice)
	{
	case 's':
		MsgSendPulse(coid,1,6,bounceNumber);
		play();
		break;
	case 'w':
		play();
		break;
	default:
		cout<<"Bledny wybor\n";
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void play()
{
	while(1)
	{
		MsgReceivePulse(remote_chid, &mypulse, sizeof(mypulse), 0);
		cerr<<"odebrano: "<<mypulse.value.sival_int<<endl;
		bounceNumber++;
		usleep(500000);
		MsgSendPulse(coid,1,6,bounceNumber);
	}
}
