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
{INT}       {printf("(%s , INT)\n",yytext);yylval.type_int=atoi(yytext); return INT;}
{FLOAT}     {printf("(%s, CONSTANTFLOAT)\n", yytext);yylval.type_float=atof(yytext); return FLOAT;}
{CHAR}      {printf("(%s, CONSTANTCHAR)\n", yytext);strcpy(yylval.type_char,yytext);return CHAR;}
{STRING}    {printf("(%s, CONSTANTSTRING)\n", yytext);strcpy(yylval.type_string,yytext);return STRING;}
"int"       {printf("(%s, TYPEINT)\n", yytext);strcpy(yylval.type_id, yytext);return TYPE;}
"float"     {printf("(%s, TYPEFLOAT)\n", yytext);strcpy(yylval.type_id, yytext);return TYPE;}
"char"      {printf("(%s, TYPECHAR)\n", yytext);strcpy(yylval.type_id, yytext);return TYPE;}
"string"    {printf("(%s, TYPESTRING)\n", yytext);strcpy(yylval.type_id, yytext);return TYPE;}
"void"      {printf("(%s, TYPEVOID)\n", yytext);strcpy(yylval.type_id, yytext);return TYPE;}
"struct"    {printf("(%s, TYPESTRUCT)\n", yytext);return STRUCT;}
"return"    {printf("(%s, RETURN)\n", yytext);return RETURN;}
"if"        {printf("(%s, IF)\n", yytext);return IF;}
"else"      {printf("(%s, ELSE)\n", yytext);return ELSE;}
"while"     {printf("(%s, WHILE)\n", yytext);return WHILE;}
"for"       {printf("(%s, FOR)\n", yytext); return FOR;}
"continue"  {printf("(%s, CONTINUE)\n", yytext);return CONTINUE;}
"break"     {printf("(%s, BREAK)\n", yytext); return BREAK;}
"switch"    {printf("(%s, SWITCH)\n", yytext);return SWITCH;}
"case"      {printf("(%s, CASE)\n", yytext);return CASE;}
"default"   {printf("(%s, DEFAULT)\n", yytext);return DEFAULT;}


{ID}        {printf("(%s, INDENTIFIER)\n", yytext);strcpy(yylval.type_id,  yytext); return ID;};/*由于关键字的形式也符合表示符的规则，所以把关键 字的处理全部放在标识符的前面，优先识别*/
";"		    {printf("(%s, SEMI)\n", yytext);return SEMI;}
":"         {printf("(%s, COLON)\n", yytext);return COLON;}
","		    {printf("(%s, COMMA)\n", yytext);return COMMA;}
"."         {printf("(%s, DOT)\n", yytext);return DOT;}
">"|"<"|">="|"<="|"=="|"!=" {printf("(%s, RELOP)\n", yytext);strcpy(yylval.type_id, yytext);return RELOP;}
"="		    {printf("(%s, ASSIGNOP)\n", yytext);return ASSIGNOP;}
"+"		    {printf("(%s, PLUSOP)\n", yytext);return PLUS;}
"-"		    {printf("(%s, MINUSOP)\n", yytext);return MINUS;}
"*"		    {printf("(%s, STAROP)\n", yytext);return STAR;}
"/"		    {printf("(%s, DIVOP)\n", yytext);return DIV;}
"%"         {printf("(%s, MODOP)\n", yytext);return MOD;}
"&&"        {printf("(%s, ANDOP)\n", yytext);return AND;}
"||"        {printf("(%s, OROP)\n", yytext);return OR;}
"!"		    {printf("(%s, NOTOP)\n", yytext);return NOT;}
"++"        {printf("(%s, AUTOPLUSOP)\n", yytext); return AUTOPLUS;}
"--"        {printf("(%s, AUTOMINUS)\n", yytext); return AUTOMINUS;}
"+="        {printf("(%s, PLUSASSIGNOP)\n", yytext); return PLUSASSIGNOP;}
"-="        {printf("(%s, MINUSASSIGNOP)\n", yytext); return MINUSASSIGNOP;}
"*="        {printf("(%s, STARASSIGNOP)\n", yytext);return STARASSIGNOP;}
"/="        {printf("(%s, DIVASSIGNOP)\n)", yytext);return DIVASSIGNOP;}
"%="        {printf("(%s, MODASSIGNOP)\n", yytext);return MODASSIGNOP;}
"("		    {printf("(%s, LP)\n", yytext);return LP;}
")"		    {printf("(%s, RP)\n", yytext);return RP;}
"{"		    {printf("(%s, LC)\n", yytext);return LC;}
"}"		    {printf("(%s, RC)\n", yytext);return RC;}
"["         {printf("(%s, LB)\n", yytext);return LB;}
"]"         {printf("(%s, LC)\n", yytext);return RB;}
[\n]        {printf("(\\n, EOL)\n");yycolumn=1;}   
[ \r\t]     {}   
.		    {printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}
\/\/[^\n]*  {printf("(%s, LINECOMMENT)\n", yytext);}        // 匹配行注释的正则表达式
[\/][\*][^\*]*[\*]+([^\*\/][^\*]*[\*]+)*[\/] {printf("(%s, BLOCKCOMMENT)\n", yytext);} // 匹配块注释的正则表达式
