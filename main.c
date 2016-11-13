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

Stack OPRT, OPND; //OPTR寄存运算符，OPND寄存操作数　operator/operand

Status GetTop(pStack s, int *e) {
    if(s->base == s->top) {
        return printf("空栈top");
    }
    
    
    *e = *(s->top - 1);

    if(s == &OPND)
    {
        printf("OPND栈顶元素值为%d\n", *e);
    } else {
        printf("OPRT栈顶元素值为%c\n", *e);
    }
    
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
        printf("OPND入栈,值为%d\n", *s->top);
    } else {
        printf("OPRT入栈,值为%c\n", *s->top);
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
        printf("OPND出栈,值为%d\n", *s->top);
    } else {
        printf("OPRT出栈,值为%c\n", *s->top);
    }
    
    return 1;
}



Status InitStack() { //初始化两栈，并PUSH #进运算符栈
    OPRT.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPRT.top = OPRT.base;
    OPRT.stacksize = STACK_INIT_SIZE;
    Push(&OPRT, '#');
    
    OPND.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPND.top = OPND.base;
    OPND.stacksize = STACK_INIT_SIZE;
    
    return 1;
}

char Precede(char input, char top) {
    int a, b; //+ - 为0，* /为1，()为2
    
	if (top == '+' || top == '-')
		a = 0;
	else if (top == '*' || top == '\\')
		a = 1;
	else if (top == '#')
		a = -1;
	else //括号
        a = 2;
    
    if(input == '+' || input == '-')
        b = 0;
    else if(input == '*' || input == '\\')
        b = 1;
    else if (input == '#')
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

Status Calculate() { //取两个数字和一个操作符，压入结果
    int num1, num2, oprt;
    
    Pop(&OPRT, &oprt);
    Pop(&OPND, &num2);
    Pop(&OPND, &num1);
        
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
    int input, rttop;
    int numtemp, numcount; //解决非1位数字输入
    printf("请输入表达式，以#表示结束\n");
    
    input = getchar();
    GetTop(&OPRT, &rttop);
    numtemp = numcount = 0;
    
    while (rttop != '#' || input != '#') {
        if('0' <= input && input <= '9') { //输入的是数字
            numtemp += charToInt(input) * pow(10, numcount);
            numcount++;
            input = getchar();
        } else {
            if(numcount > 0) //如果前面是数字，则数字已经输入完，压入
            {
                Push(&OPND, numtemp);
                numcount = numtemp = 0;
            }
            
            switch (Precede(input, rttop)) {
                case '>': //如果不是)则直接压入，否则计算括号内并消去( 3*((5-2)+4*(3+1))
                    if(input == ')') {
                        while (rttop != '(') { //括号内还剩下2个符号的情况 e.g (3+4*4)
                            Calculate();
                            GetTop(&OPRT, &rttop);
                        }
                        Pop(&OPRT, &rttop); //消去( ，rttop这里只是摆设
                    } else {
                        Push(&OPRT, input);
                    }
                    
                    input = getchar();
                    break;
                
                case '=': //若输入(，则直接压入，否则计算上一个操作符
                    if(input == '(') {
                        Push(&OPRT, input);
                    } else {
                        Calculate();
                        Push(&OPRT, input);
                    }
                    
                    input = getchar();
                    break;
                    
                case '<': //若输入是#，则继续计算上一个操作符
                    if(input == '#') {
                        Calculate();
                    } else if(rttop == '(') {
                        Push(&OPRT, input);
                        input = getchar();
                    } else { //否则计算后压入输入
                        Calculate();
                        Push(&OPRT, input);
                        input = getchar();
                    }
                    break; 
                    
                default:
                    break;
            }
            
            GetTop(&OPRT, &rttop);
        }
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

