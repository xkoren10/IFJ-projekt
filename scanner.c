/**
* Project - Compiler for IFJ21
* 
* @brief Scanner for IFJ21 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file scanner.c
*
**/

#include "scanner.h"

Dyn_string *dynamic_string; // Dynamic string that will be written into
FILE *source_file;
Dyn_string *source_string;
bool decimal;

/*
Fixed enough to be without errors. 
In special cases we need to check for the succeeding character to determine token type 
Function to choose keywords would be useful
*/
void set_source(FILE *f)
{
    source_file = f;
}

void set_string(Dyn_string *string)
{
    source_string = string;
}

int free_memory(int exit_code, Dyn_string *string)
{
    dyn_string_free(string);
    return exit_code;
}

 int process_integer(Dyn_string *dynamic_string, Token *token){
    char *ptr;
    int integer = (int)strtol(dynamic_string->string, &ptr, 10);

    if (*ptr)
	{
		return free_memory(ERROR_INTERN, dynamic_string);
	}

    (*token).value.integer_value = integer;
    (*token).type = INT;

    return ERROR_OK;
}

 int process_float(Dyn_string  *dynamic_string, Token *token){
    char *ptr;
    double double_number = strtod(dynamic_string->string, &ptr);

    if (*ptr)
	{
		return free_memory(ERROR_INTERN, dynamic_string);
	}

    (*token).value.decimal_value = double_number;
    (*token).type = DECIMAL_NUMBER;
    return ERROR_OK;
}

int identifier_check(Dyn_string *dynamic_string, Token *token)
{
    if (strcmp(dynamic_string->string, "do") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_DO;
    }
    else if (strcmp(dynamic_string->string, "else") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_ELSE;
    }
    else if (strcmp(dynamic_string->string, "end") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_END;
    }
    else if (strcmp(dynamic_string->string, "function") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_FUNCTION;
    }
    else if (strcmp(dynamic_string->string, "global") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_GLOBAL;
    }
    else if (strcmp(dynamic_string->string, "if") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_IF;
    }
    else if (strcmp(dynamic_string->string, "integer") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_INTEGER;
    }
    else if (strcmp(dynamic_string->string, "local") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_LOCAL;
    }
    else if (strcmp(dynamic_string->string, "nil") == 0)
    {
        token->type = KEYWORD;
        (*token).value.keyword = KEYWORD_NIL;
    }
    else if (strcmp(dynamic_string->string, "number") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_NUMBER;
    }
    else if (strcmp(dynamic_string->string, "require") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_REQUIRE;
    }
    else if (strcmp(dynamic_string->string, "return") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_RETURN;
    }
    else if (strcmp(dynamic_string->string, "string") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_STRING;
    }
    else if (strcmp(dynamic_string->string, "then") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_THEN;
    }
    else if (strcmp(dynamic_string->string, "while") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_WHILE;
    }

    else
    {
        (*token).type = ID;
    }
    return ERROR_OK;
}

int get_token(Token *token)

{

    if (source_file == NULL)
    {
        return ERROR_INTERN;
    }

    int state = START;

    token->value.string = source_string;
    token->type = state;
    token->start = token->end = token->lenght = 0;
    token->line = 1;

    Dyn_string tmpstring;
    Dyn_string *string = &tmpstring;

    char next_char;

    if (dyn_string_init(string) != 0)
    {
        return ERROR_INTERN;
    }

    token->value.string = dynamic_string;

    while (true)
    {
        token->lenght++;
        next_char = getc(source_file);

//------------------------------------------------
        // fprintf(stdout, "\n--Kontrola---\n");
        // putc(next_char, stdout);
        // fprintf(stdout, "\n"); // výpis obsahu tokenu
//--------------------------------------------------

        switch (state)

        {
            //---------------------------------------------------------------//

        case (START):

            if (isspace(next_char))
            {
                state = START;
                if (next_char != '\n' && next_char != EOF)
                    break;
            }
            else if ((isalpha(next_char) || next_char == '_') && next_char != EOF)
            {
                if (!dyn_string_add_char(string, (char)tolower(next_char)))
                {
                    return free_memory(ERROR_INTERN, string);
                }
                state = ID_or_KEYWORD;
            }

            else if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    return free_memory(ERROR_INTERN, string);
                }
                state = NUMBER;
            }
            else if (next_char == '-')
            {
                state = LINE_COMMENTARY; //can be -something
            }

            else if (next_char == '+')
            {
                token->type = PLUS;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '-')
            {
                token->type = MINUS;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '*')
            {
                token->type = MULTIPLY;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '(')
            {
                token->type = LEFT_PARENTHESIS;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == ')')
            {
                token->type = RIGHT_PARENTHESIS;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == ',')
            {
                token->type = COMMA;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '\n')
            {
                token->type = EOL;
                token->line++;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == EOF)
            {
                token->type = STATE_EOF;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '/')
            {
                token->type = DIVIDE;

                return free_memory(ERROR_OK, string);
            }

            else if (next_char == '=')
            {
                state = ASSIGN;
            }
            else if (next_char == '<')
                state = LESS_THAN;

            else if (next_char == '>')
                state = GREATER_THAN;

            else if (next_char == ':')
            {
                token->type = COLON;

                return free_memory(ERROR_OK, string);
            }

            else
            {
                return free_memory(ERROR_INTERN, string);
            }
            break;
            //---------------------------------------------------------------//

        case (ID_or_KEYWORD):
            if (isalnum(next_char) || next_char == '_')
            {
                if (!dyn_string_add_char(string, (char)tolower(next_char)))
                {
                    (*token).type = ID;
                    return free_memory(ERROR_INTERN, string);
                }
                state = ID_or_KEYWORD;
            }
            else
            {
                return identifier_check(string, token);
            }

            break;

            //---------------------------------------------------------------//

        case (LESS_THAN):

            if (next_char == '=')
            {

                token->type = LESS_or_EQUALS;
            }
            else
            {
                ungetc(next_char, stdin);
                token->type = LESS_THAN;
            }
            return free_memory(ERROR_OK, string);
            break;

            //---------------------------------------------------------------//

        case (GREATER_THAN):

            if (next_char == '=')
            {

                token->type = GREATER_or_EQUALS;
            }
            else
            {
                ungetc(next_char, stdin);
                token->type = GREATER_THAN;
            }
            return free_memory(ERROR_OK, string);
            break;

            //---------------------------------------------------------------//

        case (ASSIGN):

            if (next_char == '=')
            {

                token->type = EQUALS;
            }
            else
            {
                ungetc(next_char, stdin);
                token->type = ASSIGN;
            }
            return free_memory(ERROR_OK, string);
            break;

            //---------------------------------------------------------------//

        case (DIVIDE):

            if (next_char == '/')
            {

                token->type = INTEGER_DIVIDE;
            }
            else
            {
                ungetc(next_char, stdin);
                token->type = DIVIDE;
            }
            return free_memory(ERROR_OK, string);
            break;

            //---------------------------------------------------------------//

        case (NUMBER): 
            
            if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {

                    return free_memory(ERROR_LEXICAL_ANALISYS, string);
                }
                state = NUMBER;
            }
            else if (next_char == '.')
            {
                
                if (!dyn_string_add_char(string, next_char))
                {
                	return free_memory(ERROR_LEXICAL_ANALISYS, string);
                }
                state = DECIMAL_POINT;
            }
            else if (tolower(next_char) == 'e')
            {
               
                if (!dyn_string_add_char(string, next_char))
                {

                    return free_memory(ERROR_LEXICAL_ANALISYS, string);
                }
                 state = INDEX_CHAR;
            }

            else {
                return process_integer(string, token);
            }

            break;
        
        //---------------------------------------------------------------//

        case (DECIMAL_POINT):
         if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    
                    return free_memory(ERROR_LEXICAL_ANALISYS, string);
                }
               
                state = DECIMAL_NUMBER;
            }
        else {
            return free_memory(ERROR_LEXICAL_ANALISYS, string);
            }

            break;
        //---------------------------------------------------------------//

        case(DECIMAL_NUMBER):
        if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {

                    return free_memory(ERROR_LEXICAL_ANALISYS, string);
                }
            }
            
            else if (tolower(next_char) == 'e')
            {
               
                if (!dyn_string_add_char(string, next_char))
                {

                    return free_memory(ERROR_LEXICAL_ANALISYS, string);
                }
                 state = INDEX_CHAR;
            }

            else {
                return process_float(string, token);
            }




        break;
        }
    }
}
