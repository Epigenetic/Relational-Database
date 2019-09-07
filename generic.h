#ifndef _generic_h
#define _generic_h

typedef union generic* generic;

enum type{character,integer,floating,string};

union generic{
	char c;
	int i;
	float f;
	char* s;
};

/*
 * Returns a new generic
 */
generic generic_new();

/*
 * Frees the given generic
 */
void generic_free(generic g);

/*
 * Converts an enum type to a string
 */
char* type_to_string(enum type t);

/*
 * Converts a string to an enum type
 */
enum type string_to_type(const char* s);

/*
 * Tests equality of two generics holding the same type
 */
int generic_equal(generic g1, generic g2, enum type t);

/*
 * Copies the value of one generic for a given type into another
 */
void generic_copy(generic g1, generic g2, enum type t);

#endif