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
   int begin;
   int end;
   pthread_t TID;
};

std::vector<ThreadData> threadsVector;

void *threadFunction(void* argv)
{
	int counter=0;
	for(int i=threadsVector[(int)pthread_self() - 2].begin; i<threadsVector[(int)pthread_self() - 2].end; i++)
	{
		if (checkIfPrimary(i)==true)
		{
			cout<< "Thread PID: " <<pthread_self()<< " Prime number: " << i << endl;
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

	cout<<"Input parameters:"
			<<endl<<"Lower bound: "<<zd<<endl
			<<"Upper bound: "<<zg<<endl
			<<"Threads number: "<<threadsNumber<<endl;

	/*
	//granice powiekszamy o 1 tak aby uzytkownik podawal przedzial obustronnie domkniety
	upperBound+=1;
*/

	//threadsVector.reserve(threadsNumber);

	int threadInterval=(zg-zd)/threadsNumber;

	//ThreadData threadStruct;

	// DODAC zapiswanie do wektora
	for(int i=0;i<threadsNumber-1;i++)
	{
		ThreadData threadStruct;

		threadStruct.begin=zd+i*threadInterval;
		threadStruct.end=threadStruct.begin+threadInterval;
		pthread_create(&threadStruct.TID, NULL, threadFunction, NULL);

		threadsVector.push_back(threadStruct);

		/*threadsVector[i].begin=zd+i*threadInterval;
		threadsVector[i].end=threadsVector[i].begin+threadInterval;
		pthread_create(&threadsVector[i].TID, NULL, threadFunction, NULL);*/
	}

	//last interval
	/*//posprawdzac indeksy
	threadStruct.begin=zd+(threadsNumber-1)*threadInterval;
	threadStruct.end=zg;
	threadStruct.num=threadsNumber;
	pthread_create(threadsVector[threadsNumber-1].TID, NULL, threadFunction, NULL);*/

	//cout<<"vector capacity: "<< threadsVector.size()<<endl;

	//tutaj pobieranie wartroci!!!
	for(auto element : threadsVector)
	{
		void* primesNumber;
		pthread_join(element.TID, &primesNumber);
		cout<<"Thread "<<element.TID<<" Primes number: "<<*(int*)primesNumber;
	}

	sleep(10);
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
