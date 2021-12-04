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

const char table[8][8] = {
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

int expression_analysis(Token *token)
{
    Stack_Init(&expression_stack);
    dollar.type = DOLLAR;
    bool handle = false;
    bool terminal = false;
    Stack_Push(&expression_stack, dollar, handle, terminal);
    act_token = *token;
    int output = ERROR_OK;

    if (act_token.type == STATE_EOF)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    else
        output = analysis();
    return output;
}

int analysis()
{
    int output = ERROR_OK;
    int i1, i2;
    if (act_token.type == HASH)
    {
        hash();
    }

    output = find_index(&i1, &i2);
    if (output != ERROR_OK)
    {
        return output;
    }

    if ((i1 == 7) && (expression_stack.top->terminal == false))
    {
        return ERROR_OK;
    }

    switch (table[i1][i2])
    {

    case '<':

        Stack_Push(&expression_stack, act_token, true, true);

        output = get_token(&act_token);

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
        if (expression_stack.top->handle == true)
        {
            expression_stack.top->terminal = false;
            expression_stack.top->handle = false;
        }
        else
        {
            TStack_element el1, el2, el3;                   //TODO osetrenia
            Stack_Top(&expression_stack, &el3);
            Stack_Pop(&expression_stack);
            Stack_Top(&expression_stack, &el2);
            Stack_Pop(&expression_stack);
            Stack_Top(&expression_stack, &el1);
            Stack_Pop(&expression_stack);

            if (Stack_IsEmpty(&expression_stack))
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            output = reduce(el1, el2, el3);
        }

        break;

    case '=':
        Stack_Push(&expression_stack, act_token, false, true);

        output = get_token(&act_token);

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
        if ((i1 == 7) && (i2 == 7))
        {
            return ERROR_OK;
        }
        return ERROR_SYNTAX_ANALYSIS;
        break;
    }

    if (output != ERROR_OK)
    {
        return output;
    }
    output = analysis();

    return output;
}

int reduce(TStack_element el1, TStack_element el2, TStack_element el3)
{
    Token new;
    int output = ERROR_OK;
    if (!el2.terminal)
    {
        Stack_Push(&expression_stack, el2.token, false, false);
    }
    else
    {

        check_id(&el1,&el2,&el3);
        switch (el2.token.type)
        {
        case PLUS:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case MINUS:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case MULTIPLY:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case DIVIDE:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case INTEGER_DIVIDE:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case GREATER_THAN:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case LESS_THAN:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case GREATER_or_EQUALS:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case LESS_or_EQUALS:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case EQUALS:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case EG_ASSIGN:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        case CONCATENATE:
            new.type = NUMBER;
            //TODO new.value =
            Stack_Push(&expression_stack, new, false, false);
            break;

        default:
            break;
        }
    }
    return output;
}

int find_index(int *i1, int *i2)
{
    TStack_element tmp = *(expression_stack.top);
    bool popped = false;

    if (!tmp.terminal)
    {
        Stack_Pop(&expression_stack);
        popped = true;
    }

    if ((expression_stack.top->token.type == PLUS) || (expression_stack.top->token.type == MINUS))
    {
        *i1 = 0;
    }
    else if ((expression_stack.top->token.type == MULTIPLY) || (expression_stack.top->token.type == DIVIDE) || (expression_stack.top->token.type == INTEGER_DIVIDE))
    {
        *i1 = 1;
    }
    else if (expression_stack.top->token.type == CONCATENATE)
    {
        *i1 = 2;
    }
    else if ((expression_stack.top->token.type == EG_ASSIGN) || (expression_stack.top->token.type == EQUALS) || (expression_stack.top->token.type == LESS_THAN) || (expression_stack.top->token.type == GREATER_THAN) || (expression_stack.top->token.type == LESS_or_EQUALS) || (expression_stack.top->token.type == GREATER_or_EQUALS))
    {
        *i1 = 3;
    }
    else if ((expression_stack.top->token.type == ID) || (expression_stack.top->token.type == NUMBER)) //TODO moze byt asi aj int a tak
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
    else if (expression_stack.top->token.type == DOLLAR)
    {
        *i1 = 7;
    }
    else
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    if (popped)
    {
        Stack_Push(&expression_stack, tmp.token, tmp.handle, tmp.terminal);
    }

    if ((act_token.type == PLUS) || (act_token.type == MINUS))
    {
        *i2 = 0;
    }
    else if ((act_token.type == MULTIPLY) || (act_token.type == DIVIDE) || (act_token.type == INTEGER_DIVIDE))
    {
        *i2 = 1;
    }
    else if (act_token.type == CONCATENATE)
    {
        *i2 = 2;
    }
    else if ((act_token.type == EG_ASSIGN) || (act_token.type == EQUALS) || (act_token.type == LESS_THAN) || (act_token.type == GREATER_THAN) || (act_token.type == LESS_or_EQUALS) || (act_token.type == GREATER_or_EQUALS))
    {
        *i2 = 3;
    }
    else if ((act_token.type == ID) || (act_token.type == NUMBER)) //TODO moze byt asi aj int a tak
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
    else if (act_token.type == DOLLAR)
    {
        *i2 = 7;
    }
    else
    {
        if ((*i1 == 7) && (expression_stack.top->terminal == false))
        {
            return ERROR_OK;
        }
        return ERROR_SYNTAX_ANALYSIS;
    }

    return ERROR_OK;
}

int hash()
{
    int output = ERROR_OK;
    output = get_token(&act_token);
    if (output != ERROR_OK)
    {
        return ERROR_LEXICAL_ANALISYS;
    }
    else
    {
        if ((act_token.type == STATE_EOF) || (act_token.type != STRING))
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }
    Token new;
    int str_len = strlen(act_token.value.string.string);
    new.type = NUMBER;
    new.value.integer_value = str_len;
    act_token = new;

    return output;
}

int check_id(TStack_element *el1, TStack_element *el2, TStack_element *el3){
    //TODO
}