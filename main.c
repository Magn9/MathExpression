//
//  main.c
//  MathExpression
//
//  Created by Nine on 2016/11/7.
//  Copyright © 2016年 Nine. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10

typedef int Status;
typedef int ElemType;

typedef struct {
    ElemType *base;     //栈底指针
    ElemType *top;      //栈顶指针
    int stacksize;      //栈大小
}Stack;

Status GetTop(Stack s, char *e) {
    if(s.base == s.top)
        return printf("空栈");
    
    *e = *(s.top - sizeof(ElemType));
    
    return 1;
}

Status Push(Stack s, char e) {
    if(s.top - s.base >= s.stacksize * sizeof(ElemType)) { //若栈满，追加空间
        s.base = (ElemType *) realloc (s.base, (s.stacksize + STACK_INIT_SIZE) * sizeof(ElemType));
        s.top = s.base + s.stacksize; //realloc有可能改变base
        s.stacksize += STACK_INCREMENT;
    }
    
    *s.top = e;
    s.top += sizeof(ElemType);
    
    return 1;
}

Status Pop(Stack s, char *e) {
    if(s.base == s.top)
        return printf("空栈");
    
    s.top -= sizeof(ElemType);
    *e = *s.top;
    
    return 1;
}

Stack OPTR, OPND; //OPTR寄存运算符，OPND寄存操作数　operator/operand

Status InitStack() { //初始化两栈，并PUSH #进运算符栈
    OPTR.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPTR.top = OPTR.base;
    OPTR.stacksize = STACK_INIT_SIZE;
    Push(OPTR, '#');
    
    OPND.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPND.top = OPND.base;
    OPND.stacksize = STACK_INIT_SIZE;
    
    return 1;
}

char Precede(char top, char put) {
    int a, b; //+ - 为0，* /为1，()为2
    
    if(top == '+' || top == '-')
        a = 0;
    else if(top == '*' || top == '\\')
        a = 1;
    else
        a = 2;
    
    if(put == '+' || put == '-')
        b = 0;
    else if(put == '*' || put == '\\')
        b = 1;
    else
        b = 2;
    
    if(a > b)
        return '>';
    else if(a < b)
        return '<';
    else
        return '=';
}

char Calculate(char num1, char num2, char oprt) {
    return 1;
}

void EvaluateExpression() {
    printf("请输入表达式，以#表示结束\n");
    char c = getchar();
    char top; //OPTR栈顶
    char lastTop; //上一个栈顶，用于计算
    char num1, num2; // 暂存OPND的值用于计算
    
    GetTop(OPTR, &top);
    
    while(c != '#' || top != '#') {
        if('0' <= c && c <= '9') {  // 如果是操作数，则进OPND栈
            Push(OPND, c);
            c = getchar();
        } else {
            switch (Precede(top, c)) {
                case '>': //还有)的情况没写
                    Push(OPTR, c);
                    c = getchar();
                    break;
                
                case '=':
                    if(c == '(') {  //连续括号的情况，直接压入
                        Push(OPTR, c);
                    } else {        //可以计算上一个操作符的值，如3+3-
                        Pop(OPTR, &lastTop);
                        Pop(OPND, &num2);
                        Pop(OPND, &num1);
                        
                        Push(OPTR, c); //这时候压入所输入的操作符
                        Push(OPND, Calculate(num1, num2, lastTop)); //压入结果
                    }
                    break;
                    
                case '<':
                    if(top == '(') { //如(2x的情况，直接压入
                        Push(OPTR, c);
                    } else {   //如(2x1+的情况，计算上一个操作符的值
                        //待定
                        
                    }
                    
                default:
                    break;
            }
            
        }
        GetTop(OPTR, &top);
    }
    
}

Status main() {
    InitStack();
    
    return 1;
}

