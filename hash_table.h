#ifndef _hash_table_h
#define _hash_table_h

#include "tuple.h"
#include "generic.h"

typedef struct hash_table* hash_table;

/*
* Size is current size of data array
* Fill is how many elements of that array are currently full
* Element is which element of the tuple is being hashed on
* Data is the array of data
* Hash_func is the hash funciton for the appropriate data type
*/
struct hash_table{
	int size;
	int fill;
	int element;
	tuple* data;
	int (* hash_func)(generic,int);
};

/*
* Returns a new hash table
*/
hash_table hash_table_new(int n, int e, enum type index);

/*
* Frees a hash table
*/
void hash_table_free(hash_table h);

/*
* Frees a hash table and the tuples in it
*/
void hash_table_free_complete(hash_table h);

/*
* Inserts a new tuple into the hash table
*/
void hash_table_insert(tuple t, hash_table h);

/*
* Gets a tuple from the has table
*/
tuple hash_table_get(enum type t,void* key, hash_table h);

/*
* Checks each element in the table for matching the key and returns all of them
*/
tuple* hash_table_brute(enum type t, void* key, int position, hash_table h);

#endif