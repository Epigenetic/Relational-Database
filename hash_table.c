#include <stdlib.h>
#include <string.h>

#include "generic.h"
#include "hash_table.h"
#include "tuple.h"

int char_hash(void* c, int n);
int int_hash(void* i, int n);
int float_hash(void* f, int n);
int string_hash(void* s, int n);

/*
* Returns a new hash table
*/
hash_table hash_table_new(int n, int e, enum type index){
	hash_table ht = (hash_table)malloc(sizeof(struct hash_table));
	tuple d[n];
	for(int i = 0; i < n; i ++)
		d[i]=NULL;
	ht->data = d;
	ht->size = n;
	ht->element = e;
	ht->fill = 0;
	switch(index){
		case character:
		ht->hash_func = &char_hash;
		break;
		
		case integer:
		ht->hash_func = &int_hash;
		break;
		
		case floating:
		ht->hash_func = &float_hash;
		break;
		
		case string:
		ht->hash_func = &string_hash;
		break;
	}
	return ht;
}

/*
* Frees a hash table
*/
void hash_table_free(hash_table h){
	free(h->data);
	free(h);
}

tuple* increase_size(hash_table h){
	tuple* d = (tuple*)malloc(sizeof(tuple)*h->size*2);
	for(int i = 0; i < h->size*2;i++)
		d[i] = NULL;
	for(int i = 0; i < h->size; i++){
		if(h->data[i] == NULL)
			continue;
		int hash = h->hash_func(h->data[i]->data[h->element],h->size);
		if(d[hash] == NULL){
			d[hash] = h->data[i];
		}else{
			int i = 0;
			do{
				i++;
				hash = (hash + i + 2*i*i)%h->size;
			}while(d[hash] != NULL);
			d[hash]=h->data[i];
		}
	}
	return d;
}

/*
* Inserts a new tuple into the hash table element indicates which one is being hashed on
*/
void hash_table_insert(tuple t, hash_table h){
	int hash = h->hash_func(t->data[h->element],h->size);
	if(h->data[hash] == NULL){
		h->data[hash] = t;
	}else{
		int i = 1;
		hash = (hash + i + 2*i*i)%h->size;
		while(h->data[hash] != NULL){
			i++;
			hash = (hash + i + 2*i*i)%h->size;
		}
		h->data[hash]=t;
	}
	h->fill++;
	if((float)h->fill/h->size > .5){
		tuple* d = increase_size(h);
		free(h->data);
		h->data = d;
	}
}

/*
* Gets a tuple from the has table
*/
tuple hash_table_get(enum type t,void* key, hash_table h){
	int hash;
	switch(t){
		case character:
		hash = h->hash_func(key,h->size);
		break;
		
		case integer:
		hash = h->hash_func(key,h->size);
		break;
		
		case floating:
		hash = h->hash_func(key,h->size);
		break;
		
		case string:
		hash = h->hash_func(key,h->size);
		break;
		
	}
	
	int i = 0;
	tuple tu = h->data[(hash + i + 2*i*i)%h->size];
	while(tu != NULL && tu->data[h->element] != key){
		i++;
		tu = h->data[(hash+i+2*i*i)%h->size];
	}
	return tu;
}

/*
* Checks each element in the table for matching the key and returns all of them
*/
tuple* hash_table_brute(enum type t, void* key, int position, hash_table h){
	tuple* rl = malloc(sizeof(tuple)*h->size/2);
	for(int i = 0; i < h->size/2; i++)
		rl[i] = NULL;
	int fill = 0;
	for(int i = 0; i < h->size/2; i++){
		switch(t){
			case character:
			if(h->data[i]->data[position]->c == *((char*)key))
				rl[fill++] = h->data[i];
			break;
			
			case integer:
			if(h->data[i]->data[position]->i == *((int*)key))
				rl[fill++] = h->data[i];
			break;
			
			case floating:
			if(h->data[i]->data[position]->f == *((float*)key))
				rl[fill++] = h->data[i];
			break;
			
			case string:
			if(!strcmp((char*)key,h->data[i]->data[position]->s))
				rl[fill++] = h->data[i];
			break;
		}
	}
	return rl;
	
}

int char_hash(void* c, int n){
	return *((char*)c) % n;
}

int int_hash(void* i, int n){
	return *((int*)i) % n;
}

int float_hash(void* f, int n){
	return (int)*((float*)f) % n;
}

int string_hash(void* s, int n){
	char* s_ = (char*) s;
	int sum;
	for(int i = 0; i < 3; i++){
		sum += s_[i];
		if(s_[i] == '\0')
			break;
	}
	return sum % n;
}