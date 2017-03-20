#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <queue>
#include <random>
using namespace std;

const int PLACES=10;

std::vector<pthread_t> writersVector;
std::vector<pthread_t> readersVector;

std::queue<pthread_t> readersQueue;

pthread_mutex_t mutex;
pthread_cond_t turn;

size_t readersCounter;
size_t writersCounter;

bool isWriterPresent = false;
enum Type
{
	NONE=0, WRITER=1,READER=2
};
Type lastInside=NONE;

void* writerThread(void* arg)
{
	std::random_device randomSeed;
	std::mt19937 randomGenerator(randomSeed());
	std::uniform_int_distribution<int> distribution(100,3000000);

	while(true)
	{
		pthread_mutex_lock(&mutex);
		writersCounter++;
		while(readersCounter>0 || isWriterPresent==true || lastInside==WRITER)
		{
			pthread_cond_wait(&turn, &mutex);
		}
		isWriterPresent=true;

		lastInside=WRITER;

		pthread_mutex_unlock(&mutex);

		cout << "Wring... Writer: " << pthread_self() << endl;

		pthread_mutex_lock(&mutex);
		isWriterPresent = false;
		writersCounter--;
		cout << "Writer " << pthread_self() << " Going out" << endl;
		pthread_cond_broadcast(&turn);

		pthread_mutex_unlock(&mutex);
		usleep(distribution(randomGenerator));
	}
}

void* readerThread(void* arg)
{
	std::random_device randomSeed;
	std::mt19937 randomGenerator(randomSeed());
	std::uniform_int_distribution<int> distribution(100,3000000);

	while(true)
	{
		readersQueue.push(pthread_self());
		pthread_mutex_lock(&mutex);
		while(isWriterPresent==true || readersCounter>=PLACES || pthread_self()!=readersQueue.front())
			pthread_cond_wait(&turn, &mutex);
		readersCounter++;
		readersQueue.pop();
		lastInside=READER;

		pthread_mutex_unlock(&mutex);

		cout << "Reading... reader " << pthread_self() << endl;

		pthread_mutex_lock(&mutex);
		readersCounter--;
		cout << "Reader " << pthread_self() << " going out" << endl;
		pthread_cond_broadcast(&turn);
		pthread_mutex_unlock(&mutex);

		usleep(distribution(randomGenerator));
	}
}


int main(int argc, char *argv[])
{
	if(argc<3)
	{
		cout << "Invocation: ./Lab02-5 writers_number readers_number" << endl;
		return EXIT_FAILURE;
	}
	int maxWritersCount = atoi(argv[1]);
	int maxReadersCount = atoi(argv[2]);

	pthread_t tmp;
	for(size_t i=0; i<maxWritersCount; i++)
	{
		pthread_create(&tmp, NULL, writerThread, NULL);
		writersVector.push_back(tmp);
	}

	for(size_t i=0; i<maxReadersCount; i++)
	{
		pthread_create(&tmp, NULL, readerThread, NULL);
		writersVector.push_back(tmp);
	}

	//gwarantuje, ¿e watki sie nie zakoncza bo watek glowny bedzie wstrzymany
	//watki sie nigdy nie joinuja bo dzialaja w nieskonczonej petli - dlatego brak joinów
	while(true)
	{
		//cout<<"main thread...\n";
		sleep(10);
	}

	return EXIT_SUCCESS;
}
