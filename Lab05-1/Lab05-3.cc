#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <cstdlib>

#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL
#define STOP_PULSE_CODE   _PULSE_CODE_MINAVAIL+1

using namespace std;

union my_message_t
{
	_pulse  pulse;
};


const std::string realPassword="qwerty";
itimerspec       itime;
timer_t          timer_id;
sigevent         event;

void* threadFunction(void* unused)
{
	while(true)
	{
		std::string password;
		cout<<"Enter password"<<endl;
		cin>>password;

		if(password.compare(realPassword)==0) //typed password matches real password
		{
			//isAuthenticated=true;
			cout<<"Password correct!"<<endl;
			timer_delete(timer_id);
			MsgSendPulse(event.sigev_coid, event.sigev_priority, STOP_PULSE_CODE, 0);
			return 0;
		}
		else
		{
			cout<<"Password incorrect! "<<endl;
			timer_settime(timer_id, 0, &itime, NULL);
		}
	}
}

int main()
{
   int              chid;
   int              rcvid;
   my_message_t     msg;
   pthread_t 		tid;

   chid = ChannelCreate(0);

   event.sigev_notify = SIGEV_PULSE;
   event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);
   event.sigev_priority = getprio(0);
   event.sigev_code = MY_PULSE_CODE;
   timer_create(CLOCK_REALTIME, &event, &timer_id);

   itime.it_value.tv_sec = 5;
   itime.it_value.tv_nsec = 0;
   itime.it_interval.tv_sec = 5;
   itime.it_interval.tv_nsec = 0;

   pthread_create(&tid, NULL, threadFunction, NULL);

   timer_settime(timer_id, 0, &itime, NULL);

   while (true)
   {
	   rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
       if (rcvid == 0) //received pulse
       {
            if (msg.pulse.code == MY_PULSE_CODE)
            {
                cout<<"Try again"<<endl;
            }

            else
            {
            	if(msg.pulse.code==STOP_PULSE_CODE)
            		break;
            }
       }
   }
   pthread_join(tid, 0);
   cout<<"Shutting down!"<<endl;


   exit(EXIT_SUCCESS);
}
