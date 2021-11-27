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

const char table[7][7] = {
    //               +-  *///  ..   rel   i    (    )    $
    /*  + -      */ {'>', '<', '>', '>', '<', '<', '>', '>'},
    /*  * / //   */ {'>', '>', '>', '>', '<', '<', '>', '>'},
    /*  ..       */ {'<', '<', '<', '>', '<', '<', '>', '>'},
    /*  rel      */ {'<', '<', '<', '>', '<', '<', '>', '>'},
    /*  i        */ {'>', '>', '>', '>', 'e', 'e', '>', '>'},
    /*  (        */ {'<', '<', '<', '<', '>', '>', '=', 'e'},
    /*  )        */ {'>', '>', '>', '>', 'e', 'e', '>', '>'},
    /*  $        */ {'<', '<', '<', '<', '<', '<', 'e', 'e'},
};

TStack expression_stack;
Token act_token;
Token dollar;

int eol()
{
    int output = ERROR_OK;
    if (act_token.type == EOL)
    {
        output = get_token(&act_token);
        eol();
    }
    return output;
}

int expression_analysis(Token *token)
{
    Stack_Init(&expression_stack);
    dollar.type = DOLLAR; //TODO
    act_token = *token;
    int output = ERROR_OK;

    if (act_token.type == EOL)
    {
        output = eol();
    }

    if (output != ERROR_OK)
    {
        return ERROR_LEXICAL_ANALISYS;
    }
    else
    {
        if (act_token.type == STATE_EOF)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
        else
            output = analysis();
    }
    return output;
}

int analysis()
{
    int output = ERROR_OK;
    int i1, i2;
    output = find_index(&i1, &i2);
    if (output != ERROR_OK)
    {
        return output;
    }

    bool handle;
    bool terminal;

    switch (table[i1][i2])
    {

    case '<':

        expression_stack.top->handle = true;
        handle = false;
        terminal = true;
        Stack_Push(&expression_stack, act_token, handle, terminal);

        output = get_token(&act_token);
        output = eol();

        if (output != ERROR_OK)
        {
            return ERROR_LEXICAL_ANALISYS;
        }
        else
        {
            if (act_token.type == STATE_EOF)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
        }
        break;

    case '>':
        //TODO
        break;

    case '=':
        handle = false;
        terminal = true;
        Stack_Push(&expression_stack, act_token, handle, terminal);

        output = get_token(&act_token);
        output = eol();

        if (output != ERROR_OK)
        {
            return ERROR_LEXICAL_ANALISYS;
        }
        else
        {
            if (act_token.type == STATE_EOF)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
        }
        break;

    case 'e':
        //TODO
        break;
    }

    output = analysis();

    return output;
}

int find_index(int *i1, int *i2)
{
    if ((expression_stack.top->token.type == PLUS) || (expression_stack.top->token.type == MINUS))
    {
        *i1 = 0;
    }
    else if ((expression_stack.top->token.type == MULTIPLY) || (expression_stack.top->token.type == DIVIDE) || (expression_stack.top->token.type == INTEGER_DIVIDE))
    {
        *i1 = 1;
    }
    else if (expression_stack.top->token.type == DOUBLEDOT)
    {
        *i1 = 2;
    }
    else if ((expression_stack.top->token.type == EG_ASSIGN) || (expression_stack.top->token.type == EQUALS) || (expression_stack.top->token.type == LESS_THAN) || (expression_stack.top->token.type == GREATER_THAN) || (expression_stack.top->token.type == LESS_or_EQUALS) || (expression_stack.top->token.type == GREATER_or_EQUALS))
    {
        *i1 = 3;
    }
    else if ((expression_stack.top->token.type == ID) || (expression_stack.top->token.type == NUMBER) || (expression_stack.top->token.type == HASH))
    {
        *i1 = 4;
    }
    else if (expression_stack.top->token.type == LEFT_PARENTHESIS)
    {
        *i1 = 5;
    }
    else if (expression_stack.top->token.type == RIGHT_PARENTHESIS)
    {
        *i1 = 6;
    }
    else if (expression_stack.top->token.type == DOLLAR /*TODO*/)
    {
        *i1 = 7;
    }
    else
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    if ((act_token.type == PLUS) || (act_token.type == MINUS))
    {
        *i2 = 0;
    }
    else if ((act_token.type == MULTIPLY) || (act_token.type == DIVIDE) || (act_token.type == INTEGER_DIVIDE))
    {
        *i2 = 1;
    }
    else if (act_token.type == DOUBLEDOT)
    {
        *i2 = 2;
    }
    else if ((act_token.type == EG_ASSIGN) || (act_token.type == EQUALS) || (act_token.type == LESS_THAN) || (act_token.type == GREATER_THAN) || (act_token.type == LESS_or_EQUALS) || (act_token.type == GREATER_or_EQUALS))
    {
        *i2 = 3;
    }
    else if ((act_token.type == ID) || (act_token.type == NUMBER) || (act_token.type == HASH))
    {
        *i2 = 4;
    }
    else if (act_token.type == LEFT_PARENTHESIS)
    {
        *i2 = 5;
    }
    else if (act_token.type == RIGHT_PARENTHESIS)
    {
        *i2 = 6;
    }
    else if (act_token.type == DOLLAR /*TODO*/)
    {
        *i2 = 7;
    }
    else
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    return ERROR_OK;
}