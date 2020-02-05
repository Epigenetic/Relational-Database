#include <ctype.h>
#include <stdio.h>
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
void token_free(token t){
	if(t->next)
		token_free(t->next);
	free(t);
}

/*
 * Tokenize given command, returns head of list
 */
token tokenize(char* input){
	
	*strrchr(input,'\n') = '\0';
	
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
			new->type = select;
		}else if(!strcmp(t, "from")){
			new->type = from;
		}else if(!strcmp(t, "where")){
			new->type = where;
		}else if(!strcmp(t, "insert")){
			new->type = insert;
		}else if(!strcmp(t, "into")){
			new->type = into;
		}else if(!strcmp(t, "values")){
			new->type = values;
		}else if(!strcmp(t, ",")){
			new->type = comma;
		}else if(!strcmp(t, "(")){
			new->type = lparen;
		}else if(!strcmp(t, ")")){
			new->type = rparen;
		}else if(!strcmp(t,"'")){
			new->type = quote;
		}else{
			new->type = identifier;
		}
		//printf("token made: %s\n",new->content);
		t = strtok(NULL," ");
	}
	return head;
}

/*
 * Return a new node
 */
node node_new(){
	node n = (node)malloc(sizeof(struct node));
	n->parent = NULL;
	n->child = NULL;
	n->sibling = NULL;
	return n;
}

/*
 * Free a node
 */
 void node_free(node n){
	 if(n->child)
		 node_free(n->child);
	 if(n->sibling)
		 node_free(n->child);
	 free(n);
 }
 
/*
 * Parses command into parse tree given head of list of tokens
 * Returns root of tree
 */
 node parse(token t){
	 
	 return null;
 }

