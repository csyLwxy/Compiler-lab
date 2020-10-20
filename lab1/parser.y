/* 命令
  flex exp.lex
  bison -d exp.y
  gcc -o a lex.yy.c exp.tab.c
  a        
  输入含整常数和标识符的表达式后回车
  输入CTRL+Z结束
*/

%error-verbose
%{
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
extern FILE *yyin;
extern char *yytext;
struct ASTNode {
	int kind;     //0表示运算符结点，1表示常数， 2表示标识符
	union {
		struct {
			char op;
			struct ASTNode *first,*second;
		} OPNode;
		int int_const;
		char id[32];
	};
};
void display(struct ASTNode *T,int ident);
%}

%union {
	int    type_int;
	char   type_id[32];
	struct ASTNode *ptr;
};

/* 可以把标志（token）绑定到YYSTYPE的某个域 */
%token <type_int> INTEGER
%token <type_id>  ID
%type  <ptr> line exp

/* 
    %left表示左相关（left-associative）
    %right表示右相关（right-associative）。
    可以定义多组%left或%right，在后面定义的组有更高的优先级 
*/
%left '+'
%left '*'
%%

input:
     |input line
line : '\n'    {}
     | exp'\n' {display($1,0);}

exp	 : INTEGER {printf("常数%d归约成exp，生成叶子结点\n",$1);$$=(struct ASTNode *)malloc(sizeof(struct ASTNode ));$$->kind=1;$$->int_const=$1;}
     | ID {printf("标识符%s归约成exp，生成叶子结点\n",$1);$$=(struct ASTNode *)malloc(sizeof(struct ASTNode ));$$->kind=2;strcpy($$->id, $1);}
     | exp '+' exp {printf("exp + exp归约成exp，生成‘+’的非叶子结点\n");$$=(struct ASTNode *)malloc(sizeof(struct ASTNode ));
           $$->kind=0;$$->OPNode.first=$1;$$->kind=0;$$->OPNode.second=$3;$$->OPNode.op='+';}
     | exp '*' exp {printf("exp *exp归约成exp，生成‘*’的非叶子结点\n");
            $$=(struct ASTNode *)malloc(sizeof(struct ASTNode ));
            $$->kind=0;$$->OPNode.first=$1;$$->kind=0;$$->OPNode.second=$3;$$->OPNode.op='*';}
     | '(' exp')'  {printf("(exp)归约成exp，不生成新结点\n");$$=$2;}  
%%

int main(int agrc, char **argv){
    yyin = fopen(agrv[1], "r");
    if (!yyin) 
        return 1;
	yyparse();
	return 0;
}

yyerror(char *s){
   printf(" %s\n",s);
}

void display(struct ASTNode *T,int ident){
	if (T){
		switch (T->kind) {
			case 0:
                printf("%*c%c\n",ident,' ',T->OPNode.op);
                display(T->OPNode.first,ident+4);
                display(T->OPNode.second,ident+4);
                break;
			case 1:
                printf("%*c%d\n",ident,' ',T->int_const);
                break;
			case 2:
                printf("%*c%s\n",ident,' ',T->id);
		}
	}
}