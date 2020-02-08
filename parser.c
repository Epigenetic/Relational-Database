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
		if(!strcmp(t,"select"))
			new->type = select;
		else if(!strcmp(t, "from"))
			new->type = from;
		else if(!strcmp(t, "where"))
			new->type = where;
		else if(!strcmp(t, "insert"))
			new->type = insert;
		else if(!strcmp(t, "into"))
			new->type = into;
		else if(!strcmp(t, "values"))
			new->type = values;
		else if(!strcmp(t, ","))
			new->type = comma;
		else if(!strcmp(t, "("))
			new->type = lparen;
		else if(!strcmp(t, ")"))
			new->type = rparen;
		else if(!strcmp(t,"'"))
			new->type = quote;
		else if(!strcmp(t,"="))
			new->type = equal;
		else
			new->type = identifier;
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
	n->child = NULL;
	n->sibling = NULL;
	n->content = NULL;
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

//current head of token list as it is parsed 
token lis;
 
 node parse_select(node n);
 node parse_insert(node n);
 node parse_list(node n);
 node parse_equal(node n);
 
/*
 * Parses command into parse tree given head of list of tokens
 * Returns root of tree
 */
 node parse(token t){
	 lis = t;
	 if(lis->type == select){
		node n = node_new();
		n->type = select_statement;
		if(parse_select(n))
			return n;
		else
			node_free(n);
	 }else if(t->type == insert){
		 node n = node_new();
		 n->type = insert_statement;
		 if(parse_insert(n))
			 return n;
		 else
			 node_free(n);
	 }
	 //illegal statement
	return NULL;

 }
 
 node parse_select(node n){
	 node nn = node_new();//Safe operation since we test that first token is select in parse()
	 nn->type = leaf;
	 nn->content = lis;
	 n->child = nn;
	 lis = lis->next;
	 if(!lis){ //unexpected end of token stream
		 node_free(nn);
		 return NULL;
	 }
	 
	 node l = node_new();
	 l->type = list;
	 nn->sibling = l;
	 if(!parse_list(l)){
		node_free(nn);
		return NULL; 
	 }
	 
	 if(lis->type != from){
		 node_free(nn);
		 return NULL;
	 }
	 node in = node_new();
	 in->type = leaf;
	 in->content = lis;
	 l->sibling = in;
	 lis = lis->next;
	 if(!lis){ //unexpected end of token stream
		 node_free(nn);
		 return NULL;
	 }
	 
	 if(lis->type != identifier){
		 node_free(nn);
		 return NULL;
	 }
	 node table = node_new();
	 table->type = leaf;
	 table->content = lis;
	 in->sibling = table;
	 lis = lis->next;
	 if(!lis) //select without where component
		 return nn;
	 
	 if(lis->type != where){
		 node_free(nn);
		 return NULL;
	 }
	 node wh = node_new();
	 wh->type = leaf;
	 wh->content = lis;
	 table->sibling = wh;
	 lis = lis->next;
	 if(!lis){ //unexpected end of token stream
		 node_free(nn);
		 return NULL;
	 }
	 
	 node tu = node_new();
	 tu->type = equality;
	 wh->sibling = tu;
	 if(!parse_equal(tu)){
		 node_free(nn);
		 return NULL;
	 }
	 return nn;
 }
 
 node parse_insert(node n){
	 return NULL;
 }
 
 node parse_list(node n){
	 return NULL;
 }
 
 node parse_equal(node n){
	 return NULL;
 }

