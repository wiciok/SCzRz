#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

using namespace std;

volatile int temp=0;
volatile bool flag=false;

void* mierzenie(void *argv)
{
	srand(100);

	sched_param param;
	int policy;
	pthread_getschedparam(pthread_self(), &policy, &param);

	while(1)
	{
		fprintf(stderr, "TID: %d curpriority: %d priority: %d\n", pthread_self(), param.sched_curpriority, param.sched_priority);
		usleep(100000);
		temp=rand()*100/RAND_MAX;
		fprintf(stderr,"%d\n",temp);
	}
	return 0;
}

void* alarm(void *argv)
{
	sched_param param;
	int policy;
	pthread_getschedparam(pthread_self(), &policy, &param);
	fprintf(stderr, "TID: %d curpriority: %d priority: %d\n", pthread_self(), param.sched_curpriority, param.sched_priority);


	int i =11;
	while(1)
	{
//		if(flag==true)
//		{
//			fprintf(stderr, "ALARM!!! TEMP: %d\n", temp);
//		flag=false;
//		}
		i++;
		i--;
	}
	return 0;
}


int main(int argc, char *argv[])
{
	setprio(getpid(),7);

	pthread_t thread_alarm;
	pthread_t thread_mierzenie;

	fprintf(stderr,"test");


	pthread_attr_t attr;
	pthread_attr_init( &attr );
	//pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);

	pthread_create(&thread_alarm,&attr,alarm, NULL);
	pthread_create(&thread_mierzenie,&attr,mierzenie, NULL);


	pthread_attr_destroy(&attr);


	int policy;
	sched_param param;

	pthread_getschedparam(thread_alarm, &policy, &param);
	param.sched_priority = 6;
	pthread_setschedparam(thread_alarm, policy, &param);

	pthread_getschedparam(thread_mierzenie, &policy, &param);
	param.sched_priority = 7;
	pthread_setschedparam(thread_mierzenie, policy, &param);

	pthread_getschedparam(pthread_self(), &policy, &param);
	param.sched_priority = 7;
	pthread_setschedparam(pthread_self(), policy, &param);


	fprintf(stderr, "TID: %d curpriority: %d\n", pthread_self(), param.sched_curpriority);

	while(1)
	{
		if(temp>90)
			flag=true;
		fprintf(stderr, "temperatura: %d\n", temp);
		fprintf(stderr, "AAATID: %d curpriority: %d priority: %d\n", pthread_self(), param.sched_curpriority, param.sched_priority);

		sleep(1);
	}
	return 0;
}
