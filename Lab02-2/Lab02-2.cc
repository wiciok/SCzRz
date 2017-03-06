#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

bool checkIfPrimary(int number);

struct ThreadData
{
   int zd;
   int zg;
   pthread_t TID;
};

ThreadData* threads;

void *threadFunction(void* argv)
{
	int counter=0;
	//dodac 1 do zg(?)
	for(int i=threads[pthread_self() - 2].zd; i<threads[pthread_self() - 2].zg; i++)
	{
		if (checkIfPrimary(i)==true)
		{
			cout<< "Thread PID: " <<pthread_self()<< " Prime: " << i << endl;
			counter++;
		}
	}
	return (void *)counter;
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

	cout<<"Input parameters:"<<endl
			<<"Lower bound: "<<zd<<endl
			<<"Upper bound: "<<zg<<endl
			<<"Threads number: "<<threadsNumber<<endl;

	int threadInterval=(zg-zd)/threadsNumber;

	int zdOfInterval = zd;
	int zgOfInterval = zd+threadInterval;

	threads = new ThreadData[threadsNumber];

	//compute bounds and create threads
	for(int i=0; i<threadsNumber-1; i++)
	{
		threads[i].zd = zdOfInterval;
		threads[i].zg = zgOfInterval;
		zdOfInterval += threadInterval;

		pthread_create(&(threads[i].TID), NULL, threadFunction, NULL);
		zdOfInterval = zgOfInterval;
		zgOfInterval += threadInterval;
	}

	//last interval
	threads[threadsNumber-1].zd = zdOfInterval;
	threads[threadsNumber-1].zg = zg;
	pthread_create(&(threads[threadsNumber-1].TID), NULL, threadFunction, NULL);


	for(int i=0; i<threadsNumber; i++)
	{
		void* primesNumber;
		pthread_join(threads[i].TID, &primesNumber);
		cout<<"Thread "<<threads[i].TID;
		cout<<" Primes number: "<<(int)primesNumber<<endl;
	}
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
