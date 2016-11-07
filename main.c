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

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10

typedef int Status;
typedef int ElemType;

typedef struct {
    ElemType *base;     //栈底指针
    ElemType *top;      //栈顶指针
    int stacksize;      //栈大小
}Stack;

Status GetTop(Stack s, char e) {
    if(s.base == s.top)
        return printf("空栈");
    
    e = *(s.top - 1);
    
    return 1;
}

Status Push(Stack s, char e) {
    if(s.top - s.base >= s.stacksize) { //若栈满，追加空间
        s.base = (ElemType *) realloc (s.base, (s.stacksize + STACK_INIT_SIZE) * sizeof(ElemType));
        s.top = s.base + s.stacksize; //realloc有可能改变base
        s.stacksize += STACK_INCREMENT;
    }
    
    *s.top++ = e;
    
    return 1;
}

Status Pop(Stack s, char e) {
    if(s.base == s.top)
        return printf("空栈");
    
    e = *--s.top;
    
    return 1;
}


Stack OPTR, OPND; //OPTR寄存运算符，OPND寄存操作数　operator/operand

Status InitStack() {
    OPTR.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPTR.top = OPTR.base;
    OPTR.stacksize = STACK_INIT_SIZE;
    Push(OPTR, '#');
    
    OPND.base = (ElemType *) malloc (STACK_INIT_SIZE * sizeof(ElemType));
    OPND.top = OPND.base;
    OPND.stacksize = STACK_INIT_SIZE;
    
    return 1;
}



Status main() {
    InitStack();
    printf("%c\n", *OPTR.base);
    Push(OPTR, 'h');
    printf("%c", *(OPTR.base + 7));
    //暂时无解
	system("pause");

    return 1;
}

