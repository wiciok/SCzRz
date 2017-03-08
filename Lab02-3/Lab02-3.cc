#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

struct ThreadData
{
   int zd;
   int zg;
   pthread_t TID;
};

struct Interval
{
	//mozna dolozyc numer przedzialu

	int zd;
	int zg;
};


bool checkIfPrimary(int number);
Interval getInterval();



std::vector<ThreadData> ThreadsVector;
std::vector<Interval> IntervalsVector;

pthread_mutex_t intervalsMutex;

void *threadFunction(void* argv)
{
	Interval interval;

	while(IntervalsVector.size()>0)
	{
		interval=getInterval();

		if(interval.zd==-1 && interval.zg==-1)
		{
			return nullptr;
		}

		int counter=0;
		for(int i=interval.zd; i<interval.zg+1; i++)
		{
			if (checkIfPrimary(i)==true)
			{
				cout<< "Thread PID: " <<pthread_self()<< " Prime: " << i << endl;
				counter++;
			}
		}
		cout<<"Thread TID: "<<pthread_self()<<" primes counter: "<<counter<<endl;
	}
	return nullptr;
}


Interval getInterval()
{
	Interval interval;

	if(IntervalsVector.size()>0)
	{
		pthread_mutex_lock(&intervalsMutex);
		interval=IntervalsVector.back();
		IntervalsVector.pop_back();
		pthread_mutex_unlock(&intervalsMutex);
	}
	else
	{
		interval.zg=-1;
		interval.zd=-1;
	}
	return interval;
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		cout << "Wrong arguments number!" << endl;
		return EXIT_FAILURE;
	}

	int zd=atoi(argv[1]);
	int zg=atoi(argv[2]);
	int threadsNumber = atoi(argv[3]);

	const int IntervalNumber=threadsNumber*4; //zalozenie

	cout<<"Input parameters:"
			<<endl<<"Lower bound: "<<zd<<endl
			<<"Upper bound: "<<zg<<endl
			<<"Threads number: "<<threadsNumber<<endl;

	int intervalLength=(zg-zd)/IntervalNumber;
	int zdOfInterval = zd;
	int zgOfInterval = zd+intervalLength;

	Interval tmpInterval;

	//tworzenie przedzialow
	for(int i=0; i<threadsNumber-1; i++)
	{
		tmpInterval.zd=zdOfInterval;
		tmpInterval.zg = zgOfInterval;

		IntervalsVector.push_back(tmpInterval);

		zdOfInterval += intervalLength;
		zdOfInterval = zgOfInterval;
		zgOfInterval += intervalLength;
	}

	//ostatni przedzial
	tmpInterval.zd=zdOfInterval;
	tmpInterval.zg = zg;

	IntervalsVector.push_back(tmpInterval);


	ThreadData tmpThread;
	//tworzenie watkow
	for(int i=0; i<threadsNumber; i++)
	{
		pthread_create(&tmpThread.TID, NULL, threadFunction, NULL);
		ThreadsVector.push_back(tmpThread);
	}


	for(int i=0; i<threadsNumber; i++)
	{
		//void* primesNumber;
		//pthread_join(threads[i].TID, &primesNumber);
		pthread_join(ThreadsVector[i].TID, nullptr);
		//cout<<"Thread "<<ThreadsVector[i].TID;
		//cout<<" Primes number: "<<(int)primesNumber<<endl;
	}
	//usuwanie watkow z wektora
	cout<<"test";
	sleep(1);
	return EXIT_SUCCESS;
}


bool checkIfPrimary(int number)
{
	if(number==1)
		return false;
	if(number==2)
		return true;
	else
	{
		for(int i=2; i<number; i++)
			if(number%i == 0) return false;
		return true;
	}
}
