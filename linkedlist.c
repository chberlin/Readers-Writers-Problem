#include <stdlib.h>
#include <pthread.h>

typedef struct node {
	int key;
	struct node *next;
} node;

typedef struct linkedlist {
	node *head;
	pthread_mutex_t lock;
	int size;
} linkedlist;

void linkedlist_Init(linkedlist *L){
	L -> head = NULL;
	L -> size = 0;
	pthread_mutex_init(&L->lock, NULL);
}
void linkedlist_Destroy(linkedlist *L){
	node* current = L->head;
	while(current != NULL){
		node* next = current -> next;
		free(current);
		current = next;
		L->size-=1;
	}
}
		

int linkedlist_Insert(linkedlist *L, int value){
	pthread_mutex_lock(&L -> lock);
	node *newNode = malloc(sizeof(node));
	if(newNode == NULL){
		perror("Error with malloc");
		pthread_mutex_unlock(&L -> lock);
		exit(2);
	}
	newNode->key = value;
	newNode->next = NULL;
	node *current = L->head;
	while(current != NULL){
		current = current->next;
	}
	current = newNode;
	L->size+=1;
	pthread_mutex_unlock(&L->lock);
	return 0;
}

int linkedlist_count(linkedlist *L, int iThread){
	//pthread_mutex_lock(&L->lock);
	node *current = L->head;
	int count = 0;
	while(current != NULL){
		if((current -> key) % 10 == iThread){
			count++;
		}
		current = current->next;
	}
	//pthread_mutex_unlock(&L->lock);
	return count;
}
