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
    CHAR = 258,
    INT = 259,
    ID = 260,
    RELOP = 261,
    TYPE = 262,
    FLOAT = 263,
    STRING = 264,
    STRUCT = 265,
    LP = 266,
    RP = 267,
    LC = 268,
    RC = 269,
    LB = 270,
    RB = 271,
    SEMI = 272,
    COMMA = 273,
    DOT = 274,
    PLUS = 275,
    MINUS = 276,
    STAR = 277,
    DIV = 278,
    MOD = 279,
    ASSIGNOP = 280,
    PLUSASSIGNOP = 281,
    MINUSASSIGNOP = 282,
    STARASSIGNOP = 283,
    DIVASSIGNOP = 284,
    MODASSIGNOP = 285,
    AND = 286,
    OR = 287,
    NOT = 288,
    AUTOPLUS = 289,
    AUTOMINUS = 290,
    IF = 291,
    ELSE = 292,
    WHILE = 293,
    RETURN = 294,
    FOR = 295,
    COLON = 296,
    DEFAULT = 297,
    CONTINUE = 298,
    BREAK = 299,
    SWITCH = 300,
    CASE = 301,
    EXT_DEF_LIST = 302,
    EXT_VAR_DEF = 303,
    FUNC_DEF = 304,
    FUNC_DEC = 305,
    EXT_DEC_LIST = 306,
    PARAM_LIST = 307,
    PARAM_DEC = 308,
    VAR_DEF = 309,
    DEC_LIST = 310,
    DEF_LIST = 311,
    COMP_STM = 312,
    STM_LIST = 313,
    EXP_STMT = 314,
    IF_THEN = 315,
    IF_THEN_ELSE = 316,
    ARRAY_LIST = 317,
    ARRAY_ID = 318,
    FUNC_CALL = 319,
    ARGS = 320,
    FUNCTION = 321,
    PARAM = 322,
    ARG = 323,
    CALL = 324,
    LABEL = 325,
    GOTO = 326,
    JLT = 327,
    JLE = 328,
    JGT = 329,
    JGE = 330,
    EQ = 331,
    NEQ = 332,
    FOR_DEC = 333,
    STRUCT_DEF = 334,
    STRUCT_TAG = 335,
    EXP_ELE = 336,
    SWITCH_STMT = 337,
    CASE_STMT = 338,
    DEFAULT_STMT = 339,
    UMINUS = 340,
    LOWER_THEN_ELSE = 341
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 16 "parser.y" /* yacc.c:1909  */

    int    type_int;
    float  type_float;
    char   type_char[5];
    char   type_id[32];
    char   type_string[32];
    char   struct_name[32];
    struct ASTNode *ptr;

#line 151 "parser.tab.h" /* yacc.c:1909  */
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
