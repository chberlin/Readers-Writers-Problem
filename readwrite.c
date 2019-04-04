#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "linkedlist.c"


int createThreads(int num, int numreaders, int numwriters);
void *lastThread(void *arg);
void *handleReaders(void * arg);
void *handleWriters(void *arg);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int rc = pthread_mutex_lock(&lock);
if(rc == 0) exit(2);
//Pthread_mutex_lock(&lock);
//pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;


linkedlist list;
linkedlist_Init(&list);

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
	return 0;
}


int createThreads(int num, int numreaders, int numwriters){
	int numThreads = numwriters + numreaders + 1;

	pthread_t threads[numThreads];
	void * retvals[numThreads];
	int i;
	for(i = 0; i < numwriters; i++){
		int *N = malloc(sizeof(num));
		*N = num;
		if(pthread_create(&threads[i], NULL, handleWriters, (int *) N) != 0){
			fprintf(stderr, "Error: Writer Thread %d cannot created\n", i);
			exit(2);
		}
	}
	int j = i;
	for(j; j < i+numreaders; j++){
		int *N = malloc(sizeof(num));
		*N = num;
		if(pthread_create(&threads[j], NULL, handleReaders, (int *) N)!= 0){
			fprintf(stderr, "Error: Reader Thread %d cannot created\n", j-i);
			exit(2);
		}
	}
	int *N = malloc(sizeof(num));
	*N = num;
	if(pthread_create(&threads[j], NULL, lastThread, (int *) N)!= 0){
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
	int num = *((int *) arg);
	free(arg);
	fprintf(stdout, "Reader %d\n", num);
	
	
}

void *handleWriters(void *arg){
	int num = *((int *) arg);
	free(arg);
	fprintf(stdout, "Writer %d\n", num);
	
}


