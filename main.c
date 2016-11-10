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
}Stack, *pStack;

Stack OPTR, OPND; //OPTR寄存运算符，OPND寄存操作数　operator/operand

Status GetTop(pStack s, int *e) {
    if(s->base == s->top) {
        return printf("空栈top");
    }
    
    *e = *(s->top - 1);
    
    return 1;
}

Status Push(pStack s, int e) {
    if(s->top - s->base >= s->stacksize) { //若栈满，追加空间
        s->base = (ElemType *) realloc (s->base, (s->stacksize + STACK_INIT_SIZE) * sizeof(ElemType));
        s->top = s->base + s->stacksize; //realloc有可能改变base
        s->stacksize += STACK_INCREMENT;
    }
    
    
    *s->top = e;
    if(s == &OPND)
    {
        printf("压入OPND,值为%d\n", *s->top);
    } else {
        printf("压入OPRT,值为%c\n", *s->top);
    }
        
	s->top++;
    
    return 1;
}

Status Pop(pStack s, int *e) {
    if(s->base == s->top)
        return printf("空栈");
    
	s->top--;
    *e = *s->top;
    
    if(s == &OPND)
    {
        printf("出栈OPND,值为%d\n", *s->top);
    } else {
        printf("出栈OPRT,值为%c\n", *s->top);
    }
    
    return 1;
}



Status InitStack() { //初始化两栈，并PUSH #进运算符栈
    OPTR.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPTR.top = OPTR.base;
    OPTR.stacksize = STACK_INIT_SIZE;
    Push(&OPTR, '#');
    
    OPND.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPND.top = OPND.base;
    OPND.stacksize = STACK_INIT_SIZE;
    
    return 1;
}

char Precede(char top, char put) {
    int a, b; //+ - 为0，* /为1，()为2
    
	if (top == '+' || top == '-')
		a = 0;
	else if (top == '*' || top == '\\')
		a = 1;
	else if (top == '#')
		a = -1;
	else //括号
        a = 2;
    
    if(put == '+' || put == '-')
        b = 0;
    else if(put == '*' || put == '\\')
        b = 1;
    else if (put == '#')
        b = -1;
    else
        b = 2;
    
    if(a > b)
        return '<';
    else if(a < b)
        return '>';
    else
        return '=';
}

Status Calculate(int num1, int num2, char oprt) {
    switch (oprt) {
        case '+':
            Push(&OPND, num1+num2);
            break;
        case '-':
            Push(&OPND, num1-num2);
            break;
        case '*':
            Push(&OPND, num1*num2);
            break;
        case '/':
            Push(&OPND, num1/num2);
            break;
        default:
            break;
    }
    return 1;
}

int charToInt(char c) {
    return c - '0';
}

Status EvaluateExpression() {
    printf("请输入表达式，以#表示结束\n");
    char c = getchar();
    int top; //OPTR栈顶
    int num1, num2; // 暂存OPND的值用于计算
    int numtemp, numcount; //由于putchar只能单个输入，如果数字不是1位数，就会出现问题，于是有numtemp暂存,numcount记录数位
    
    GetTop(&OPTR, &top);
    numtemp = numcount = 0;
    
	while(c != '#' || top != '#') {
        if('0' <= c && c <= '9') {  // 如果是操作数，则进OPND栈
            numtemp += charToInt(c) * pow(10, numcount); //解决非1位数字进栈
            numcount++;
            c = getchar();
        } else {
            if(numcount > 0) { //把之前的数字压进栈
                Push(&OPND, numtemp);
                numtemp = 0;
                numcount = 0;
            }
            
            switch (Precede(top, c)) {
                case '>':
                    if(c == ')') { //如果遇到)，则括号内只剩下一个操作符和两个数，直接算出结果并消去(
                        Pop(&OPTR, &top);
                        Pop(&OPND, &num2);
                        Pop(&OPND, &num1);
                        
                        Calculate(num1, num2, top); //调用，计算并压入结果
                        Pop(&OPTR, &top); //消去(
                    } else {
                        Push(&OPTR, c);
                    }
                    c = getchar();
                    break;
                
                case '=':
                    if(c == '(') {  //连续括号的情况，直接压入
                        Push(&OPTR, c);
                    } else {        //可以计算上一个操作符的值，如3+3-
                        Pop(&OPTR, &top);
                        Pop(&OPND, &num2);
                        Pop(&OPND, &num1);
                        
                        Calculate(num1, num2, top); //调用，计算并压入结果
                        Push(&OPTR, c); //这时候压入所输入的操作符
                    }
                    c = getchar();
                    break;
                    
                case '<':
                    if(top == '(') { //如(2x的情况，直接压入
                        Push(&OPTR, c);
                    } else {   //如(2x1+的情况，计算上一个操作符的值
                        Pop(&OPTR, &top);
                        Pop(&OPND, &num2);
                        Pop(&OPND, &num1);
                        
                        
                        Calculate(num1, num2, top); //压入结果
                        if(c != '#') {
                            Push(&OPTR, c); //这时候压入所输入的操作符
    
                        } else {
                            Pop(&OPTR, &top);
                            Pop(&OPND, &num2);
                            Pop(&OPND, &num1);

                            Calculate(num1, num2, top); //压入结果
                            
                        }
                    }
                    if(c != '#')
                        c = getchar();
                    break;
                    
                default:
                    break;
            }
            
        }
        GetTop(&OPTR, &top);
    }
    
    return 1;
    
}

Status main() {
    InitStack();

    EvaluateExpression();
    
    int a;
    GetTop(&OPND, &a);
    printf("结果:%d\n", a);

	system("pause");
    return 1;
}

