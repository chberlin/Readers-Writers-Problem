#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.c"
#include <pthread.h>

int createThreads(int numThreads, int num, void * (*start_func)(int));
//input: Number of threads to create
//input: num
//input:function pointer to a starting function for the thread to run

void *handleReaders(int num);

void *handleWriters(int num);

void *lastThread();

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
		printf("incorrect N value, choose value between 1 and 1000\n");
		exit(1);
	}

	if (numreaders < 1 || numreaders > 9) {
		printf("incorrect R value, choose value between 1 and 9\n");
		exit(1);
	}

	if (numwriters < 1 || numwriters > 9) {
		printf("incorrect W value, choose value between 1 and 9\n");
		exit(1);
	}

	//function pointer messed up
	//can't figure out proper syntax
	createThreads(numwriters, num, handleWriters);
	createThreads(numreaders, num, handleReaders);
	createThreads(1, 0, lastThread);
	
}

int createThreads(int numThreads, int num, void * (*start_func){
	pthread_t threads[numThreads];
	void * retvals[numThreads];
	int i;
	for(i = 0; i < numThreads; i++){
		if(pthread_create(&threads[i], NULL, start_func(int), num) != 0){
			fprintf(stderr, "Error: Thread % %d cannot created\n", i);
			exit(2);
		}
	}
	for(int j = 0; j < i; j++){
		if(pthread_join(threads[i], (void **)&retvals[i]) != 0){
			fprintf(stderr, "Error: Thread %d cannot join\n", j);
			exit(2);
		}
	}
	return 0;
}

void *lastThread(){
	//waits on condition variable when only single reader thread remains
	fprintf(stdout, "Almost Done!\n");
}

void *handleReaders(int num){

}

void *handleWriters(int num){

}
	
