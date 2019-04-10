#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "linkedlist.c"


int createThreads(int num, int numreaders, int numwriters);
void *lastThread(void *arg);
void *handleReaders(void * arg);
void *handleWriters(void *arg);

void Pthread_mutex_lock(pthread_mutex_t * mutex) {
	int rc = pthread_mutex_lock(mutex);
	if(rc != 0){
		fprintf(stderr, "Error locking mutex\n");
		exit(2);
	}
}
void Pthread_mutex_unlock(pthread_mutex_t * mutex) {
	int rc = pthread_mutex_unlock(mutex);
	if(rc != 0){
		fprintf(stderr, "Error unlocking mutex\n");
		exit(2);
	}
}
void Pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex) {
	int rc = pthread_cond_wait(cond, mutex);
	if(rc != 0){
		fprintf(stderr, "Error waiting\n");
		exit(2);
	}
}
void Pthread_cond_signal(pthread_cond_t * cond) {
	int rc = pthread_cond_signal(cond);
	if(rc != 0){
		fprintf(stderr, "Error signaling\n");
		exit(2);
	}
}

pthread_mutex_t rmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readTry = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  lastTCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lastT = PTHREAD_MUTEX_INITIALIZER;
linkedlist list;
int readcount = 0;
int writecount = 0;
int lastRead = 1;
int almostDone;

int main(int argc, char ** argv, char ** envp) {

	if (argc < 4) {
		printf("incorrect command line parameters. Correct usage: \t\n %s <N value> <R-value> <W-value>\n", argv[0]);
		exit(1);
	}

	int num, numreaders, numwriters;
	num = atoi(argv[1]);
	numreaders = atoi(argv[2]);
	numwriters = atoi(argv[3]);

	if (num < 1 || num > 1000) {
		printf("Errpr: Incorrect N value, choose value between 1 and 1000\n");
		exit(1);
	}

	if (numreaders < 1 || numreaders > 9) {
		printf("Error: Incorrect R value, choose value between 1 and 9\n");
		exit(1);
	}

	if (numwriters < 1 || numwriters > 9) {
		printf("Error: Incorrect W value, choose value between 1 and 9\n");
		exit(1);
	}
	almostDone = numreaders;
	linkedlist_Init(&list);
	createThreads(num, numreaders, numwriters);
	//linkedlist_Destroy(&list);
	return 0;
}


int createThreads(int num, int numreaders, int numwriters){
	int numThreads = numwriters + numreaders + 1;

	pthread_t threads[numThreads];
	void * retvals[numThreads];
	int i;
	for(i = 0; i < numwriters; i++){
		int *args = (int*)malloc(2 * sizeof(int));
		args[0] = num;
		args[1] = i + 1;
		//int *N = malloc(sizeof(num));
		//*N = num;
		if(pthread_create(&threads[i], NULL, handleWriters, args) != 0){
			fprintf(stderr, "Error: Writer Thread %d cannot created\n", i);
			exit(2);
		}
	}
	int j = i;
	for(j; j < i+numreaders; j++){
		//int *N = malloc(sizeof(num));
		//*N = num;
		int *args = (int*)malloc(2 * sizeof(int));
		args[0] = num;
		args[1] = j-i + 1;
		if(pthread_create(&threads[j], NULL, handleReaders, args)!= 0){
			fprintf(stderr, "Error: Reader Thread %d cannot created\n", j-i);
			exit(2);
		}
	}
	int *args = (int*)malloc(2 * sizeof(int));
	args[0] = num;
	args[1] = 1;
	if(pthread_create(&threads[j], NULL, lastThread, args)!= 0){
		fprintf(stderr, "Error: Waiting Thread cannot be created");
		exit(2);
	}
	
	for(int c = 0; c <= j; c++){
		if(pthread_join(threads[c], (void **)&retvals[c]) != 0){
			fprintf(stderr, "Error: Thread %d cannot join\n", c);
			exit(2);
		}
	}
	return 0;
}
	

void *lastThread(void *arg){
	//waits on condition variable when only single reader thread remains
	free(arg);
	Pthread_mutex_lock(&lastT);
	while(almostDone > 1){
		Pthread_cond_wait(&lastTCond, &lastT);
	}
	Pthread_mutex_unlock(&lastT);
	fprintf(stdout, "Almost Done!\n");
	
}

void *handleReaders(void * arg){

	int * arr = (int *) arg;

	int linesToRead = arr[0];
	int iThread = arr[1];
	free(arg);
	int linesRead = 0;
	int count = 0;
	char fileName[] = "reader_0.txt";
	
	for(int i = 1; i <= linesToRead; i++){
		Pthread_mutex_lock(&readTry); //inicate reader is trying to enter
		Pthread_mutex_lock(&rmutex); // entry section lock to avoid race condition with other readers
		readcount++; //report self as reader
		if(readcount == 1){
			Pthread_mutex_lock(&resource);//if first reader, lock the linked list
		}
		Pthread_mutex_unlock(&rmutex);//releae lock so other readers can enter
		Pthread_mutex_unlock(&readTry);//done trying to access linked list
		count = linkedlist_count(&list, iThread);
		char iReplace = iThread + '0';
		fileName[7] = iReplace;
		FILE *pFile = fopen (fileName, "a");
		fprintf(pFile, "Reader %d: Read %d: %d values ending in %d\n", iThread, i, count, iThread);
		fclose(pFile);
		//fprintf(stdout, "Reader %d: Read %d: %d values ending in %d\n", iThread, i, count, iThread);

		Pthread_mutex_lock(&rmutex); //locking exit section to avoid race condition
		readcount--; //report self as leaving

		if(readcount == 0){
			Pthread_mutex_unlock(&resource); //if last reader, must unlock linkedlist
		}
		
		Pthread_mutex_unlock(&rmutex);//release exit lock
		struct timespec tim;
			tim.tv_sec = 0;
			tim.tv_nsec = 5;
		if(nanosleep(&tim, NULL) < 0){
			fprintf(stderr, "Nano sleep system call failed \n");
			exit(1);
		}
	}
	Pthread_mutex_lock(&lastT);
	almostDone--;
	Pthread_cond_signal(&lastTCond);
	Pthread_mutex_unlock(&lastT);
}

void *handleWriters(void *arg){
	int * arr = (int *) arg;
	int linesToWrite = arr[0];
	int iThread = arr[1];
	free(arg);
	int number;
	int counter = 0;
	while (counter != linesToWrite) {
		Pthread_mutex_lock(&wmutex); //reserve entry section for writers- avoids race conditons
		writecount++; //report self as writer
		if(writecount == 1){ //check if first writer
			Pthread_mutex_lock(&readTry); //if first, lock out readers from accesing
		}
		Pthread_mutex_unlock(&wmutex);//release entry section, let other writers in
		Pthread_mutex_lock(&resource);//lock the linkedlist
		number = rand() % 100; // random num from 1 - 100
		if (number % 10 == iThread) {
			linkedlist_Insert(&list, number);
			counter += 1;
		}
		Pthread_mutex_unlock(&resource);//unlock the linkedlist
		Pthread_mutex_lock(&wmutex);//reserve exit section
		writecount--;//indicate self is leaving
		if(writecount == 0){
			Pthread_mutex_unlock(&readTry);//if last  writer, unlock for readers to use
		}
		Pthread_mutex_unlock(&wmutex); // releae exit section

		struct timespec tim; //sleep after to force thread switching
			tim.tv_sec = 0;
			tim.tv_nsec = 5;
		if(nanosleep(&tim, NULL) < 0){
				fprintf(stderr, "Nano sleep system call failed \n");
				exit(1);
		}
	}
}
