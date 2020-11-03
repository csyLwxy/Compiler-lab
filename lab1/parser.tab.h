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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    INT = 258,
    ID = 259,
    RELOP = 260,
    TYPE = 261,
    ARRAY = 262,
    FLOAT = 263,
    CHAR = 264,
    STRING = 265,
    LP = 266,
    RP = 267,
    LB = 268,
    RB = 269,
    LC = 270,
    RC = 271,
    SEMI = 272,
    COMMA = 273,
    FOR_1 = 274,
    ASSIGNOP = 275,
    PLUS = 276,
    MINUS = 277,
    STAR = 278,
    DIV = 279,
    AND = 280,
    OR = 281,
    DOT = 282,
    NOT = 283,
    STRUCT = 284,
    RETURN = 285,
    BREAK = 286,
    CONTINUE = 287,
    IF = 288,
    ELSE = 289,
    WHILE = 290,
    FOR = 291,
    SELFADD = 292,
    SELFDEC = 293,
    ADD_ASSIGNOP = 294,
    MINUS_ASSIGNOP = 295,
    STAR_ASSIGNOP = 296,
    DIV_ASSIGNOP = 297,
    EXT_DEF_LIST = 298,
    EXT_VAR_DEF = 299,
    FUNC_DEF = 300,
    FUNC_DEC = 301,
    EXT_DEC_LIST = 302,
    PARAM_LIST = 303,
    PARAM_DEC = 304,
    VAR_DEF = 305,
    DEC_LIST = 306,
    DEF_LIST = 307,
    COMP_STM = 308,
    STM_LIST = 309,
    EXP_STMT = 310,
    IF_THEN = 311,
    IF_THEN_ELSE = 312,
    FUNC_CALL = 313,
    ARGS = 314,
    FUNCTION = 315,
    PARAM = 316,
    ARG = 317,
    CALL = 318,
    LABEL = 319,
    GOTO = 320,
    JLT = 321,
    JLE = 322,
    JGT = 323,
    JGE = 324,
    EQ = 325,
    NEQ = 326,
    STRUCT_ACCESS = 327,
    UMINUS = 328,
    LOWER_THEN_ELSE = 329,
    ARRAY_FIRST = 330
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 16 "parser.y" /* yacc.c:1909  */

    int    type_int;
    float  type_float;
    char   type_char;
    char   type_id[32];
    char   type_string[32];
    char   struct_name[32];
    struct ASTNode *ptr;

#line 140 "parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
