#include "def.h"
#define OFFSET 3
#define MAX_LOOP 100

symbolTable myTable = {{0}, 0};
symbol_scope_TX myScope = {{0}, 0};

int i, j, t, counter = 0;
int rtn, flag1, flag2, num;
int mem, stru_dec = 0, exp_ele = 0;
int rtn2;
char struct_name[33];
int switch_flag = 0, loop_flag = 0;
int left_required = 0;
int array_size = 0;

struct ASTNode * mknode(int num,int kind,int pos,...){
    struct ASTNode *T=(struct ASTNode *)calloc(sizeof(struct ASTNode),1);
    int i=0;
    T->kind=kind;
    T->pos=pos;
    va_list pArgs;
    va_start(pArgs, pos);
    for(i=0; i<num; i++)
        T->ptr[i]= va_arg(pArgs, struct ASTNode *);
    while (i<4) 
        T->ptr[i++]=NULL;
    va_end(pArgs);
    return T;
}

int searchSymbolTable(char *name) {
    int flag=0;
    for(int i = myTable.index-1; i >= 0; --i){
        if (myTable.symbols[i].level==0)
            flag=1;
        if (flag && myTable.symbols[i].level==1)
            continue;   //跳过前面函数的形式参数表项
        if (!strcmp(myTable.symbols[i].name, name)) {
            if(myTable.symbols[i].flag == 'M'){
                if(exp_ele)
                    return i;
            } else
                return i;
        }
    }
    return -1;
}

void semantic_error(int line, char *msg1, char *msg2){
    //这里可以只收集错误信息，最后一次显示
    printf("ERROR! 第%d行, %s %s\n",line, msg1, msg2);
}

void DisplaySymbolTable() {
    printf("\t\t\n***Symbol Table***\n");
    printf("----------------------------------------------------------------------\n");
    printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n","Index","Name","Level","Type","Flag","Param_num","Array_size");
    printf("----------------------------------------------------------------------\n");
    for(int i = 0;i < myTable.index; i++){
        printf("%d\t",i);
        printf("%s\t",myTable.symbols[i].name);
        printf("%d\t",myTable.symbols[i].level);
        if(myTable.symbols[i].type==INT)
               printf("%s\t","int");
        else if(myTable.symbols[i].type==FLOAT)
            printf("%s\t","float");
        else if(myTable.symbols[i].type==CHAR)
            printf("%s\t","char");
        else if(myTable.symbols[i].type==STRING)
            printf("%s\t","string");
        else if(myTable.symbols[i].type==STRUCT)
            printf("%s\t", "struct");
        else
            printf("%s\t","void");
            
        printf("%c\t",myTable.symbols[i].flag);
        if(myTable.symbols[i].flag=='F')
            printf("%d",myTable.symbols[i].paramnum);
        printf("\t\t");
        if(myTable.symbols[i].flag == 'A')
            printf("%d\n", myTable.symbols[i].array_size);
        else
            printf("\n");
    }
    printf("----------------------------------------------------------------------\n");
    printf("\n");
}

int fillSymbolTable(char *name, char *alias, int level, int type, char flag, int offset){
    // 首先根据name查符号表，不能重复定义 重复定义返回-1
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for (i = myTable.index - 1; myTable.symbols[i].level == level || (level == 0 && i >= 0); i--)
    {
        if (level == 0 && myTable.symbols[i].level == 1)
            continue; //外部变量和形参不必比较重名
        if (!strcmp(myTable.symbols[i].name, name))
            return -1;
    }
    //填写符号表内容
    strcpy(myTable.symbols[myTable.index].name, name);
    strcpy(myTable.symbols[myTable.index].alias, alias);
    myTable.symbols[myTable.index].level = level;
    myTable.symbols[myTable.index].type = type;
    myTable.symbols[myTable.index].flag = flag;
    myTable.symbols[myTable.index].offset = offset;
    return myTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}

// 匹配函数参数
int match_param(int i, struct ASTNode *T) { 
    int num = myTable.symbols[i].paramnum;
    int type1, type2;
    if (num == 0 && T == NULL)
        return 1;
    for (int j = 1; j < num; j++) {
        if (!T) {
            semantic_error(T->pos, "", "函数调用参数太少");
            return 0;
        }
        type1 = myTable.symbols[i + j].type; //形参类型
        type2 = T->ptr[0]->type;
        if (type1 != type2) {
            semantic_error(T->pos, "", "参数类型不匹配");
            return 0;
        }
        T = T->ptr[1];
    }
    if (T->ptr[1]) { //num个参数已经匹配完，还有实参表达式
        semantic_error(T->pos, "", "函数调用参数太多");
        return 0;
    }
    return 1;
}

int semantic_Analysis(struct ASTNode *T, int type, int level, char flag, int command) {
    int type1, type2;
    if(T) {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            break;
        case EXT_VAR_DEF:
            type = semantic_Analysis(T->ptr[0], type, level, flag, command);
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            break;
        case EXT_STRUCT_DEF:
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            break;
        case STRUCT_DEF:
            rtn = searchSymbolTable(T->ptr[0]->type_id);
            if(rtn != -1){
                if(myTable.symbols[rtn].flag == 'S'){
                    semantic_error(T->pos, "", "结构体名重复定义");
                }
            }
            strcpy(myTable.symbols[myTable.index].name, T->ptr[0]->type_id);
            myTable.symbols[myTable.index].level = level;
            myTable.symbols[myTable.index].flag = 'S';
            myTable.symbols[myTable.index].type = STRUCT;
            myTable.index++;
            flag = 'M';
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            break;
        case STRUCT_DEC:
            rtn = searchSymbolTable(T->ptr[0]->type_id);
            if(rtn == -1){
                semantic_error(T->pos, T->type_id, "结构体未定义");
            }
            else{
                stru_dec = 1;
                strcpy(struct_name, T->ptr[0]->type_id);
            }
            break;
        case EXP_ELE:
            rtn = searchSymbolTable(T->ptr[0]->type_id);
            flag1 = 0;

            if(rtn == -1){
                semantic_error(T->pos, T->ptr[0]->type_id, "结构体变量未定义");
            }
            else{
                if(myTable.symbols[rtn].type != STRUCT){
                    semantic_error(T->pos, T->ptr[0]->type_id, "不是结构体");
                }
                else{
                    rtn = searchSymbolTable(myTable.symbols[rtn].struct_name);
                    if(rtn == -1){
                        semantic_error(T->pos, "", "结构体未定义");
                        return 0;
                    }
                    num = rtn;
                    exp_ele = 1;
                    do{
                        num++;
                        if(!strcmp(myTable.symbols[num].name, T->type_id)){
                            flag1 = 1;
                            break;
                        }
                    } while(num < myTable.index && myTable.symbols[num].flag == 'M');
                    if(!flag1){
                        semantic_error(T->pos, "结构体不含成员变量", T->type_id);
                    }
                    exp_ele = 0;
                    flag1 = 0;
                }
            }
            break;
        case ARRAY_DEC:
            rtn = searchSymbolTable(T->type_id);
            if(rtn != -1){
                if(myTable.symbols[rtn].level == level){
                    semantic_error(T->pos, "", "数组名重复定义");
                }
            }
            else{
                strcpy(myTable.symbols[myTable.index].name, T->type_id);
                myTable.symbols[myTable.index].level = level;
                myTable.symbols[myTable.index].flag = 'A';
                myTable.symbols[myTable.index].type = type;
                myTable.symbols[myTable.index].array_size = 0;
                myTable.index++;
                semantic_Analysis(T->ptr[0], type, level, 'A', 0);
            }
            break;
        case ARRAY_LIST:
            type1 = semantic_Analysis(T->ptr[0], type, level, flag, command);
            if(type1 != INT){
                semantic_error(T->pos, "", "数组下标不是整型表达式");
            }
            else{
                if(command == 0){
                    myTable.symbols[myTable.index-1].array_size++;
                }
                else{
                    array_size++;
                }
                semantic_Analysis(T->ptr[1], type, level, flag, command);
            }
            break;
        case EXP_ARRAY:
            rtn = searchSymbolTable(T->type_id);
            if(rtn == -1){
                semantic_error(T->pos, T->type_id, "数组未定义");
            }
            else{
                if(myTable.symbols[rtn].flag != 'A'){
                    semantic_error(T->pos, T->type_id, "不是数组");
                }
                else{
                    array_size = 0;
                    semantic_Analysis(T->ptr[0], type, level, flag, 1);
                    // printf("\n\n%d %d\n\n", array_size, myTable.symbols[rtn].array_size);
                    if(array_size != myTable.symbols[rtn].array_size && array_size != 0)
                        semantic_error(T->pos, T->type_id, "数组维数不一致");
                    array_size = 0;
                }
            }
            break;
        case TYPE:
            return T->type;
        case EXT_DEC_LIST:
            flag = 'V';
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            break;
        case ID:
            // printf("\n\n%s\n\n", T->type_id);
            i = (flag == 'P' ? 2 : 0);
            while(myTable.symbols[i].level != level && i < myTable.index)
                i++;
            if(command == 0){ //定义变量
                while(i < myTable.index){
                    if(!strcmp(myTable.symbols[i].name,T->type_id) && (myTable.symbols[i].flag==flag)){
                        // printf("\n\n%s %s\n\n", myTable.symbols[i].name, T->type_id);
                        if(flag=='V')
                            semantic_error(T->pos, T->type_id, "全局变量重复定义");
                        else if(flag=='F')
                            semantic_error(T->pos, T->type_id, "函数名重复定义");
                        else if(flag=='T')
                            semantic_error(T->pos, T->type_id, "局部变量重复定义");
                        else if(flag=='M')
                            semantic_error(T->pos, "", "成员变量重复定义");
                        else
                            semantic_error(T->pos, "", "函数参数定义重复");
                        return 0;
                    }
                    i++;
                }
                strcpy(myTable.symbols[myTable.index].name, T->type_id);
                myTable.symbols[myTable.index].level = level;
                myTable.symbols[myTable.index].flag = flag;
                myTable.symbols[myTable.index].type = type;
                if (stru_dec) {
                    strcpy(myTable.symbols[myTable.index].struct_name, struct_name);
                }
                myTable.index++;
                stru_dec = 0;
                return type;
            }
            else{ // 使用变量
                i = myTable.index - 1;
                while(i >= 0){
                    if(myTable.symbols[i].level <= level && !strcmp(myTable.symbols[i].name, T->type_id) && (myTable.symbols[i].flag == 'T' || myTable.symbols[i].flag == 'V' || myTable.symbols[i].flag == 'P'))
                    {
                        return myTable.symbols[i].type;
                    }    
                    i--;
                }
                if(i < 0){
                    semantic_error(T->pos, T->type_id, "变量未定义");
                }
            }
            break;
        case FUNC_DEF:
            type = semantic_Analysis(T->ptr[0], type, level+1, flag, command);
            if (type != VOID)
                flag1 = 1;
            semantic_Analysis(T->ptr[1], type, 1, flag, command);
            semantic_Analysis(T->ptr[2], type, 1, flag, command);
            num = myTable.index;
            do{
                num--;
            } while (myTable.symbols[num].flag == 'P');
            myTable.index = num+1;
            if(flag1 && !flag2){
                semantic_error(T->pos, T->type_id, "函数没有返回值");
            }   
            flag1 = 0;
            flag2 = 0;
            break;
        case FUNC_DEC:
            rtn = searchSymbolTable(T->type_id);
            if(rtn != -1){
                if(myTable.symbols[rtn].flag == 'F'){
                    semantic_error(T->pos, T->type_id, "函数名重复定义");
                }
            }
            strcpy(myTable.symbols[myTable.index].name, T->type_id);
            myTable.symbols[myTable.index].flag = 'F';
            myTable.symbols[myTable.index].level = 0;
            myTable.symbols[myTable.index].type = type;
            myTable.index++;
            counter = 0;
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            myTable.symbols[myTable.index - counter - 1].paramnum = counter;
            break;
        case PARAM_LIST:
            counter++;
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            break;
        case PARAM_DEC:
            flag = 'P';
            type = semantic_Analysis(T->ptr[0], type, level+1, flag, command);
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            break;
        case COMP_STM:
            flag = 'T';
            command = 0;
            myScope.TX[myScope.top++] = myTable.index;
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            command = 1;
            semantic_Analysis(T->ptr[1], type, level+1, flag, command);
            DisplaySymbolTable();
            myTable.index = myScope.TX[--myScope.top];
            break;
        case STM_LIST:
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            break;
        case DEF_LIST:
            semantic_Analysis(T->ptr[0],type,level,flag,command);
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            break;
        case VAR_DEF:
            type = semantic_Analysis(T->ptr[0],type,level+1,flag,command);
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            break;
        case DEC_LIST:
            if(stru_dec){
                type = STRUCT;
            }
            semantic_Analysis(T->ptr[0],type,level,flag,command);
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            break;
        case EXP_STMT:
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            break;
        case RETURN:
            flag2 = 1;
            if(flag1){
                if(!T->ptr[0]){
                    semantic_error(T->pos, "", "缺少返回值");
                    break;
                }
            }
            type = semantic_Analysis(T->ptr[0], type, level, flag, command);
            num = myTable.index;
            do{
                num--;
            }while (myTable.symbols[num].flag != 'F');
            if( !(!flag1 && !T->ptr[0]) && type != myTable.symbols[num].type){
                semantic_error(T->pos, "", "返回值类型错误");
            }
            break;
        case IF_THEN:
            semantic_Analysis(T->ptr[0],type,level,flag,command);
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            break;
        case WHILE:
        case FOR:
            loop_flag++;
            semantic_Analysis(T->ptr[0],type,level,flag,command);
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            loop_flag--;
            break;
        case SWITCH_STMT:
            switch_flag++;
            type = semantic_Analysis(T->ptr[0],type,level,flag,command);
            if(type != INT && type != CHAR){
                semantic_error(T->pos, "", "switch语句只接受int, char类型");
            }
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            switch_flag--;
            break;
        case CASE_STMT:
            semantic_Analysis(T->ptr[0],type,level,flag,command);
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            break;
        case BREAK:
            if(!switch_flag && !loop_flag)
                semantic_error(T->pos, "", "break语句要在循环语句或switch语句中");
            break;
        case CONTINUE:
            if(!loop_flag)
                semantic_error(T->pos, "", "continue语句要在循环语句中");
            break;
        case IF_THEN_ELSE:
            semantic_Analysis(T->ptr[0],type,level,flag,command);
            semantic_Analysis(T->ptr[1],type,level,flag,command);
            semantic_Analysis(T->ptr[2],type,level,flag,command);
            break;
        case DEFAULT_STMT:
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            break;
        case FOR_DEC:
            semantic_Analysis(T->ptr[0], type, level, flag, command);
            semantic_Analysis(T->ptr[1], type, level, flag, command);
            semantic_Analysis(T->ptr[2], type, level, flag, command);
            break;
        case ASSIGNOP:
        case PLUSASSIGNOP:
        case MINUSASSIGNOP:
        case STARASSIGNOP:
        case DIVASSIGNOP:
        case MODASSIGNOP:
            if(T->ptr[0]->kind != ID && T->ptr[0]->kind != EXP_ARRAY && T->ptr[0]->kind != EXP_ELE) {
                semantic_error(T->pos, "", "赋值表达式需要左值");
            }
            if(T->ptr[0]->kind == ID){
                rtn = searchSymbolTable(T->ptr[0]->type_id);
                if(myTable.symbols[rtn].type == STRUCT){
                    semantic_error(T->pos, "", "赋值表达式需要左值");
                }
                else{
                    type1 = semantic_Analysis(T->ptr[0], type, level, flag, command);
                    type2 = semantic_Analysis(T->ptr[1], type, level, flag, command);
                    if(type1 == type2)
                        return type1;
                }
            }
            else{
                type1 = semantic_Analysis(T->ptr[0], type, level, flag, command);
                type2 = semantic_Analysis(T->ptr[1], type, level, flag, command);
                if(type1 == type2)
                    return type1;
            }
            break;
        case AND:
        case OR:
        case RELOP:
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:
        case MOD:
            type1 = semantic_Analysis(T->ptr[0], type, level, flag, command);
            type2 = semantic_Analysis(T->ptr[1], type, level, flag, command);
            if(type1 == type2)
                return type1;
            break;
        case AUTOPLUS_L:
        case AUTOPLUS_R:
        case AUTOMINUS_L:
        case AUTOMINUS_R:
        case UMINUS:
        case NOT:
            if(T->ptr[0]->kind == ID){
                rtn = searchSymbolTable(T->ptr[0]->type_id);
                if(rtn == -1){
                    semantic_error(T->pos, T->ptr[0]->type_id, "变量未定义");
                }
                else{
                    if(myTable.symbols[rtn].type == STRUCT)
                        semantic_error(T->pos, T->ptr[0]->type_id, "不是左值");
                    else{
                        if(T->ptr[0]->kind != ID && T->ptr[0]->kind != EXP_ARRAY && T->ptr[0]->kind != EXP_ELE){
                            semantic_error(T->pos, "", "自增自减表达式需要左值");
                            break;
                        }
                        if(T->ptr[0]->kind == ID){
                            rtn = searchSymbolTable(T->ptr[0]->type_id);
                            if(myTable.symbols[rtn].type == STRUCT){
                                semantic_error(T->pos, "", "赋值表达式需要左值");
                            }
                        }
                        type1 = semantic_Analysis(T->ptr[0], type, level, flag, command);
                        return type1;
                    }
                }
            }
            else if(T->ptr[0]->kind == EXP_ELE || T->ptr[0]->kind == EXP_ARRAY || !strcmp(T->ptr[0]->type_id, "AUTO")){
                type1 = semantic_Analysis(T->ptr[0], type, level, flag, command);
                return type1;
            }
            else{
                semantic_error(T->pos, "", "自增自减表达式需要左值");
            }
            break;
        case INT:
            return INT;
        case CHAR:
            return CHAR;
        case FLOAT:
            return FLOAT;
        case STRING:
            return STRING;
        case VOID:
            return VOID;
        case FUNC_CALL:
            rtn = searchSymbolTable(T->type_id);
            if(rtn != -1){
                if(myTable.symbols[rtn].flag != 'F'){
                    semantic_error(T->pos, T->type_id, "不是函数");
                    return 0;
                }
                type = myTable.symbols[rtn].type;
                counter = 0;
                semantic_Analysis(T->ptr[0], type, level, flag, command);
                if(myTable.symbols[rtn].paramnum != counter)
                    semantic_error(T->pos, "", "参数数量不匹配");
                return myTable.symbols[rtn].type;
            }
            else{
                semantic_error(T->pos, T->type_id, "函数未定义");
                return 0;
            }
            break;
        case ARGS:
            counter++;
            t = semantic_Analysis(T->ptr[0], type, level, flag, command);
            if(type != t){
                semantic_error(T->pos, "", "函数调用参数类型不匹配");
            }
            type = myTable.symbols[j+counter+1].type;
            semantic_Analysis(T->ptr[1], type, level, flag, command);       
            break;


        default:
            break;
        }
    }
}

void semantic_Analysis0(struct ASTNode *T){
    myTable.index = 0;
    fillSymbolTable("read", "", 0, INT, 'F', 4);
    myTable.symbols[0].paramnum = 0; //read的形参个数
    fillSymbolTable("x", "", 1, INT, 'P', 12);
    fillSymbolTable("write", "", 0, INT, 'F', 4);
    myTable.symbols[2].paramnum = 1;
    myScope.TX[0] = 0; //外部变量在符号表中的起始序号为0
    myScope.top = 0;
    T->offset = 0; // 外部变量在数据区的偏移量
    semantic_Analysis(T, 0, 0, ' ', 0);
    DisplaySymbolTable();
}