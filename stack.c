/**
* Project - Compiler for IFJ21
* 
* @brief Stack for syntax analysis of expressions
* 
* @author Marek Krizan <xkriza08>
* @file stack.c
*
**/
#include "scanner.h"
#include "stack.h"
#include <stdbool.h>
#include <stdlib.h>

void Stack_Init(TStack *stack)
{
    stack->top = NULL;
}

int Stack_Push(TStack *stack, Token *token)
{
    TStack_element *new = (TStack_element *)malloc(sizeof(TStack_element));
    if (new == NULL)
    {
        return 0;
    }
    else
    {
        new->data = token;
        new->next = stack->top;
        stack->top = new;
    }
    return 1;
}

void Stack_Pop(TStack *stack)
{
    TStack_element *elem;
    if (stack->top != NULL)
    {
        elem = stack->top;
        stack->top = stack->top->next;
        free(elem);
    }
}

void Stack_Top(TStack *stack, Token *token)
{
    token = stack->top->data;
}

bool Stack_IsEmpty(TStack *stack)
{
    return (stack->top == NULL);
}