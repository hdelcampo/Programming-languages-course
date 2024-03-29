#include "StringMap.h"
#include <stdlib.h>
#include <string.h>

map* createMap(){
	map *newMap = malloc(sizeof(map));

	newMap->numElements = 0;
	newMap->maxElements = INIT_MAX_ELEM;
	newMap->buckets = malloc(sizeof(node) * INIT_MAX_ELEM);

	for(int i=0; i < INIT_MAX_ELEM; i++){
		newMap->buckets[i] = NULL;
	}

	return newMap;
}

iterator* createIterator(const map *map){
	iterator *newIterator = malloc(sizeof(iterator));

	newIterator->index = -1;
	newIterator->node = NULL;
	newIterator->map = map;

	return newIterator;
}

node* getNext(iterator *iterator){
	if(iterator->node != NULL 
		&& iterator->node->next != NULL){
		iterator->node = iterator->node->next;
		return iterator->node;
	}
	
	for(int i = iterator->index+1; i < iterator->map->maxElements; i++){
		if(iterator->map->buckets[i] != NULL){
			iterator->index = i;
			iterator->node = iterator->map->buckets[i];
			return iterator->node;
		}
	}

	return NULL;
}

void refactor(map *m){
	const node **oldBuck = m->buckets;
	m->maxElements *= 2;
	m->numElements = 0;
	m->buckets = malloc(sizeof(node) * m->maxElements);
	for(int i = 0; i < m->maxElements; i++){
		m->buckets[i] = NULL;
	}

	node *iter = NULL;
	for(int i = 0; i < m->maxElements/2;i++){
		iter = oldBuck[i];
		while(iter != NULL){
			addElement(m, iter->key, iter->value);
			iter = iter->next;
		}
	}
}

void addElement(map *m, const char *key, const int value){
	m->numElements++;
	if(getChargeFactor(m) > MAX_CHARGE){
		refactor(m);
	}

	const hash i = getHash(key) % m->maxElements;
	node *n = malloc(sizeof(node));
	strcpy(n->key, key);
	n->value = value;
	n->next = m->buckets[i];
	m->buckets[i] = n;
}

void removeElement(map *m, const char *key){
	const hash index  = getHash(key) % m->maxElements;
	node *bf = NULL;
	node *cnt = m->buckets[index];

	while(cnt != NULL && strcmp(key, cnt->key) != 0){
		bf = cnt;
		cnt = cnt->next;
	}
	if(cnt != NULL){
		if(bf == NULL){
			m->buckets[index] = cnt->next;
		}
		else{
			bf->next = cnt->next;
		}
		free(cnt);
		m->numElements--;
	}
}

int hasNext(const iterator *iterator){
	if(iterator->node != NULL
		&& iterator->node->next != NULL){
		return TRUE;
	}
	
	for(int i = iterator->index + 1; i < iterator->map->maxElements; i++){
		if(iterator->map->buckets[i] != NULL){
			return TRUE;
		}
	}

	return FALSE;
}

int getValue(const map *m, const char *key){
	const hash index = getHash(key) % m->maxElements;
	node *n = m->buckets[index];

	while(n != NULL && n->next != NULL && strcmp(n->key, key) != 0){
		n = n->next;
	}
	return (n != NULL && strcmp(n->key, key) == 0) ? n->value : 0;
}

float getChargeFactor(const map *m){
	return 1.0*m->numElements/m->maxElements;
}

hash getHash(const unsigned char *str){
	hash value = 5381;
	int c;

	while (c = *str++)
		value = ((value << 5) + value) + c; // value = value*33 + c

	return value;
}
