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
#include <ctype.h>
#include <unistd.h>

char next_char;
int state = START;
Token *token;

FILE *source;               // Source file that will be scanned
Dyn_string *dynamic_string; // Dynamic string that will be written into

/*
Fixed enough to be without errors. 
In special cases we need to check for the succeeding character to determine token type 
Function to choose keywords would be useful
*/

int identifier_check(Dyn_string *dynamic_string, Token *token){   
    if (strcmp(dynamic_string->string, "do")) token->value.keyword=KEYWORD_DO;
    else if (strcmp(dynamic_string->string, "else")) token->value.keyword=KEYWORD_ELSE;
    else if (strcmp(dynamic_string->string, "end")) token->value.keyword=KEYWORD_END;
    else if (strcmp(dynamic_string->string, "function")) token->value.keyword=KEYWORD_FUNCTION;
    else if (strcmp(dynamic_string->string, "global")) token->value.keyword=KEYWORD_GLOBAL;
    else if (strcmp(dynamic_string->string, "if")) token->value.keyword=KEYWORD_IF;
    else if (strcmp(dynamic_string->string, "integer")) token->value.keyword=KEYWORD_INTEGER;
    else if (strcmp(dynamic_string->string, "local")) token->value.keyword=KEYWORD_LOCAL;
    else if (strcmp(dynamic_string->string, "nil")) token->value.keyword=KEYWORD_NIL;
    else if (strcmp(dynamic_string->string, "number")) token->value.keyword=KEYWORD_NUMBER;
    else if (strcmp(dynamic_string->string, "require")) token->value.keyword=KEYWORD_REQUIRE;
    else if (strcmp(dynamic_string->string, "return")) token->value.keyword=KEYWORD_RETURN;
    else if (strcmp(dynamic_string->string, "string")) token->value.keyword=KEYWORD_STRING;
    else if (strcmp(dynamic_string->string, "then")) token->value.keyword=KEYWORD_THEN;
    else if (strcmp(dynamic_string->string, "while")) token->value.keyword=KEYWORD_WHILE;
    else{
        token->type = ID;
    }

    if (!dyn_string_copy(dynamic_string, token->value.string)){
		return free_dynamic_string(dynamic_string,ERROR_INTERN);
	}
    if (token->type!=ID) token->type = KEYWORD;
    return free_dynamic_string(dynamic_string, ERROR_OK);
}


int free_dynamic_string(Dyn_string *dynamic_string, int Exit_code)
{

    dyn_string_free(dynamic_string);
    return Exit_code;
}

int get_token(Token *token)
{

    if ((!dyn_string_init(dynamic_string)) || (source == NULL))
    {
        return ERROR_INTERN;
    }

    while (1)
    {

        next_char = getc(source);
        token->value.string = dynamic_string;


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
            else if (isalpha(next_char) || next_char == '_')
            {
                if (!dyn_string_add_char(dynamic_string, (char)tolower(next_char)))
                {
                    return free_dynamic_string(dynamic_string, ERROR_INTERN);
                }
                state = ID_or_KEYWORD;
            }

            else if (isdigit(next_char))
            {
                if (!dyn_string_add_char(dynamic_string, next_char))
                {
                    return free_dynamic_string(dynamic_string, ERROR_INTERN);
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
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == '-')
            {
                token->type = MINUS;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == '*')
            {
                token->type = MULTIPLY;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == '(')
            {
                token->type = LEFT_PARENTHESIS;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == ')')
            {
                token->type = RIGHT_PARENTHESIS;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == ',')
            {
                token->type = COMMA;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == '\n')
            {
                token->type = EOL;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == EOF)
            {
                token->type = STATE_EOF;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
            }
            else if (next_char == '/')
            {
                token->type = DIVIDE;
                return free_dynamic_string(dynamic_string, EXIT_SUCCESS);
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
				return free_dynamic_string(dynamic_string,ERROR_LEXICAL_ANALISYS);
			}

			break; 
//---------------------------------------------------------------//

case (ID_or_KEYWORD):
				if (isalnum(next_char) || next_char == '_')
				{
					if (!dyn_string_add_char(dynamic_string, (char) tolower(next_char)))
					{
						return free_dynamic_string(dynamic_string,ERROR_INTERN);
					}
				}
				else
				{
					ungetc(next_char, source);
					return identifier_check(dynamic_string, token);
				}

				break;

//---------------------------------------------------------------//   

        case(LESS_THAN):

            if (next_char == '=')
				{
				
					token->type = LESS_or_EQUALS;
				}
				else
				{
					ungetc(next_char, source);
					token->type = LESS_THAN;
				}

				return free_dynamic_string(dynamic_string,ERROR_OK);


//---------------------------------------------------------------//


        case(GREATER_THAN):

            if (next_char == '=')
				{
				
					token->type = GREATER_or_EQUALS;
				}
				else
				{
					ungetc(next_char, source);
					token->type = GREATER_THAN;
				}

				return free_dynamic_string(dynamic_string,ERROR_OK);

//---------------------------------------------------------------//


        case(ASSIGN):

            if (next_char == '=')
				{
				
					token->type = EQUALS;
				}
				else
				{
					ungetc(next_char, source);
					token->type = ASSIGN;
				}

				return free_dynamic_string(dynamic_string,ERROR_OK);

//---------------------------------------------------------------//


        case(DIVIDE):

            if (next_char == '/')
				{
				
					token->type = INTEGER_DIVIDE;
				}
				else
				{
					ungetc(next_char, source);
					token->type = DIVIDE;
				}

				return free_dynamic_string(dynamic_string,ERROR_OK);

//---------------------------------------------------------------//


        case (NUMBER):                                                                      
        //if number is int -> strtoi, if decimal -> check after point for another number and back to decimal state, in both check for E
				if (isdigit(next_char))
				{
					if (!dyn_string_add_char(dynamic_string, next_char))
					{
						return free_dynamic_string(dynamic_string,ERROR_INTERN);
					}
				}
				else if (next_char == '.')
				{
					/*state = DECIMAL_POINT;
					if (!dynamic_string_add_char(dynamic_string, next_char))
					{
						return free_dynamic_string(dynamic_string,ERROR_INTERN);
					}*/
				}
				else if (tolower(next_char) == 'e')
				{
					state = INDEX_CHAR;
					if (!dyn_string_add_char(dynamic_string, next_char))
					{
						return free_dynamic_string(dynamic_string,ERROR_INTERN);
					}
				}
				else
				{
					ungetc(next_char, source);
				//	return process_integer(dynamic_string, token);
				}

				break;

    }
//---------------------------------------------------------------//

    }
    return 0;
}
