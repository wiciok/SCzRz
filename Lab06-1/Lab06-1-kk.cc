/*#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <process.h>
#include <pthread.h>

#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL
#define MY_PULSE_CODE_1 _PULSE_CODE_MINAVAIL+1

typedef union {
        struct _pulse   pulse;

} my_message_t;

using namespace std;

int secondsCounter=0;
int minutesCounter=0;
int chid;

void *kod(void *argv)
{
	int coid;
	char znak;
	coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);
	MsgSendPulse(coid,5,MY_PULSE_CODE_1,0);
    while(1)
    {
        cin>>znak;
        if(znak=='w'){
        	secondsCounter++;
        	MsgSendPulse(coid,5,MY_PULSE_CODE_1,0);
        }
        else if(znak=='s'){
        	secondsCounter--;
        	MsgSendPulse(coid,5,MY_PULSE_CODE_1,0);
        }
        else if(znak=='a'){
        	minutesCounter--;
        	MsgSendPulse(coid,5,MY_PULSE_CODE_1,0);
        }
        else if(znak=='d'){
        	minutesCounter++;
        	MsgSendPulse(coid,5,MY_PULSE_CODE_1,0);
        }
   }
   return NULL;
}

int main()
{
	cout<<"LEGENDA\nW +1s\nS -1s\nA -1m\nD +1m\n"<<endl;
   struct sigevent event,event2;
   struct itimerspec itime;
   struct itimerspec itime2;
   timer_t timer1_id,timer2_id;
   int rcvid;
   my_message_t msg;

   chid = ChannelCreate(0);

   event.sigev_notify = SIGEV_PULSE;
   event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,
                                    chid,
                                    _NTO_SIDE_CHANNEL, 0);
   event.sigev_priority = getprio(0);
   event.sigev_code = MY_PULSE_CODE;

   event2.sigev_notify = SIGEV_PULSE;
   event2.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,
                                    chid,
                                    _NTO_SIDE_CHANNEL, 0);
   event2.sigev_priority = getprio(0);
   event2.sigev_code = MY_PULSE_CODE_1;

   timer_create(CLOCK_REALTIME, &event, &timer1_id);
   timer_create(CLOCK_REALTIME, &event2, &timer2_id);

   itime.it_value.tv_sec = 0;
   itime.it_value.tv_nsec = 1;
   itime.it_interval.tv_sec = 1;
   itime.it_interval.tv_nsec = 0;
   timer_settime(timer1_id, 0, &itime, NULL);

   itime2.it_value.tv_sec = 10;
   itime2.it_value.tv_nsec = 0;
   itime2.it_interval.tv_sec = 10;
   itime2.it_interval.tv_nsec = 0;
   timer_settime(timer2_id, 0, &itime2, NULL);


   int wtid;
   pthread_attr_t wattr;
   pthread_attr_init( &wattr );
   pthread_attr_setdetachstate(&wattr,PTHREAD_CREATE_DETACHED);
   pthread_create(&wtid,&wattr,kod,NULL);

   while(1) {
       rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
       if (rcvid == 0) {
            if (msg.pulse.code == MY_PULSE_CODE) {
                secondsCounter++;
                if(secondsCounter==60){
                	minutesCounter++;
                	secondsCounter=0;
                }
            }
            if(msg.pulse.code==MY_PULSE_CODE_1){
            	cout<<minutesCounter<<"m "<<secondsCounter<<"s"<<endl;
            }
       }
   }
   return 0;
}*/
