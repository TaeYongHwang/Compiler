/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"
typedef struct LineListRec{
	int lineno;
	struct LineListRec * next;
} * LineList;

typedef struct BucketListRec {
	TreeNode * node;
	LineList lines;
	int memloc;
	struct BucketListRec * next;
} * BucketList;

typedef struct ScopeListRec {
	int locNum;
	int isReceiveFun;
	char * name;
	BucketList bucket;
	struct ScopeListRec * parent;
	struct ScopeListRec * next; 	

} * ScopeList;

 
typedef struct FunListRec{
	char * funName;
	char * scopeName;
	ExpType type;
	TreeNode*  params;
	struct FunListRec * next;
} * FunList;


void makeFunScope(char * scopeName, TreeNode* node);
FunList findFunScope(char * name);


ScopeList firstScope();
ScopeList makeScope(char* preName, char * curName, int isReceiveFun);
ScopeList findScope(char * scope) ;
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void bk_insert( char * scope, TreeNode* node, int loc );
/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
BucketList bk_lookup(char * scope, TreeNode * node);
BucketList bk_lookup_excluding_parent( char * scope, TreeNode* node );
void bk_add_line(BucketList bk,int lineno );
void st_insert( char * name, int lineno, int loc );
int st_lookup ( char * name );
/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);
void printAllFunScope(FILE * listing);


#endif
