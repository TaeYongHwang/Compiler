/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "util.h"
/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4
#define ENUMTOSTR(x) #x

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
/*
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;
*/
/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
/*
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ; // memory location for variable 
     struct BucketListRec * next;
   } * BucketList;
*/
/* the hash table */
//static BucketList hashTable[SIZE];
static ScopeList allScope;
static FunList allFunScope ;

void makeFunScope(char * scopeName, TreeNode* node) {

	FunList funScope = (FunList)malloc(sizeof(struct FunListRec));
	funScope -> funName = node->attr.name;
	funScope -> scopeName = scopeName;
	funScope -> type = node->type;
	funScope -> next = NULL;
	funScope -> params = NULL;

	//if parameter exist
	if(node-> child[0] != NULL){
		funScope -> params = node->child[0];
    }


	if(allFunScope == NULL){
	  allFunScope = funScope;
    }
	else{
	  FunList temp = allFunScope;

	  while(temp -> next !=NULL)
		temp = temp->next;
	  temp->next = funScope;
	}

}

FunList findFunScope(char * name){
	FunList temp = allFunScope;

	while(temp != NULL){
	  if(strcmp(name, temp->funName) == 0)
		break;
	  temp = temp -> next;
	}


	return temp;
}










ScopeList findScope(char * scope){
  ScopeList sl = allScope;

//1. find scopelist
  while((sl != NULL) && (strcmp(scope, sl->name) !=0))
	sl = sl->next;

  return sl;
} 


ScopeList firstScope(){
	allScope = (ScopeList)malloc(sizeof(struct ScopeListRec));
	allScope ->locNum = 0;
	allScope ->isReceiveFun = 0;
	allScope ->name ="global";
	allScope ->bucket = NULL;
	allScope ->parent = NULL;
	allScope ->next = NULL;
	return allScope;
}

ScopeList makeScope(char* preName, char * curName,int isReceiveFun){
  ScopeList preSL = NULL;
  ScopeList curSL = allScope;
  

  while(curSL->next != NULL)
	curSL = curSL->next;

  if(preName != NULL){
	preSL = findScope(preName);
  }

  curSL->next = (ScopeList)malloc(sizeof(struct ScopeListRec));
  curSL->next->locNum = 0;
  curSL->next->isReceiveFun = isReceiveFun;
  curSL->next->name = curName;
  curSL->next->bucket = NULL;
  curSL->next->parent = preSL;
  curSL->next->next = NULL;

  return curSL->next;	

}


void bk_insert(char * scope,TreeNode * node, int loc){

  ScopeList sl = findScope(scope);
  if(sl == NULL){
	return;
  }
  BucketList bl = sl->bucket;

  while((bl != NULL) && (strcmp(bl->node->attr.name, node->attr.name) != 0))
	bl = bl->next;
  
  if(bl == NULL){ //make bucket (definition)
    bl = (BucketList) malloc(sizeof(struct BucketListRec));
	bl -> node = node;
	bl -> lines = (LineList) malloc(sizeof(struct LineListRec));
	bl -> lines-> lineno = node->lineno;
	bl -> lines->next = NULL;
	bl -> memloc = loc;
    bl -> next = sl -> bucket;
	sl -> bucket = bl;
  }else{ //just add linenum (use)
	LineList t = bl->lines;
	while(t->next !=NULL)
	  t = t->next;
	t->next = (LineList) malloc(sizeof(struct LineListRec));
	t->next->lineno = node->lineno;
	t->next->next = NULL;
	}
}


void st_insert( char * name, int lineno, int loc )
{
/*
  int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) 
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l; }
  else
  { LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  } */
} /* st_insert */


/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */

BucketList bk_lookup (char * scope, TreeNode * node){
  ScopeList sl =findScope(scope);
  BucketList bl = NULL;
  
  while(sl != NULL){
    if( (bl =bk_lookup_excluding_parent(sl->name, node)) != NULL)
      break;
    sl = sl->parent;
  }

  return bl;
}

BucketList bk_lookup_excluding_parent(char * scope, TreeNode * node ){
  ScopeList sl = findScope(scope);
  BucketList bl = sl->bucket;
  while((bl != NULL) &&  (strcmp(bl->node->attr.name,node->attr.name) != 0))
	bl = bl->next;
 
  return bl;

}

void bk_add_line(BucketList bk,int lineno ){
	LineList l = bk->lines;
	while( l->next !=NULL)
	  l = l->next;
	l-> next = (LineList) malloc(sizeof(struct LineListRec));
	l = l-> next;
	l->lineno = lineno;
	l->next = NULL;


}



int st_lookup ( char * name )
{ 
/*
  int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
*/
}


/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */



void printBucket(ScopeList curScope,BucketList bucket){
	TreeNode* t = bucket->node;
	LineList ll = bucket->lines;

	fprintf(listing,"%-13s", t->attr.name );
	if(t->nodekind == StmtK){
	  fprintf(listing,"%-13s",ENUMTOSTR(stmt));
	}
	else
	  fprintf(listing,"%-13s",ENUMTOSTR(exp));

	fprintf(listing,"%-10s", curScope->name);
	fprintf(listing,"%-10d", bucket->memloc);

	while(ll != NULL) {
	  fprintf(listing,"%4d", ll->lineno);
	  ll = ll->next;
	}
	fprintf(listing,"\n");

}



void printSymTab(FILE * listing)
{ int i;
	
  fprintf(listing,"Variable Name  Variable Type  Scope Name   Location   Line Numbers\n");
  fprintf(listing,"-------------  -------------  ----------   --------   ------------\n");
  
  ScopeList curScope = allScope;

  while(curScope!= NULL){
	BucketList curBucket = curScope->bucket;
	while(curBucket!=NULL){
	  printBucket(curScope, curBucket);
	  curBucket = curBucket->next;
	}
    curScope = curScope->next;
  }


} /* printSymTab */


void printAllFunScope(FILE * listing) {
  FunList funScope = allFunScope;

  fprintf(listing,"Function Name  Scope Name  Function Type \n");
  fprintf(listing,"-------------  ----------  ----------------\n");
  

  while(funScope!= NULL){
	fprintf(listing,"%-13s", funScope->funName);
	fprintf(listing,"%-13s", funScope->scopeName);
	if(funScope->type == Integer)
	  fprintf(listing,"Integer");
	if(funScope->type == Void)
	  fprintf(listing,"Void");
	fprintf(listing,"\n");
	funScope = funScope->next;
  }

}


















