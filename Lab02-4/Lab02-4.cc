#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>

#include "circular.h"

using namespace std;


std::vector<pthread_t> producersVector;
std::vector<pthread_t> consumentsVector;

pthread_mutex_t mutex;
pthread_cond_t isFreeSpace, isAnyElement;

const int bufferCapacity=15;
circular_buffer<int> buffer(bufferCapacity);


void* producerCode(void* arg)
{
	std::random_device randomSeed;
	std::mt19937 randomGenerator(randomSeed());
	std::uniform_int_distribution<int> distribution(0,100);
	int producedValue;

	while(1)
	{
		pthread_mutex_lock(&mutex);

		while(buffer.capacity()==buffer.size()) //bufor pelny
		{
			pthread_cond_wait(&isFreeSpace, &mutex); //czekamy a¿ pojawi sie info ze jest wolne miejsce
		}

		producedValue=distribution(randomGenerator);
		buffer.push_back(producedValue);

		//ustawiamy info ze w buforze cos jest
		pthread_cond_signal(&isAnyElement);

		pthread_mutex_unlock(&mutex);

		cout << "Produced value: " << producedValue << endl;
		usleep(distribution(randomGenerator)*1000);
	}
	return 0;
}

void* consumerCode(void* arg)
{
	std::random_device randomSeed;
	std::mt19937 randomGenerator(randomSeed());
	std::uniform_int_distribution<int> distribution(0,100);

	int product;

	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(buffer.empty()) //bufor pusty
		{
			pthread_cond_wait(&isAnyElement, &mutex); //czekamy na info ze w buforze jest juz jakis element
		}

		//zdejmowanie z bufora i usuwanie wartosci
		product = buffer.front();
		buffer.pop_front();

		//ustawiamy info ze w buforze jest miejsce
		pthread_cond_signal(&isFreeSpace);

		pthread_mutex_unlock(&mutex);

		cout << "Consumed value: " << product << endl;
		usleep(distribution(randomGenerator)*1000);
	}
	return 0;
}


int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		cout << "Invocation: ./Lab02-4 producers_number consuments_number";
		return EXIT_FAILURE;
	}

	size_t producersCount = atoi(argv[1]);
	size_t consumersCount = atoi(argv[2]);

	producersVector.reserve(producersCount);
	consumentsVector.reserve(consumersCount);

	pthread_t tmp;
	for(size_t i=0;i<producersCount;i++)
	{
		pthread_create(&tmp, NULL, producerCode, NULL);
		producersVector.push_back(tmp);
	}
	for(size_t i=0;i<consumersCount;i++)
	{
		pthread_create(&tmp, NULL, consumerCode, NULL);
		producersVector.push_back(tmp);
	}

	//gwarantuje, ¿e watki sie nie zakoncza bo watek glowny bedzie wstrzymany
	//watki sie nigdy nie joinuja bo dzialaja w nieskonczonej petli - dlatego brak joinów
	while(true)
	{
		cout<<"main thread...\n";
		sleep(10);
	}

	return EXIT_SUCCESS;
}
