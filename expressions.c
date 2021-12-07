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
ht_table_t *symtable;

int expression_analysis(Token *token, ht_table_t *symtable_ptr)
{
    symtable = symtable_ptr;
    Stack_Init(&expression_stack);
    dollar.type = DOLLAR;
    Stack_Push(&expression_stack, dollar, false, true);
    act_token = *token;
    int output = ERROR_OK;

    if (act_token.type == STATE_EOF)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    else
    {
        output = analysis();
        *token = act_token;
    }
    Stack_Dispose(&expression_stack);
    return output;
}

int analysis()
{
    int output = ERROR_OK;
    int i1 = 7, i2 = 7;
    if (act_token.type == GET_LENGTH)
    {
        hash();
    }

    output = find_index(&i1, &i2); 
    if (output != ERROR_OK)
    {
        return output;
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
        
        break;

    case '>':
        if (expression_stack.top->handle == true) //E -> id
        {
            expression_stack.top->terminal = false;
            expression_stack.top->handle = false;
        }
        else
        {
            TStack_element el1, el2, el3;
            el3 = *(Stack_Top(&expression_stack));
            Stack_Pop(&expression_stack);
            if (Stack_IsEmpty(&expression_stack))
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            el2 = *(Stack_Top(&expression_stack));
            Stack_Pop(&expression_stack);
            if (Stack_IsEmpty(&expression_stack))
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            el1 = *(Stack_Top(&expression_stack));
            Stack_Pop(&expression_stack);

            if (Stack_IsEmpty(&expression_stack) || el1.token.type == DOLLAR)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            output = reduce(el1, el2, el3); //E -> E ? E  alebo E -> (E)
              
            if((expression_stack.top->terminal == false)&&(expression_stack.top->next->token.type == DOLLAR)&&(i2==7))
            {
                return ERROR_OK;
            }
        }

        break;

    case '=':
        Stack_Push(&expression_stack, act_token, false, true);

        output = get_token(&act_token);

        if (output != ERROR_OK)
        {
            return output;
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
    new.value.integer_value = 1;
    new.value.decimal_value = 1;
    int output = ERROR_OK;
    Token_type return_type = ERROR;
    if (!el2.terminal) //E -> (E)
    {
        Stack_Push(&expression_stack, el2.token, false, false);
    }
    else
    {

        switch (el2.token.type) //TODO zavolaj code gen
        {
        case PLUS:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case MINUS:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case MULTIPLY:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case DIVIDE:
            if((el3.token.type == INT) && (el3.token.value.integer_value == 0)){
                return ERROR_RUNTIME_DIVISON_BY_ZERO;
            }
            if((el3.token.type == NUMBER) && (el3.token.value.decimal_value == 0)){
                return ERROR_RUNTIME_DIVISON_BY_ZERO;
            }
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case INTEGER_DIVIDE:
            if((el3.token.type == INT) && (el3.token.value.integer_value == 0)){
                return ERROR_RUNTIME_DIVISON_BY_ZERO;
            }
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case GREATER_THAN:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case LESS_THAN:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case GREATER_or_EQUALS:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case LESS_or_EQUALS:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case EQUALS:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case EG_ASSIGN:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);
            if(output!=ERROR_OK)
            {
                return output;
            }
            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        case CONCATENATE:
            output = check_id_and_type(&el1, &el2, &el3, &return_type);

            if(output!=ERROR_OK)
            {
                return output;
            }

            new.type = return_type;
            Stack_Push(&expression_stack, new, false, false);
            break;

        default:
            return ERROR_SYNTAX_ANALYSIS;
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
    else if ((expression_stack.top->token.type == ID) || (expression_stack.top->token.type == NUMBER) || (expression_stack.top->token.type == INT) || (expression_stack.top->token.type == STRING))
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
        return ERROR_INTERN;
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
    else if ((act_token.type == ID) || (act_token.type == NUMBER) || (act_token.type == INT) || (act_token.type == STRING))
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
    else
    {
        *i2 = 7;
    }

    return ERROR_OK;
}

int hash()
{
    int output = ERROR_OK;
    output = get_token(&act_token);
    if (output != ERROR_OK)
    {
        return output;
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
    new.type = INT;
    new.value.integer_value = str_len;
    act_token = new;

    return output;
}

int check_id_and_type(TStack_element *el1, TStack_element *el2, TStack_element *el3, Token_type *return_type)
{
                                //fprintf(stdout,"%d-%d-%d\n",el1->token.type,el2->token.type,el3->token.type); Pomocný výpis, ako Marek povedal
    Token_type type1, type2;
    ht_item_t *var = NULL;
    if ((el2->token.type == PLUS) || (el2->token.type == MINUS) || (el2->token.type == MULTIPLY) || (el2->token.type == DIVIDE) || (el2->token.type == INTEGER_DIVIDE)) //+-*///
    {

        if (el1->token.type == INT)
        {
            type1 = INT;
        }
        else if (el1->token.type == NUMBER)
        {
            type1 = NUMBER;
        }
        else if (el1->token.type == ID)
        {
            var = ht_search(symtable, el1->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if ((var->var_type != NUMBER) && (var->var_type != INT))
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type1 = var->var_type;
            }
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        if (el3->token.type == INT)
        {
            type2 = INT;
        }
        else if (el3->token.type == NUMBER)
        {
            type2 = NUMBER;
        }
        else if (el3->token.type == ID)
        {
            var = ht_search(symtable, el3->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if ((var->var_type != NUMBER) && (var->var_type != INT))
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type2 = var->var_type;
            }
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        if ((type1 == INT) && (type2 == INT))
        {
            *return_type = INT;
            return ERROR_OK;
        }
        else
        {
            if (el2->token.type == INTEGER_DIVIDE)
            {
                return ERROR_SEMANTIC_EXPRESSION_TYPE;
            }
            *return_type = NUMBER;
            return ERROR_OK;
        }
    }
    else if ((el2->token.type == CONCATENATE)) //..
    {
      
        if (el1->token.type == STRING)
        {
            type1 = STRING;
        }
        else if (el1->token.type == ID)
        {
            var = ht_search(symtable, el1->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if (var->var_type != STRING)
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type1 = var->var_type;
            }
        }
        else
        {  
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        if (el3->token.type == STRING)
        {
            type2 = STRING;
        }
        else if (el3->token.type == ID)
        {
            var = ht_search(symtable, el3->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if (var->var_type != STRING)
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type2 = var->var_type;
            }
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        *return_type = STRING;
        return ERROR_OK;
    }
    else if ((el2->token.type == GREATER_THAN) || (el2->token.type == GREATER_or_EQUALS) || (el2->token.type == LESS_THAN) || (el2->token.type == LESS_or_EQUALS)) // <= => < >
    {
        if (el1->token.type == INT)
        {
            type1 = INT;
        }
        else if (el1->token.type == NUMBER)
        {
            type1 = NUMBER;
        }
        else if (el1->token.type == ID)
        {
            var = ht_search(symtable, el1->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if ((var->var_type != NUMBER) && (var->var_type != INT))
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type1 = var->var_type;
            }
        }
        else if (el1->token.type == STRING)
        {
            type1 = STRING;
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        if (el3->token.type == INT)
        {
            type2 = INT;
        }
        else if (el3->token.type == NUMBER)
        {
            type2 = NUMBER;
        }
        else if (el3->token.type == ID)
        {
            var = ht_search(symtable, el3->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if ((var->var_type != NUMBER) && (var->var_type != INT))
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type2 = var->var_type;
            }
        }
        else if (el3->token.type == STRING)
        {
            type2 = STRING;
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        if (type1 == type2)
        {
            *return_type = INT;
        }
        else if (((type1 == INT) && (type2 == NUMBER)) || ((type2 == INT) && (type1 == NUMBER)))
        {
            *return_type = INT;
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        return ERROR_OK;
    }
    else if ((el2->token.type == EQUALS) || (el2->token.type == EG_ASSIGN)) // == ~=
    {
        if (el1->token.type == INT)
        {
            type1 = INT;
        }
        else if (el1->token.type == NUMBER)
        {
            type1 = NUMBER;
        }
        else if (el1->token.type == ID)
        {
            var = ht_search(symtable, el1->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if ((var->var_type != NUMBER) && (var->var_type != INT))
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type1 = var->var_type;
            }
        }
        else if (el1->token.type == STRING)
        {
            type1 = STRING;
        }
        else if ((el1->token.type == KEYWORD) && (el1->token.value.keyword == KEYWORD_NIL))
        {
            type1 = KEYWORD;
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        if (el3->token.type == INT)
        {
            type2 = INT;
        }
        else if (el3->token.type == NUMBER)
        {
            type2 = NUMBER;
        }
        else if (el3->token.type == ID)
        {
            var = ht_search(symtable, el3->token.value.string.string);
            if (var == NULL)
            {
                return ERROR_SEMANTIC;
            }
            else
            {
                if ((var->var_type != NUMBER) && (var->var_type != INT))
                {
                    return ERROR_SEMANTIC_EXPRESSION_TYPE;
                }
                type2 = var->var_type;
            }
        }
        else if (el3->token.type == STRING)
        {
            type2 = STRING;
        }
        else if ((el3->token.type == KEYWORD) && (el3->token.value.keyword == KEYWORD_NIL))
        {
            type2 = KEYWORD;
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        if (type1 == type2)
        {
            *return_type = INT;
        }
        else if (((type1 == INT) && (type2 == NUMBER)) || ((type2 == INT) && (type1 == NUMBER)))
        {
            *return_type = INT;
        }
        else
        {
            return ERROR_SEMANTIC_EXPRESSION_TYPE;
        }

        return ERROR_OK;
    }
    return ERROR_OK;
}