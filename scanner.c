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

int identifier_check(Dyn_string *dynamic_string, Token *token)
{
    if (strcmp(dynamic_string->string, "do"))
        (*token).value.keyword = KEYWORD_DO;
    else if (strcmp(dynamic_string->string, "else"))
        (*token).value.keyword = KEYWORD_ELSE;
    else if (strcmp(dynamic_string->string, "end"))
        (*token).value.keyword = KEYWORD_END;
    else if (strcmp(dynamic_string->string, "function"))
        (*token).value.keyword = KEYWORD_FUNCTION;
    else if (strcmp(dynamic_string->string, "global"))
        (*token).value.keyword = KEYWORD_GLOBAL;
    else if (strcmp(dynamic_string->string, "if"))
        (*token).value.keyword = KEYWORD_IF;
    else if (strcmp(dynamic_string->string, "integer"))
        (*token).value.keyword = KEYWORD_INTEGER;
    else if (strcmp(dynamic_string->string, "local"))
        (*token).value.keyword = KEYWORD_LOCAL;
    else if (strcmp(dynamic_string->string, "nil"))
        (*token).value.keyword = KEYWORD_NIL;
    else if (strcmp(dynamic_string->string, "number"))
        (*token).value.keyword = KEYWORD_NUMBER;
    else if (strcmp(dynamic_string->string, "require"))
        (*token).value.keyword = KEYWORD_REQUIRE;
    else if (strcmp(dynamic_string->string, "return"))
        (*token).value.keyword = KEYWORD_RETURN;
    else if (strcmp(dynamic_string->string, "string"))
        (*token).value.keyword = KEYWORD_STRING;
    else if (strcmp(dynamic_string->string, "then"))
        (*token).value.keyword = KEYWORD_THEN;
    else if (strcmp(dynamic_string->string, "while"))
        (*token).value.keyword = KEYWORD_WHILE;
    else
    {
        token->type = ID;
    }

    /*if (!dyn_string_copy(dynamic_string, token->value.string))
    {
        return ERROR_INTERN;                                            --->  kinda useless
    }*/

    if (token->type != ID)
        token->type = KEYWORD;
    {
        return ERROR_OK;
    }
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

                                        //fprintf(stdout, "\n--Kontrola---\n"); putc(next_char,stdout);fprintf(stdout, "\n"); // výpis obsahu tokenu

        switch (state)
        {
            //---------------------------------------------------------------//

        case START:

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
                if (!dyn_string_add_char(dynamic_string, next_char))
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
                state = COLON;

            else
            {

                return free_memory(ERROR_INTERN, string);

                break;
                //---------------------------------------------------------------//

            case (ID_or_KEYWORD):
                if (isalnum(next_char) || next_char == '_')
                {
                    if (!dyn_string_add_char(string, (char)tolower(next_char)))
                    {

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
                //if number is int . strtoi, if decimal . check after point for another number and back to decimal state, in both check for E
                if (isdigit(next_char))
                {
                    if (!dyn_string_add_char(dynamic_string, next_char))
                    {

                        return free_memory(ERROR_LEXICAL_ANALISYS, string);
                    }
                }
                else if (next_char == '.')
                {
                    ///state = DECIMAL_POINT;
                    //if (!dynamic_string_add_char(dynamic_string, next_char))
                    //{
                    //	return token;
                    //}
                }
                else if (tolower(next_char) == 'e')
                {
                    state = INDEX_CHAR;
                    if (!dyn_string_add_char(dynamic_string, next_char))
                    {

                        return free_memory(ERROR_LEXICAL_ANALISYS, string);
                    }
                }
                else
                {
                    ungetc(next_char, stdin);
                    //	return process_integer(dynamic_string, token);
                }

                break;
            }
            //---------------------------------------------------------------//
        }
    }
}
