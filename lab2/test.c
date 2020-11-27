// 17 


struct A{
    int a;
    float b;
    char c;
};

// void test0(int a) {
//     int b;
//     b = 0;
//     switch (b)
//     {
//     case 0:
//         b = 1;
//         break;
//     default:
//         break;
//     }
// }

// void test1() {
//     // 变量重复定义
//     int a;
//     int a;

//     // 对非函数名采用函数调用形式
//     a();

//     // 使用未定义的变量
//     c = 0;

//     // 调用未定义或未声明的函数
//     test();
// }

// void test2() {
//     int a, b;
//     a = 1;
//     // 函数调用时参数个数不匹配
//     test0();
//     test0(a, b);

//     // 函数调用时实参和形参类型不匹配
//     char c;
//     test0(c);
// }

// int test3() {
//     float x[10][20],y=12.3;

//     // a不是数组
//     // f5不是数组
//     x=a[1]+f5[1];

//     // 数组下标不是整型表达式
//     x[1]=x[1][y]+x[1+1][1];
//     // 函数返回值类型与函数定义的返回值类型不匹配； 
//     return y;
// }

int test4(float x) {
    struct A m;
    int a;
    m.b=17;

    // x不是结构体
    x.a=m.b;

    // m.a不是结构体
    m.d=m.a.a;

    // 缺少return
}

// int test5() {
//     int a,b=0;
//     if (a-12.3) continue;   // continue不在循环语句中
//     else   break;           // break不在循环语句中
//     while ( a || f3()) 
//     {
//         while (1) 
//             break;
//         continue;
//     }
//     for(a=1;a>0 && f3()>0;a++)
//  	    if (a+b == 0.0)
//             break;
// }

// void test6(int a,int b) {
//     char a,b;

//     // 赋值表达式不是左值
//     a+b=10;

//     // 自增表达式不是左值
//     (a+b)++;
// }


