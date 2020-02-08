#ifndef _parser_h
#define _parser_h

typedef struct token* token;
typedef struct node* node;

enum token_type {select, from, where, insert, into, values, identifier, comma, lparen, rparen, quote,equal};
enum node_type {insert_statement, select_statement, tupl, list, leaf,equality};

struct token{
	enum token_type type;
	char* content;
	token next;
	token prev;
};

struct node{
	node child; //parse tree is in left child, right sibling format
	node sibling;
	token content;
	enum node_type type;
};

/*
 * Return a new token
 */
token token_new();

/*
 * Free a token
 */
void token_free(token t);

/*
 * Tokenize given command, returns head of list
 */
token tokenize(char* input);

/*
 * Return a new node
 */
node node_new();

/*
 * Free a node
 */
 void node_free(node n);
 
/*
 * Parses command into parse tree given head of list of tokens
 * Returns root of tree
 */
 node parse(token t);

#endif