#include "list.h"
#include <stdlib.h>

/*****DEFINICIONES TIPOS DE dataS*****/

typedef struct node {
	void *data;
	struct node* ptr;
} node_t;


struct list {
	node_t* first;
	node_t* last;
	size_t size;
};

struct list_iter {
	list_t* list;
	node_t* previous;
	node_t* actual;
};

#include <stdint.h>

/*****AUXILIARY FUNCTIONS*****/
node_t* node_new(void* data, node_t* previous, node_t* next) {
	node_t* node=malloc(sizeof(node_t));
	if(!node)
		return NULL;
	node->data=data;
	node->ptr=(void *)((uintptr_t)previous ^ (uintptr_t)next);
	return node;
}

void update_ptr(node_t* node, node_t* previous, node_t* next) {
	node->ptr=(void *)((uintptr_t)previous ^ (uintptr_t)next);
}

/*****INTERFACE FUNCTIONS*****/

list_t *list_create(void) {
	list_t* aux=malloc(sizeof(list_t));
	if(!aux)
		return NULL;	
	aux->first=NULL;
	aux->last=NULL;
	aux->size=0;
	return aux;
}

bool list_is_empty(const list_t *list) {
	return (!list->size);
}

bool list_insert_first(list_t *list, void *data) {
	node_t* aux=NULL;
	if(list_is_empty(list)) {
		aux=node_new(data,NULL,NULL);
		if(!aux)
			return false;
		list->last=aux;
	}
	else {
		aux=node_new(data,NULL,list->first);
		if(!aux)
			return false;
		update_ptr(list->first,aux,list->first->ptr);
	}
	list->size++;
	list->first=aux;
	return true;
}

bool list_insert_last(list_t *list, void *data) {	
	node_t* aux=NULL;	
	if(list_is_empty(list)) {
		aux=node_new(data,NULL,NULL);
		if(!aux)
			return false;
		list->first=aux;
	}		
	else {
		aux=node_new(data,list->last,NULL);
		if(!aux)
			return false;
		update_ptr(list->last,list->last->ptr,aux);
	}			
	list->last=aux;	
	list->size++;
	return true;
}

void* list_delete_first(list_t *list) {
	if(list_is_empty(list))
		return NULL;
	list->size--;
	node_t* aux=list->first;
	list->first=list->first->ptr;
	if(list_is_empty(list))
		list->last=NULL;
	else
		list->first->ptr=(void*)((uintptr_t)list->first->ptr^(uintptr_t)aux);
	void* data=aux->data;
	free(aux);	
	return data;
}

void* list_delete_last(list_t *list) {
	if(list_is_empty(list))
		return NULL;
	list->size--;
	node_t* aux=list->last;
	list->last=list->last->ptr;
	if(list_is_empty(list))
		list->first=NULL;
	else
		list->last->ptr=(void*)((uintptr_t)list->last->ptr^(uintptr_t)aux);
	void* data=aux->data;
	free(aux);	
	return data;
}

void *list_peek_first(const list_t *list) {
	return(list_is_empty(list)? NULL: list->first->data);
}

void *list_peek_last(const list_t *list) {
	return(list_is_empty(list)? NULL: list->last->data);
}

size_t list_length(const list_t *list) {
	return list->size;
}

void list_destroy(list_t *list, void destroy_data(void *)) {
	while(!list_is_empty(list)) {
		if(!destroy_data)
			list_delete_first(list);
		else 
			destroy_data(list_delete_first(list));		
	}
	free(list);
}

/*****ITERATION FUNCTIONS*****/

list_iter_t* list_iter_create_start(list_t *list) {
	list_iter_t *aux=malloc(sizeof(list_iter_t));
	if(!aux)
		return NULL;
	aux->list=list;
	aux->previous=NULL;
	aux->actual=list->first;
	return aux;
}

list_iter_t *list_iter_create_end(list_t *list) {
	list_iter_t *aux=malloc(sizeof(list_iter_t));
	if(!aux)
		return NULL;
	aux->list=list;
	aux->previous=list->last;
	aux->actual=NULL;
	return aux;
}

bool list_iter_forward(list_iter_t *iter) {
	if(list_iter_at_last(iter))
		return false;
	node_t* aux=iter->actual;
	iter->actual=(void *)((uintptr_t)iter->previous ^ (uintptr_t)iter->actual->ptr);
	iter->previous=aux;	
	return true;
}

bool list_iter_backward(list_iter_t *iter) {
	if(list_iter_at_start(iter))
		return false;
	node_t* aux=iter->previous;
	iter->previous=(void *)((uintptr_t)iter->actual ^ (uintptr_t)iter->previous->ptr);
	iter->actual=aux;	
	return true;
}


void* list_iter_peek_current(const list_iter_t *iter) {
	return(list_iter_at_last(iter)? NULL: iter->actual->data);
}

bool list_iter_at_start(const list_iter_t *iter) {
	return (iter->previous==NULL);
}

bool list_iter_at_last(const list_iter_t *iter) {
	return (iter->actual==NULL);
}

void list_iter_destroy(list_iter_t *iter) {
	free(iter);
}

//*****FUNCTIONS WITH ITERATOR*****//

bool list_iter_insert(list_iter_t *iter, void *data) {
	node_t* aux=node_new(data,iter->previous,iter->actual);
	if(!aux)
		return false;
	if(!list_iter_at_start(iter))
		iter->previous->ptr=(void*)((uintptr_t)iter->previous->ptr^(uintptr_t)iter->actual^(uintptr_t)aux);
	if(!list_iter_at_last(iter))
		iter->actual->ptr=(void*)((uintptr_t)iter->actual->ptr^(uintptr_t)iter->previous^(uintptr_t)aux);
	iter->list->size++;

	iter->actual=aux;

	if(aux->ptr==iter->previous)
		iter->list->last=aux;
	if(iter->previous==NULL)
		iter->list->first=aux;

	return true;
}

void* list_iter_delete(list_iter_t *iter) {

	if(list_iter_at_last(iter))
		return NULL;

	void* aux=iter->actual->data;
	node_t* freed=iter->actual;

	iter->actual=(void*)((uintptr_t)iter->actual->ptr^(uintptr_t)iter->previous);
	
	if(!list_iter_at_last(iter))
		iter->actual->ptr=(void*)((uintptr_t)iter->actual->ptr^(uintptr_t)freed^(uintptr_t)iter->previous);

	if(!list_iter_at_start(iter))
		iter->previous->ptr=(void*)((uintptr_t)iter->previous->ptr^(uintptr_t)freed^(uintptr_t)iter->actual);
	
	free(freed);	
	iter->list->size--;

	if(iter->actual==NULL)
		iter->list->last=iter->previous;

	if(iter->previous==NULL)
		iter->list->first=iter->actual;

	return aux;
}

void list_iterate_start(list_t *list, bool (*callback)(void *data, void *extra), void *extra) {
	list_iter_t* iter=list_iter_create_start(list);
	while(!list_iter_at_last(iter) && callback(iter->actual->data,extra)) {
		list_iter_forward(iter);
	}
	list_iter_destroy(iter);
}

void list_iterate_end(list_t *list, bool (*callback)(void *data, void *extra), void *extra) {
	list_iter_t* iter=list_iter_create_end(list);
	while(!list_iter_at_start(iter) && callback(iter->actual->data,extra)) {
		list_iter_backward(iter);
	}
	list_iter_destroy(iter);
}