#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "generic.h"
#include "hash_table.h"
#include "tuple.h"

int char_hash(generic c, int n);
int int_hash(generic i, int n);
int float_hash(generic f, int n);
int string_hash(generic s, int n);

/*
* Returns a new hash table
*/
hash_table hash_table_new(int n, int e, enum type index){
	//printf("Size is %ld\n", sizeof(struct hash_table));
	hash_table ht = (hash_table)malloc(sizeof(struct hash_table));
	tuple* d = (tuple*)malloc(sizeof(tuple)*n);
	ht->data = d;
	for(int i = 0; i < n; i ++)
		ht->data[i] = NULL;
	ht->size = n;
	ht->element = e;
	//printf("element: %d\n",ht->element);
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

/*
* Frees a hash table and the tuples in it
*/
void hash_table_free_complete(hash_table h){
	int found = 0;
	for(int i = 0; i < h->size; i++){
		if(h->data[i] != NULL){
			tuple_free(h->data[i]);
			found++;
		}
		if(found == h->fill)
			break;
	}
	
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
	//printf("%d\n",t->data[0]->i);
	//printf("%I64lld",(long long)&t->data[h->element]);
	int hash = h->hash_func(t->data[h->element],h->size);
	//printf("to here\n");
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
* Gets a tuple from the table
*/
tuple hash_table_get(enum type t,void* key, hash_table h){
	int hash;
	int i = 0;
	tuple tu;
	switch(t){
		case character:
		hash = h->hash_func(key,h->size);
		
		tu = h->data[(hash + i + 2*i*i)%h->size];
		while(tu != NULL && tu->data[h->element]->c != *(char*)key){
			i++;
			tu = h->data[(hash+i+2*i*i)%h->size];
		}
		return tu;
		
		case integer:
		hash = h->hash_func(key,h->size);
		
		tu = h->data[(hash + i + 2*i*i)%h->size];
		while(tu != NULL && tu->data[h->element]->i != *(int*)key){
			i++;
			tu = h->data[(hash+i+2*i*i)%h->size];
		}
		return tu;
		
		case floating:
		hash = h->hash_func(key,h->size);
		
		tu = h->data[(hash + i + 2*i*i)%h->size];
		while(tu != NULL && tu->data[h->element]->f != *(float*)key){
			i++;
			tu = h->data[(hash+i+2*i*i)%h->size];
		}
		return tu;
		
		case string:
		hash = h->hash_func(key,h->size);
		
		tu = h->data[(hash + i + 2*i*i)%h->size];
		while(tu != NULL && !strcmp(tu->data[h->element]->s,(char*)key)){
			i++;
			tu = h->data[(hash+i+2*i*i)%h->size];
		}
		return tu;
		
	}
	return NULL;
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
		if(h->data[i] == NULL)
			continue;
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

int char_hash(generic c, int n){
	//printf("entered");
	return c->c % n;
}

int int_hash(generic i, int n){
	//printf("entered");
	return i->i % n;
}

int float_hash(generic f, int n){
	//printf("entered");
	return (int)(f->f) % n;
}

int string_hash(generic s, int n){
	//printf("entered");
	char* s_ = s->s;
	int sum;
	for(int i = 0; i < 3; i++){
		sum += s_[i];
		if(s_[i] == '\0')
			break;
	}
	return sum % n;
}