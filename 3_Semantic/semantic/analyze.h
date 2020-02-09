/****************************************************/
/* File: analyze.h                                  */
/* Semantic analyzer interface for TINY compiler    */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _ANALYZE_H_
#define _ANALYZE_H_

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *);
void initScope();
static void closeScope(TreeNode * t);
static void insertNode( TreeNode * t);
static void typeError(TreeNode * t, char * message); 
/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *);

#endif