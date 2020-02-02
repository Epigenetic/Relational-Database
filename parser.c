#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/*
 * Return a new token
 */
token token_new(){
	token t = (token)malloc(sizeof(struct token));
	t->content = NULL;
	t->prev = NULL;
	t->next = NULL;
	return t;
}

/*
 * Free a token
 */
void token_free(){
	
}

/*
 * Tokenize given command, returns head of list
 */
token tokenize(char* input){
	
	//all strings processed case insensitive
	for(int i = 0; input[i]; i++)
		input[i] = tolower(input[i]);
	
	char* t;
	token head = NULL, tail = NULL;
	t = strtok(input, " ");
	while(t != NULL){
		token new = token_new();
		
		if(head == NULL){
			head = new;
			tail = new;
		}else{
			tail->next = new;
			new->prev = tail;
			tail = new;
		}
		
		new->content = t;
		if(!strcmp(t,"select")){
			t->type = select;
		}else if(!strcmp(t, "from")){
			t->type = from;
		}else if(!strcmp(t, "where")){
			t->type = where;
		}else if(!strcmp(t, "insert")){
			t->type = insert;
		}else if(!strcmp(t, "into")){
			t->type = into;
		}else if(!strcmp(t, "values")){
			t->type = values;
		}else if(!strcmp(t, ",")){
			t->type = comma;
		}else if(!strcmp(t, "(")){
			t->type = lparen;
		}else if(!strcmp(t, ")")){
			t->type = rparen;
		}else{
			t->type = identifier;
		}
	}
	return head;
}

from,where,insert,into,values,identifier,comma