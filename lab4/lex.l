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
    char type_char[5];
	char type_id[32];
	char type_string[32];
	char struct_name[32];
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
{INT}       {yylval.type_int=atoi(yytext); return INT;}
{FLOAT}     {yylval.type_float=atof(yytext); return FLOAT;}
{CHAR}      {strcpy(yylval.type_char,yytext);return CHAR;}
{STRING}    {strcpy(yylval.type_string,yytext);return STRING;}
"int"       {strcpy(yylval.type_id, yytext);return TYPE;}
"float"     {strcpy(yylval.type_id, yytext);return TYPE;}
"char"      {strcpy(yylval.type_id, yytext);return TYPE;}
"string"    {strcpy(yylval.type_id, yytext);return TYPE;}
"void"      {strcpy(yylval.type_id, yytext);return TYPE;}
"struct"    {return STRUCT;}
"return"    {return RETURN;}
"if"        {return IF;}
"else"      {return ELSE;}
"while"     {return WHILE;}
"for"       {return FOR;}
"continue"  {return CONTINUE;}
"break"     {return BREAK;}
"switch"    {return SWITCH;}
"case"      {return CASE;}
"default"   {return DEFAULT;}


{ID}        {strcpy(yylval.type_id,  yytext); return ID;};/*由于关键字的形式也符合表示符的规则，所以把关键 字的处理全部放在标识符的前面，优先识别*/
";"		    {return SEMI;}
":"         {return COLON;}
","		    {return COMMA;}
"."         {return DOT;}
">"|"<"|">="|"<="|"=="|"!=" {strcpy(yylval.type_id, yytext);return RELOP;}
"="		    {return ASSIGNOP;}
"+"		    {return PLUS;}
"-"		    {return MINUS;}
"*"		    {return STAR;}
"/"		    {return DIV;}
"%"         {return MOD;}
"&&"        {return AND;}
"||"        {return OR;}
"!"		    {return NOT;}
"++"        {return AUTOPLUS;}
"--"        {return AUTOMINUS;}
"+="        {return PLUSASSIGNOP;}
"-="        {return MINUSASSIGNOP;}
"*="        {return STARASSIGNOP;}
"/="        {return DIVASSIGNOP;}
"%="        {return MODASSIGNOP;}
"("		    {return LP;}
")"		    {return RP;}
"{"		    {return LC;}
"}"		    {return RC;}
"["         {return LB;}
"]"         {return RB;}
[\n]        {yycolumn=1;}   
[ \r\t]     {}   
.		    {printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}
[\/][\/].* {}//匹配注释的正则表达式
[\/][\*][^\*]*[\*]+([^\*\/][^\*]*[\*]+)*[\/] {} // 匹配块注释的正则表达式
