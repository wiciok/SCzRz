#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <process.h>
#include <pthread.h>
#include <sched.h>
#include <cstdlib>
#include <stdlib.h>
//#include <unistd.h>

using namespace std;

#define PULSE_SECOND   _PULSE_CODE_MINAVAIL
#define PULSE_10_SECONDS _PULSE_CODE_MINAVAIL+1

union my_message_t
{
	_pulse pulse;
};

void *threadFunction(void*);

int chid;
int secondsCounter=0;
int minutesCounter=0;


int main()
{
   sigevent eventTimer, eventNotifier;
   itimerspec itimerspecTimer, itimerspecNotifier;
   timer_t timerTimer,timerNotifier;
   my_message_t msg;

   chid = ChannelCreate(0);

   //przygotowanie eventów

   eventTimer.sigev_notify = SIGEV_PULSE;
   eventTimer.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,chid,_NTO_SIDE_CHANNEL, 0);
   eventTimer.sigev_priority = getprio(0);
   eventTimer.sigev_code = PULSE_SECOND;

   eventNotifier.sigev_notify = SIGEV_PULSE;
   eventNotifier.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,chid,_NTO_SIDE_CHANNEL, 0);
   eventNotifier.sigev_priority = getprio(0);
   eventNotifier.sigev_code = PULSE_10_SECONDS;

   timer_create(CLOCK_REALTIME, &eventTimer, &timerTimer);
   timer_create(CLOCK_REALTIME, &eventNotifier, &timerNotifier);

   //przygotowanie timerów

   itimerspecTimer.it_value.tv_sec = 0;
   itimerspecTimer.it_value.tv_nsec = 1;
   itimerspecTimer.it_interval.tv_sec = 1;
   itimerspecTimer.it_interval.tv_nsec = 0;
   timer_settime(timerTimer, 0, &itimerspecTimer, NULL);

   itimerspecNotifier.it_value.tv_sec = 10;
   itimerspecNotifier.it_value.tv_nsec = 0;
   itimerspecNotifier.it_interval.tv_sec = 10;
   itimerspecNotifier.it_interval.tv_nsec = 0;
   timer_settime(timerNotifier, 0, &itimerspecNotifier, NULL);

   pthread_t TID;
   pthread_create(&TID,NULL,threadFunction,NULL);

   int rcvid;
   while(true)
   {
       rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
       if (rcvid == 0)
       {
            if (msg.pulse.code == PULSE_SECOND)
            {
                ++secondsCounter;
                if(secondsCounter==60)
                {
                	++minutesCounter;
                	secondsCounter=0;
                }
            }
            else if(msg.pulse.code==PULSE_10_SECONDS)
            {
            	cout<<minutesCounter<<":"<<secondsCounter<<endl;
            }
            else
            {
            	cerr<<"Odebrano niezidentyfikowany puls!"<<endl;
            	exit(EXIT_FAILURE);
            }
       }
   }
   return 0;
}


void *threadFunction(void* unused)
{
	char inputChar='\0';
	int coid=ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);
	MsgSendPulse(coid,getprio(0),PULSE_10_SECONDS,0);

	cout<<"Klawisze:"<<endl
			<<"8 - dodaj minuty"<<endl
			<<"2 - odejmij minuty"<<endl
			<<"6 - dodaj sekundy"<<endl
			<<"4 - odejmij sekundy"<<endl;

	while(true)
	{
		cin.get(inputChar);
		//inputChar=getc(stdin);
		//read(0, &inputChar, 1);

		switch(inputChar)
		{
		case '8':
			++minutesCounter;
			break;
		case '2':
			if(minutesCounter>=1)
				--minutesCounter;
			break;
		case '6':
			++secondsCounter;
			break;
		case '4':
			if(secondsCounter>=1)
				--secondsCounter;
			break;
		case '\n':
			break;
		default:
			cerr<<"Nierozpoznany znak!"<<endl;
			break;
		}
	}
	return 0;
}
