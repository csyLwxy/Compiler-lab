%option noyywrap yylineno
%{
#include "parser.tab.h"
#include "string.h"
#include "def.h"
int yycolumn=1;
#define YY_USER_ACTION  yylloc.first_line = yylloc.last_line=yylineno;\
                        yylloc.first_column = yycolumn;\
                        yylloc.last_column = yycolumn+yyleng-1;\
                        yycolumn += yyleng;

typedef union {
	int type_int;
	int type_float;
    char type_char;
	char type_id[32];
	struct node *ptr;
} YYLVAL;
#define YYSTYPE YYLVAL
%}

ID   	[A-Za-z_]([A-Za-z0-9_])*
INT  	([1-9][0-9]*)|0
FLOAT  	[0-9]*[.][0-9]+([eE][-+]?([1-9][0-9*]|[0]))?
CHAR 	\'(\\.|[^'])\'
STRING 	\"(\\.|[^"\\])*\"

%%
{INT}        {printf("(%s , INT)\n",yytext);yylval.type_int=atoi(yytext); return INT;}
{FLOAT}      {printf("(%s , FLOAT)\n",yytext);yylval.type_float=atof(yytext); return FLOAT;}
{CHAR}       {printf("(%s  ,CHAR)\n",yytext);yylval.type_char=yytext[1];return CHAR;}
{STRING}     {printf("(%s, CONSTANTSTRING)\n", yytext);strcpy(yylval.type_string,yytext);return STRING;}
"int"        {printf("(int , TYPE)\n");strcpy(yylval.type_id,  yytext);return TYPE;}
"float"      {printf("(float , TYPE)\n");strcpy(yylval.type_id,  yytext);return TYPE;}
"char"       {printf("(char , TYPE)\n");strcpy(yylval.type_id,  yytext);return TYPE;}
"string"     {printf("(string , TYPE)\n");strcpy(yylval.type_id,  yytext);return TYPE;}
"struct"     {printf("(struct , STRUCT)\n");return STRUCT;}

"return"     {printf("(return , RETURN)\n");return RETURN;}
"if"         {printf("(if , IF)\n");return IF;}
"else"       {printf("(else , ELSE)\n");return ELSE;}
"while"      {printf("(while , WHILE)\n");return WHILE;}
"break"      {printf("(break , BREAK)\n");return BREAK;}
"continue"   {printf("(continue , CONTINUE)\n");return CONTINUE;}
"for"        {printf("(for , FOR)\n");return FOR;}

{ID}         {printf("(%s , ID)\n",yytext);strcpy(yylval.type_id,  yytext); return ID;}// 由于关键字的形式也符合标识符的规则，所以把关键字的处理全部放在标识符的前面，优先识别
";"			 {printf("(; , SEMI)\n");return SEMI;}
","			 {printf("(, , COMMA)\n");return COMMA;}
">"|"<"|">="|"<="|"=="|"!=" {printf("(%s , RELOP)\n",yytext);strcpy(yylval.type_id, yytext);return RELOP;}
"="			{printf("(= , ASSIGNOP)\n");return ASSIGNOP;}
"+"			{printf("(+ , PLUS)\n");return PLUS;}
"-"			{printf("(- , MINUS)\n");return MINUS;}
"*"			{printf("(* , STAR)\n");return STAR;}
"/"			{printf("(/ , DIV)\n");return DIV;}
"++"        {printf("(++ , SELFADD)\n");return SELFADD;}
"--"        {printf("(-- , SELFDEC)\n");return SELFDEC;}
"+="        {printf("(+= , ADD_ASSIGNOP)\n");return ADD_ASSIGNOP;}
"-="        {printf("(-= , MINUS_ASSIGNOP)\n");return MINUS_ASSIGNOP;}
"*="        {printf("(*= , STAR_ASSIGNOP)\n");return STAR_ASSIGNOP;}
"/="        {printf("(/= , DIV_ASSIGNOP)\n");return DIV_ASSIGNOP;}
"&&"        {printf("(&& , AND)\n");return AND;}
"||"        {printf("(|| , OR)\n");return OR;}
"!"			{printf("(! , NOT)\n");return NOT;}
"("			{printf("(( , LP)\n");return LP;}
")"			{printf("() , RP)\n");return RP;}
"{"			{printf("({ , LC)\n");return LC;}
"}"			{printf("(} , RC)\n");return RC;}
"."         {printf("(. , DOT)\n");return DOT;}
"["         {printf("([ , LB)\n");return LB;}
"]"         {printf("(] , RB)\n");return RB;}
[\n]        {yycolumn=1;}
[ \r\t]     {}
.		    {printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}
\/\/[^\n]*  {printf("(%s, LINECOMMENT)\n", yytext);}        // 匹配行注释的正则表达式
[\/][\*][^\*]*[\*]+([^\*\/][^\*]*[\*]+)*[\/] {printf("(%s, BLOCKCOMMENT)\n", yytext);} // 匹配块注释的正则表达式
