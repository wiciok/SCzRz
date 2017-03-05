/*#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <vector>
#include <algorithm>

using namespace std;

std::vector<time_t> DeadlineVector(20);
std::vector<pthread_t> TIDVector(20);

void* threadFunction(void *arg)
{
	pthread_t TID=pthread_self();
	int policy;
	sched_param sched_param;

	pthread_getschedparam(TID, &policy, &sched_param);

	std::cout<<"This is thread TID: "<<TID<<endl<<"My policy is: "<<policy<<endl<<"My priority is: "<<sched_param.sched_priority;
	while(1)
	{
		std::cout<<"*";
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	sched_param param;
	int policy;
	pthread_t newTID;

	while(1)
	{
		fprintf(stderr, "This is main thread.\n");
		fprintf(stderr, "A new thread is being created. Please input relative execution time in seconds");

		int relativeThreadExecutionTime;
		scanf("%d", &relativeThreadExecutionTime);

		time_t currentUTCTime;
		time(&currentUTCTime);
		time_t deadline = currentUTCTime + relativeThreadExecutionTime; // = absolute thread execution time


		pthread_create(&newTID, NULL, threadFunction, NULL);
		pthread_getschedparam(newTID, &policy, &param);

		if(TIDVector.size()==0) //first thread (except main)
		{
			param.sched_priority=sched_ss_low_priority+1;
		}

		else
		{
			auto minDeadlineIter=std::min_element(DeadlineVector.begin(), DeadlineVector.end());
			int index=std::distance(DeadlineVector.begin(), minDeadlineIter);

			//if(deadline<*minDeadlineIter)
			if(deadline<DeadlineVector[index])
			{
				int maxExistingPriority=TIDVector[index];

				if(maxExistingPriority+1<254) //? czy na pewno?
					param.sched_priority=maxExistingPriority+1;
				else
				{
					cout<<"Error!"<<endl;
					exit(1);
				}
			}
			else
			{
				for (auto element : DeadlineVector)
				{
					//dla ka¿dego watku o deadline mniejszym niz nowy watek
					if(element<deadline)
					{
						int index=std::distance(DeadlineVector.begin(), element);

						pthread_t tmpTID=TIDVector[index];
						int policy;
						sched_param sched_param;

						pthread_getschedparam(tmpTID, &policy, &sched_param);

						//powiekszamy priorytet tego elementu o 1
						sched_param.sched_priority+=1;
						pthread_setschedparam(tmpTID, policy, &sched_param);


						//i ustalamy jaki (pytanie - jaki???) priorytet dla nowego watku
						param.sched_priority=maxExistingPriority;
					}
				}
			}
		}

		TIDVector.push_back(newTID);
		DeadlineVector.push_back(deadline);

		pthread_setschedparam(newTID, policy, &param);
	}

	return EXIT_SUCCESS;
}*/
