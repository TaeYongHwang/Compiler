/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* counter for variable memory locations */
static int location = 0;

ScopeList curScope;
FunList curFunScope;
int comId = 0;
/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

static void setCurFunScope(TreeNode * t){
	//fun_scope find
	if(t->nodekind == StmtK && t->kind.stmt == FunK)
	  curFunScope = findFunScope(t->attr.name);


}


/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */




void initScope(){

	curScope = firstScope();
	TreeNode * temp, *param;

//-------input--------------------------------------
	temp = newStmtNode(FunK);
	temp->attr.name = "input";
	temp->lineno = 0;
	temp->type = Integer;
	bk_insert("global", temp, curScope->locNum++);
	makeFunScope("global", temp);


//------output------------------------------------------
	temp = newStmtNode(FunK);
	temp->attr.name = "output";
	temp->lineno = 0;
	temp->type = Void;

	param = newStmtNode(ParamK);
	param->type = Integer;
	param->attr.name = "param";
	param->lineno = 0;	
	temp->child[0] = param;


	bk_insert("global", temp,  curScope->locNum++);
	makeFunScope("global", temp);







}




static void closeScope(TreeNode * t){
	if(t->nodekind == StmtK && t->kind.stmt == CompoundK && curScope->isReceiveFun==0){
		curScope = curScope->parent;
	}
	else if(t->nodekind == StmtK && t->kind.stmt == FunK){
		curScope = curScope->parent;
	}
	

}



static void insertNode( TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      {
		case FunK:
			//1. fun add to global scope
			bk_insert(curScope->name ,t, curScope->locNum++);
			makeFunScope(curScope->name, t);
			//2. change scope
			curScope = makeScope(curScope->name, t->attr.name, 1);
			break;		

		case CompoundK:
		//1. fun check.
			if(curScope->isReceiveFun !=1) {
		//if not fun
				char tempStr[100] = "compound";
				char tempStr2[10];
				snprintf(tempStr2, sizeof(tempStr2),"%d",comId);
				comId++;
				strcat(tempStr, tempStr2); 				
				curScope = makeScope(curScope->name, tempStr ,0);
			}
			break;

		case VarK:
			if(t->type == Void){
			  typeError(t,"Void type can't use to VarK");
			  break;
			}
			if(bk_lookup_excluding_parent(curScope ->name, t) == NULL)
			  bk_insert(curScope->name, t, curScope->locNum++);
		    else
			  typeError(t,"varble name already exist");
			break;

		case VarArrayK:
			if(t->type == Void){
			  typeError(t,"Void type can't use to VarArrayK");		
			  break;
			}
			if(bk_lookup_excluding_parent(curScope->name ,t) == NULL)
			  bk_insert(curScope->name, t, curScope->locNum++);
			else
			  typeError(t, "varArr name already exist");		
			t->type = IntArr;
			
		
			break;

		case ParamK:{
			TreeNode * param = t;
			if(t->type == Void){
			  typeError(t,"Void type can't use to Param(Arr)K");
			  break;
			}
			if(bk_lookup_excluding_parent(curScope->name, param) ==NULL)
			  bk_insert(curScope->name,  param, curScope->locNum++);
			else
			  typeError(t, "paramer name already exist");
			break;
			} 		
	
		case ParamArrayK:{
			TreeNode * param = t;
			if(t->type == Void){
			  typeError(t,"Void type can't use to Param(Arr)K");
			  break;
			}
			if(bk_lookup_excluding_parent(curScope->name, param) ==NULL)
			  bk_insert(curScope->name,  param, curScope->locNum++);
			else{
			  typeError(t, "paramer name already exist");
			  break;
			}

			t->type = IntArr;			

			break;
			} 			
	

	


        default:
          break;
      }
      break;


    case ExpK:
      switch (t->kind.exp)
      {

		case IdK:
		//1. def exist check
		 if(bk_lookup(curScope->name, t) == NULL)
			typeError(t," IdK not defined!");	
		//2. follow def type 
		else{			

		  if(t->child[0] == NULL){
			t->type = bk_lookup(curScope->name,t)->node->type;	
		
		  } else{
			if(bk_lookup(curScope->name,t)->node->type == IntArr)
				t->type = Integer;
			else
			  typeError(t,"not valid Integer Array Access : type is not IntArr");
		  }
   		 }

		  break;
/*		
		case CallK:
		  if((temBK = bk_lookup(curScope->name,t)) != NULL){
		//	bk_add_line(temBK, t->lineno);
		  }

		break;
*/

        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ 
  initScope();

  traverse(syntaxTree,insertNode, closeScope );
/*
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
	printAllFunScope(listing);
  }
*/

}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */

static void checkNode(TreeNode * t)
{ switch (t->nodekind)
  { case ExpK:
      switch (t->kind.exp)
      {
		case OpK:
		  if( (t->child[0]->type !=Integer) || (t->child[1]->type != Integer)){
			typeError(t, "Op applied to non-integer");
		    break;
		  }
		  t->type = Integer;
		  break;


		case ConstK:
		  t->type = Integer;
		  break;
/*
		case IdK:
		  //single variable
		  t->type = Integer;
		  fprintf(listing,"var name : %s\n", t->attr.name);  
		  break;
*/
		case ArraySizeK:
		  t->type = IntArr;
		  break;


		case CallK:{

		int count = 0;
		FunList funScope = findFunScope(t->attr.name);
		TreeNode * criteria,* data;
		//1. def check
		if(bk_lookup(curScope->name, t)  == NULL)
		  typeError(t, " function declaration is not defined");


		//2. parameters check (type, number)
		criteria = funScope->params;
		data = t->child[0];

		while(criteria != NULL && data!=NULL){
		  if(criteria->type != Integer || data->type != Integer)
			typeError(t, "parater type mismatch");
		  criteria = criteria->sibling;
		  data = data->sibling;
		
		}

		if(!(criteria== NULL && data ==NULL)){ 
		  typeError(t, "call parameter number mismatch");
		}
		t->type = funScope->type;


		break;
		}




        default:
          break;
      }
      break;
    case StmtK:	
      switch (t->kind.stmt)
      {

		case IfK:
		case RepeatK:
		  if (t->child[0]->type != Integer)
			typeError(t->child[0], "if test is not integer");
		  break;

		case ReturnK:
		//return ; form
		  if(t->child[0] == NULL){
			if(curFunScope->type != Void)
			  typeError(t, "fun type is not VOID");
		  } else{
		//return X; form
			//1. if X is function
			if(t->child[0]->nodekind == ExpK && t->child[0]->kind.exp == CallK){
				if(curFunScope->type != findFunScope(t->child[0]->attr.name)->type)
				  typeError(t, "fun return(function) conflict!");			
	
			}
			//2. else Id or Const
			else{
	       	  if(curFunScope->type != t->child[0]->type){
			    typeError(t, "fun return(ID,NUM) conflist!");
			  }
			}
		  }

		  break;
		 	

		case AssignK:
		  if(!(t->child[0]->type ==Integer && t->child[1]->type ==Integer)){
			typeError(t, "Assign can't enable, LHS type != RHS type (enable :integer)");

		  }

		  break;

		
/*
	    case IfK:
          if (t->child[0]->type == Integer)
            typeError(t->child[0],"if test is not Boolean");
          break;
        case AssignK:
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"assignment of non-integer value");
          break;
        case WriteK:
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"write of non-integer value");
          break;
        case RepeatK:
          if (t->child[1]->type == Integer)
            typeError(t->child[1],"repeat test is not Boolean");
          break;

*/
        default:
          break;
      }
      break;
    default:
      break;

  }
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree)
{ traverse(syntaxTree,setCurFunScope,checkNode);
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
	printAllFunScope(listing);
  }
}
