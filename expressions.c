/**
* Project - Compiler for IFJ21
* 
* @brief Syntax analysis of expressions
* 
* @author Marek Krizan <xkriza08>
* @file expressions.c
*
**/

#include "expressions.h"
#include "scanner.h"
#include "stack.h"
#include "error_codes.h"
#include "parser.h"

Token *act_token;

int expression_analysis(Token *token){
    TStack expression_stack;
    Stack_Init(&expression_stack);                  //stack init
    act_token = token;                          
    int output = ERROR_OK;

    if(act_token->type == EOL){
        output = Is_Eol();
    }

    if (output != ERROR_OK)
    {
        return ERROR_LEXICAL_ANALISYS;
    }
    else
    {
        if(actToken.type == STATE_EOF){
            return ERROR_SYNTAX_ANALYSIS;
        }
        else
            output = analysis();
    }
    return output;
}

int analysis(){

}