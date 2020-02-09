/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    ELSE = 259,
    WHILE = 260,
    RETURN = 261,
    INT = 262,
    VOID = 263,
    THEN = 264,
    END = 265,
    REPEAT = 266,
    UNTIL = 267,
    READ = 268,
    WRITE = 269,
    ASSIGN = 270,
    EQ = 271,
    NE = 272,
    LT = 273,
    LE = 274,
    GT = 275,
    GE = 276,
    TIMES = 277,
    OVER = 278,
    PLUS = 279,
    MINUS = 280,
    LPAREN = 281,
    RPAREN = 282,
    LBRACE = 283,
    RBRACE = 284,
    LCURLY = 285,
    RCURLY = 286,
    SEMI = 287,
    COMMA = 288,
    NUM = 289,
    ID = 290,
    ERROR = 291
  };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define WHILE 260
#define RETURN 261
#define INT 262
#define VOID 263
#define THEN 264
#define END 265
#define REPEAT 266
#define UNTIL 267
#define READ 268
#define WRITE 269
#define ASSIGN 270
#define EQ 271
#define NE 272
#define LT 273
#define LE 274
#define GT 275
#define GE 276
#define TIMES 277
#define OVER 278
#define PLUS 279
#define MINUS 280
#define LPAREN 281
#define RPAREN 282
#define LBRACE 283
#define RBRACE 284
#define LCURLY 285
#define RCURLY 286
#define SEMI 287
#define COMMA 288
#define NUM 289
#define ID 290
#define ERROR 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
