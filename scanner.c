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

FILE *source_file;
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

int free_memory(int exit_code, Dyn_string *string)
{
    dyn_string_free(string);
    return exit_code;
}

int process_integer(Dyn_string *dynamic_string, Token *token)
{
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

int process_float(Dyn_string *dynamic_string, Token *token)
{
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

    // Id check

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

    // Built-in check

        else if (strcmp(dynamic_string->string, "reads") == 0)
    {
        (*token).type = BUILT_IN_READS;
    }
            else if (strcmp(dynamic_string->string, "readi") == 0)
    {
        (*token).type = BUILT_IN_READI;
    }
            else if (strcmp(dynamic_string->string, "readn") == 0)
    {
        (*token).type = BUILT_IN_READN;
    }
            else if (strcmp(dynamic_string->string, "write") == 0)
    {
        (*token).type = BUILT_IN_WRITE;
    }
            else if (strcmp(dynamic_string->string, "tointeger") == 0)
    {
        (*token).type = BUILT_IN_TOINTEGER;
    }
            else if (strcmp(dynamic_string->string, "substr") == 0)
    {
        (*token).type = BUILT_IN_SUBSTR;
    }
            else if (strcmp(dynamic_string->string, "ord") == 0)
    {
        (*token).type = BUILT_IN_ORD;
    }
            else if (strcmp(dynamic_string->string, "chr") == 0)
    {
        (*token).type = BUILT_IN_CHR;
    }

    // Else it's ID

   else
    {
        (*token).type = ID;
        if (!dyn_string_copy(dynamic_string, &token->value.string))
        {
            return free_memory(ERROR_INTERN, dynamic_string);
        }
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
    int esc_cnt = 0;
    int esc_val = 0;

    token->type = state;

    Dyn_string tmpstring;
    Dyn_string *string = &tmpstring; // question

    char next_char;



   if (dyn_string_init(string) != 0)
    {
        return ERROR_INTERN;
    }

    while (true)
    {

        next_char = getc(source_file);

        /* if (next_char == '\n')
            {
                token->type = EOL;
                return free_memory(ERROR_OK, string);                       //dunno how
            }*/

        //------------------------------------------------
        //fprintf(stdout, "\n--Kontrola---\n");
        //putc(next_char, stdout);
        //fprintf(stdout, "\n"); // výpis obsahu tokenu
        //--------------------------------------------------

        switch (state)

        {
            //---------------------------------------------------------------//

        case (START):

            if (isspace(next_char) || next_char == '\n' || next_char == '\t')
            {
                state = START;

                if (next_char != '\n' && next_char != EOF)
                    break;
            }
            else if ((isalpha(next_char) || next_char == '_') && next_char != EOF)
            {

                ungetc(next_char, source_file); // lebo sak si zoberie hned pri starte jedno pismeno tak samozrejme ze nebude nic spravne na zaciatku
                state = ID_or_KEYWORD;          // dalsia vec, preco sa toto nepouziva cisto ako starting rozcestnik, ale robia sa tu veci aj s dyn_stringom?
                                                // nebolo by lepsie s tym narabat az v jednotlivych states?
            }
            else if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    return free_memory(ERROR_INTERN, string);
                }
                state = NUMBER;
            }

            else if (next_char == '+')
            {
                token->type = PLUS;

                return free_memory(ERROR_OK, string);
            }

             else if (next_char == '#')
            {
                token->type = GET_LENGTH;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '-')
            { // maybe check for EOF a bit more?

                next_char = getc(source_file);
                if (next_char == '-')
                {
                    state = LINE_COMMENTARY;
                    next_char = getc(source_file);
                    if (next_char == '[' && next_char != EOF)
                    {
                        next_char = getc(source_file);
                        if (next_char == '[' && next_char != EOF)
                        {
                            state = BLOCK_COMMENTARY;
                        }
                        else
                            state = LINE_COMMENTARY;
                    }
                    else
                        state = LINE_COMMENTARY;
                }
                else
                {
                    ungetc(next_char, source_file);
                    token->type = MINUS;
                    return free_memory(ERROR_OK, string);
                }
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

            else if (next_char == EOF)
            {
                token->type = STATE_EOF;

                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '/')
            {
                state = DIVIDE;
                
            }
            else if (next_char == '=')
            {

                state = ASSIGN;
            }
            else if (next_char == '~')
            {

                state = NOT_EQUAL;
            }
            else if (next_char == '<')
            {

                state = LESS_THAN;
            }
            else if (next_char == '>')
            {

                state = GREATER_THAN;
            }
            else if (next_char == '.')
            {

                 state = DOT;
            }
            else if (next_char == ':')
            {

                token->type = COLON;
                return free_memory(ERROR_OK, string);
            }
            else if (next_char == '"')
            {
                state = STRING;
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

                if (!dyn_string_add_char(string, next_char))
                {
                    (*token).type = ID;
                    return free_memory(ERROR_INTERN, string);
                }
                state = ID_or_KEYWORD;
            }
            else
            {
                ungetc(next_char, source_file); // maybe?
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
                ungetc(next_char, source_file);
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
                ungetc(next_char, source_file);
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
                ungetc(next_char, source_file);
                token->type = ASSIGN;
            }
            return free_memory(ERROR_OK, string);
            break;

            //---------------------------------------------------------------//

        case (NOT_EQUAL):

            if (next_char == '=')
            {

                token->type = NOT_EQUAL;
            }
            else
            {
               return free_memory(ERROR_LEXICAL_ANALISYS, string);                
            }
            return free_memory(ERROR_OK, string);
            break; 
            //---------------------------------------------------------------//
        case (DOT):
            if (next_char == '.')
            {

                token->type = CONCATENATE;
            }
            else
            {
                ungetc(next_char, source_file);
                return free_memory(ERROR_INTERN, string);
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
                ungetc(next_char, source_file);
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

            else
            {
                ungetc(next_char, source_file);
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
            else
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, string);
            }

            break;
            //---------------------------------------------------------------//

        case (DECIMAL_NUMBER):
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

            else
            {   
                ungetc(next_char,source_file);
                return process_float(string, token);
            }

            break;
        //---------------------------------------------------------------//
        case (INDEX_CHAR):
            if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    return free_memory(ERROR_INTERN, string);
                }
                state = EXPONENT_NUMBER;
            }
            else if (next_char == '+' || next_char == '-')
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    return free_memory(ERROR_INTERN, string);
                }
                state = EXPONENT_SIGN;
            }
            break;
            //---------------------------------------------------------------//

        case (EXPONENT_SIGN):
            if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    return free_memory(ERROR_INTERN, string);
                }
                state = EXPONENT_NUMBER;
            }
            else
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, string);
            }
            break;
        //---------------------------------------------------------------//
        case (EXPONENT_NUMBER):

            if (isdigit(next_char))
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    return free_memory(ERROR_INTERN, string);
                }
            }
            else
            {
                return process_float(string, token);
            }
            break;

        //---------------------------------------------------------------//
        case (LINE_COMMENTARY):

            if (next_char == '\n' || next_char == EOF)
                state = START; // check line counter
            break;

        case (BLOCK_COMMENTARY):
            if (next_char == ']' || next_char == EOF) // idk, seems like does nothing when EOF
            {
                next_char = getc(source_file);
                if (next_char == ']' || next_char == EOF)
                {
                    state = START;
                }
            }
            break;
        //---------------------------------------------------------------//
        case (STRING):

            if ((next_char >= 32)&&(next_char != '\\')&&(next_char != '"'))
            {
                if (!dyn_string_add_char(string, next_char))
                {
                    return free_memory(ERROR_INTERN, string);
                }
            }
            else if (next_char == '\\')
            {
                 
                state = ESCAPE;
            }
            else if (next_char == '"')
            {
                token->type = STRING;
                

                if (!dyn_string_copy(string, &token->value.string))
                {
                    return free_memory(ERROR_INTERN, string);
                }
                
                return free_memory(ERROR_OK, string);
            }
            if (next_char < 32)
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, string);
            }
            break;
            //---------------------------------------------------------------//

        case ESCAPE:
            if (next_char == '\\')
            {
                dyn_string_add_char(string, next_char);
                dyn_string_add_char(string, '0');
                dyn_string_add_char(string, '9');
                dyn_string_add_char(string, '2');
                state = STRING;
            }
            else if (next_char == 'n')
            {
                dyn_string_add_char(string, '\\');
                dyn_string_add_char(string, '0');
                dyn_string_add_char(string, '1');
                dyn_string_add_char(string, '0');
                state = STRING;
            }
            else if (next_char == 't')
            {
                dyn_string_add_char(string, '\\');
                dyn_string_add_char(string, '0');
                dyn_string_add_char(string, '0');
                dyn_string_add_char(string, '9');
                state = STRING;
            }
            else if (next_char == '"')
            {
                dyn_string_add_char(string, '\\');
                dyn_string_add_char(string, '"');
                state = STRING;
            }
            else if (isdigit(next_char))
            {
                switch (esc_cnt)

                {
                case 0:
                    esc_val += (next_char - 48) * 100;
                    esc_cnt++;
                    break;

                case 1:
                    esc_val += (next_char - 48) * 10;
                    esc_cnt++;

                    break;

                case 2:
                    esc_val += (next_char - 48);
                    if ((0 < esc_val) || (esc_val < 255))
                    {
                        dyn_string_add_char(string, '\\');
                        dyn_string_add_char(string, (esc_val / 100 + 48));
                        esc_val -= (esc_val / 100) * 100;
                        dyn_string_add_char(string, (esc_val / 10 + 48));
                        esc_val -= (esc_val / 10) * 10;
                        dyn_string_add_char(string, esc_val + 48);

                        state = STRING;
                    }
                    break;

                default:
                    return free_memory(ERROR_LEXICAL_ANALISYS, string);
                }
            }

            else
                return free_memory(ERROR_LEXICAL_ANALISYS, string);
            break;

        } //switch end
    }     //while end
} //get_token end
