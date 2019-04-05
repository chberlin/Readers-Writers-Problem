#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
linkedlist list;

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

	createThreads(num, numreaders, numwriters);
	linkedlist_Init(&list);
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
	fprintf(stdout, "Almost Done!\n");
	free(arg);
}

void *handleReaders(void * arg){
	
	int * arr = (int *) arg;

	int linesToRead = arr[0];
	int iThread = arr[1];
	printf("num Readers = %d\n", num);
	printf("reader i, where i = %d\n", iThread);

	free(arg);
	Pthread_mutex_lock(&lock);
	Pthread_mutex_unlock(&lock);
	
	
}

void *handleWriters(void *arg){

	int * arr = (int *) arg;

	int linesToWrite = arr[0];
	int iThread = arr[1];
	printf("num writers = %d\n", num);
	printf("writer i, where i = %d\n", iThread);

	free(arg);

	int number = rand() % 100; // random num from 1 - 100
	if (number % 10 == iThread) {
		linkedlist_Insert(&list, number);
	}

	int number;
	int counter = 0;
	
	while (counter != linesToWrite) {

		number = rand() % 100; // random num from 1 - 100
		if (number % 10 == iThread) {
			linkedlist_Insert(&list, number);
			counter += 1;
	
			// now, make the thread wait for a bit
			
		}

	}


}


