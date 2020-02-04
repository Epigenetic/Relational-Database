#ifndef _parser_h
#define _parser_h

typedef struct token* token;

enum token_type {select, from, where, insert, into, values, identifier, comma, lparen, rparen, quote};

struct token{
	enum token_type type;
	char* content;
	token next;
	token prev;
};

/*
 * Return a new token
 */
token token_new();

/*
 * Free a token
 */
void token_free();

/*
 * Tokenize given command, returns head of list
 */
token tokenize(char* input);

#endif