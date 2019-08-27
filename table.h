#ifndef _table_h
#define _table_h

#include "hash_table.h"
#include "tuple.h"
#include "generic.h"

typedef struct table* table;

/*
* data_size is number of hash tables
* data is the hash tables
* scheme_size is the size of the scheme
* scheme_type is an array indicating the type of data held in each position of the tuple
* scheme_labels is the corresponding labels to the each item in the tuple
*/
struct table{
	int data_size;
	hash_table* data;
	int scheme_size;
	enum type* scheme_type;
	char** scheme_labels;
	char** index_labels;
};

/*
* Creates and returns a new table with the appropriate tuple properties
* Creates a hash_table to index the entries for each nonzero position (there must be at least one index)
* The position in the is_index array corresponds to the label in the scheme at the same position
*/
table table_new(int scheme_size, char* scheme_labels[], enum type scheme_type[], int is_index[]);

/*
* Frees the given table
*/
void table_free(table t);

/*
* Inserts a new tuple into the table, number is number of items in the tuple
* Nothing will happeen if incorrect number of arguments given
*/
void table_insert(int num, table t, ...);

/*
* Gets the appropriate tuple(s) from using the appropriate table
*/
table table_select(char* label, void* key, table t);

/*
* Projects a table on the given column(s)
*/
table table_project(char* label[], int num, table t);

/*
* Joins two tables on the given item(s)
* Column(s) to join on are given in the label_pairs such that label_pairs[0][0] goes with label_pairs[1][0], [0][1] with [1][1], etc.
* Label in the new table will be the name of the same item in the first table unless it is not present there (in which case it is the label of the second table)
* Tuples will have the following scheme: (JoinedItems, t1Items-JoinedItems,t2Items-JoinedItems)
*/
table table_join(char*** label_pairs, int num, table t1, table t2);

/*
* Copies a table's scheme into a new one
*/
table table_copy_scheme(table t);

/*
* Inserts a given tuple into the table (assumes tuple is of correct scheme)
*/
void table_insert_tuple(tuple tu, table ta);

/*
* Prints a given table (prints the scheme, the index labels, and all the tuples)
*/
void table_print(table t);

#endif