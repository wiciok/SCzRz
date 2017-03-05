#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <vector>
#include <algorithm>

using namespace std;

struct TIDAndTimeStruct
{
	pthread_t TID;
	time_t deadline;

	bool operator < (const TIDAndTimeStruct &x)const
	{
		return deadline>x.deadline;
	}
};

std::vector<TIDAndTimeStruct> TIDAndTimeStructsVector;

void* threadFunction(void *arg)
{
	sleep(10);
	return 0;
}

int main(int argc, char *argv[])
{
	while(1)
	{
		cout<<"This is main thread.\n"<<"Press c key to create new thread, or q to exit\n";
		char choice;
		cin>>choice;

		if(choice=='c')
		{
			cout<<"A new thread is being created. Please input relative execution time in seconds\n";

			int relativeThreadExecutionTime;
			fscanf(stdin, "%d", &relativeThreadExecutionTime);
			//cin>>relativeThreadExecutionTime;

			//time() gets current UTC Time
			//newDeadline = absolute thread execution time
			time_t newDeadline = time(nullptr) + relativeThreadExecutionTime;

			//create new thread
			pthread_t newTID;

			pthread_create(&newTID, NULL, threadFunction, NULL);

			TIDAndTimeStruct threadData;
			threadData.TID=newTID;
			threadData.deadline=newDeadline;

			TIDAndTimeStructsVector.push_back(threadData);


			//sorting threads (nearest deadline = first thread in vector)
			std::sort(TIDAndTimeStructsVector.begin(), TIDAndTimeStructsVector.end());

			//setting new priorities
			sched_param param;
			int policy;


			for(size_t index=0; index<TIDAndTimeStructsVector.size(); index++)
			{
				pthread_getschedparam(TIDAndTimeStructsVector[index].TID, &policy, &param);
				param.sched_priority = index+1;
				//param.sched_priority = TIDAndTimeStructsVector[TIDAndTimeStructsVector.size()-1-index].deadline;

				pthread_setschedparam(TIDAndTimeStructsVector[index].TID, policy, &param);
			}

			//printing threads information:
			for(auto element : TIDAndTimeStructsVector)
			{
				pthread_getschedparam(element.TID, &policy, &param);
				cout<<"TID: "<<element.TID << " deadline " << element.deadline <<" Priority: "<<param.sched_priority<<endl;
			}
		}

		else
		{
			if(choice=='q')
			{
				cout<<"joining threads...";

				for(auto element : TIDAndTimeStructsVector)
				{
					pthread_join(element.TID,NULL);
				}
				cout<<"exitting...";
				exit(0);
			}
			cout<<"Unknown command. Sleeping 3 seconds.\n";
			sleep(3);
		}
	}
	return EXIT_SUCCESS;
}

