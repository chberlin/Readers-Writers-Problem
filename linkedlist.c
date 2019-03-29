#include <stdlib.h>
#include <pthread.h>

typedef struct node {
	int key;
	struct node *next;
} node;

typedef struct linkedlist {
	node *head;
	pthread_mutex_t lock;
} linkedlist;

void linkedlist_Init(linkedlist *L){
	L -> head = NULL;
	pthread_mutex_init(&L->lock, NULL);
}

int linkedlist_Insert(linkedlist *L, int key){
	pthread_mutex_lock(&L -> lock);
	node *new = malloc(sizeof(node));
	if(new == NULL){
		perror("Error with malloc");
		pthread_mutex_unlock(&L -> lock);
		return -1;
	}
	new->key = key;
	new-> next = L->head;
	L->head = new;
	pthread_mutex_unlock(&L->lock);
	return 0;
}

int linkedlist__count(linkedlist *L){
	pthread_mutex_lock(&L->lock);
	node *current = L->head;
	int count = 0;
	while(current){
		current = current->next;
		current++;
	}
	pthread_mutex_unlock(&L->lock);
	return count;
}