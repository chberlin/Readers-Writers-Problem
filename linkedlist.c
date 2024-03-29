#include <stdlib.h>
#include <pthread.h>

typedef struct node {
	int key;
	struct node *next;
} node;

typedef struct linkedlist {
	node *head;
	int size;
} linkedlist;

void linkedlist_Init(linkedlist *L){
	L -> head = NULL;
	L -> size = 0;
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
		

void linkedlist_Insert(linkedlist *L, int value){
	node *newNode = malloc(sizeof(node));
	if(newNode == NULL){
		perror("Error with malloc");
		exit(2);
	}
	newNode->key = value;
	newNode->next =L-> head;
	L->head = newNode;
	L->size++;
	return;
}

int linkedlist_count(linkedlist *L, int iThread){

	node *current = L->head;
	int count = 0;
	while(current != NULL){
		if(((current -> key) % 10) == iThread){
			count++;
		}
		current = current->next;
	}
	return count;
}

void linkedlist_printAll(linkedlist *L){

	node *current = L->head;
	while(current != NULL){
		//if(((current -> key) % 10) == iThread){
		//	count++;
		//}
		printf("[%d]", current->key);
		current = current->next;
	}
	printf("\n");
}
