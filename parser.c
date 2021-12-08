/**
 *  Project - Compiler for IFJ21
 *
 * @brief Parser for IFJ21 - syntax analysis
 *
 * @author Lukasz Pycz <xpyczl00>
 * @file parser.c
 *
 **/

#include "parser.h"
#include "code_gen.h"
#include "expressions.h"

int parse()
{

    // Global symtable for functions
    ht_table_t tmp_global;
    global = &tmp_global;
    ht_init(global);

    // Local symtable for variables
    ht_table_t tmp_local;
    local = &tmp_local;
    ht_init(local);

    return Program();
}

int Program() // Program -> <Prolog> <Deklaracie or Definicie> EOF
{

    int output;
    output = get_token(&actToken);

    //<PROLOG> REQUIRE
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != KEYWORD)
        return ERROR_SYNTAX_ANALYSIS;

    if (actToken.value.keyword != KEYWORD_REQUIRE)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);

    //<PROLOG> IFJ21
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != STRING)
        return ERROR_SYNTAX_ANALYSIS;

    if (dyn_string_compare(actToken.value.string, "ifj21") != 0)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    gen_header();
    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    return Deklaracie_or_Definicie();
}

int Deklaracie_or_Definicie()
{
    int output;

    // IF EOF-> KONIEC
    if (actToken.type == STATE_EOF)
    {

        return ERROR_OK;
    }

    // IF GLOBAL -> DEKLARACIA_FUNKCIE
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_GLOBAL)
    {

        output = Deklaracie_Funkcie();
    }
    // IF FUNCTION -> DEFINICIA_FUNKCIE
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_FUNCTION)
    {
        output = Definicia_Funkcie();
    }
    // ELSE ERROR
    else
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    if (output != ERROR_OK)
        return output;

    // REKURZIVNE VOLANIE
    output = Deklaracie_or_Definicie();
    return output;
}

int Deklaracie_Funkcie() //  DEKLARACIE_FUNKCIE -> GLOBAL ID : FUNCTION (<TYPY>):<TYPY>
{

    int output;
    ht_item_t *current;
    output = get_token(&actToken);
    // ID
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != ID)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    // SAVING FUNCTION NAME
    // char *function_name = actToken.value.string.string;
    char *func_name = actToken.value.string.string;
    output = get_token(&actToken);
    // ":"
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != COLON)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    // FUNCTION
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != KEYWORD)
        return ERROR_SYNTAX_ANALYSIS;

    if (actToken.value.keyword != KEYWORD_FUNCTION)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    // "("
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != LEFT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    // ")" OR <TYPY>

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;
    // <TYPY>
    if (actToken.type != RIGHT_PARENTHESIS)
    {
        // TEMPORARY LL

        func_val_t *INPUT_TYPES = malloc(sizeof(func_val_t));
        // INSERTING FUNCTION INTO GLOBAL
        current = ht_search(global, func_name);

        if (current != NULL)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
        current = ht_insert(global, func_name);
        current->is_defined = false;

        // REWRITING LL IN THE TYPES
        Token_type typ = Typ();

        INPUT_TYPES->type = typ;
        INPUT_TYPES->next = NULL;
        current->inval = INPUT_TYPES;
        get_token(&actToken);
        // ak je token.type valid

        while (actToken.type == COMMA)
        {
            func_val_t *tmp = malloc(sizeof(func_val_t));
            output = get_token(&actToken);
            if (output != ERROR_OK)
                return output;

            tmp->type = Typ();
            INPUT_TYPES->next = tmp;

            INPUT_TYPES = INPUT_TYPES->next;
            get_token(&actToken);
        }
    }

    if (output != ERROR_OK)
        return output;

    // ")"
    if (actToken.type != RIGHT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    // ":"
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;
    // IF NO OUTPUT
    if (actToken.type != COLON)
        return ERROR_OK;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;
    //<TYPY>
    func_val_t *OUTPUT_TYPES = malloc(sizeof(func_val_t));
    output = get_token(&actToken);

    Token_type typ = Typ();
    if (output != ERROR_OK)
        return output;

    OUTPUT_TYPES->type = typ;
    OUTPUT_TYPES->next = NULL;
    current->outval = OUTPUT_TYPES;
    get_token(&actToken);

    while (actToken.type == COMMA)
    {
        func_val_t *tmp = malloc(sizeof(func_val_t));
        output = get_token(&actToken);
        if (output != ERROR_OK)
            return output;

        tmp->type = Typ();

        OUTPUT_TYPES->next = tmp;

        OUTPUT_TYPES = OUTPUT_TYPES->next;
        get_token(&actToken);
    }

    /* output = Typy(OUTPUT_TYPES); */
    if (output != ERROR_OK)
        return output;

    return output;
}

int Definicia_Funkcie() // DEFINICIA_FUNKCIE -> <HLAVICKA> <TELO>
{
    int output;
    output = Hlavicka_Funkcie();
    if (output != ERROR_OK)
        return output;

    output = Telo_Funkcie();
    return ERROR_OK;
    if (output != ERROR_OK)
        return output;

    return ERROR_OK;
}

int Hlavicka_Funkcie() // HLAVICKA-> FUNCTION ID (<ZOZNAM_PARAM>):<TYPY>
{

    // FUNCTION
    if (actToken.type != KEYWORD)
        return ERROR_SYNTAX_ANALYSIS;

    if (actToken.value.keyword != KEYWORD_FUNCTION)
        return ERROR_SYNTAX_ANALYSIS;

    int output;
    output = get_token(&actToken);
    // ID
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != ID)
        return ERROR_SYNTAX_ANALYSIS;

    // VAR NAME

    char *func_name = actToken.value.string.string;
    ht_item_t *item = ht_search(global, func_name);
    if (item == NULL)
    {
        item = ht_insert(global, func_name);
    }

    /* strcpy(*function_name,*func_name); */

    if (strcmp(func_name, "main") == 0)
    {
        get_token(&actToken);
        get_token(&actToken);
        get_token(&actToken);
        gen_main();

        return ERROR_OK;
    }

    current_item = ht_search(global, func_name);

    // CHECKING IF ID IS IN GLOBAL SYMT
    if (current_item == NULL)
    {
        current_item = ht_insert(global, func_name); // move to end, create new item instead
        current_item->is_defined = true;
    }
    else
    {
        if (current_item->is_defined == true)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    output = get_token(&actToken);
    //"("
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != LEFT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;
    // ")" or <ZOZNAM_PARAM>
    if (actToken.type != RIGHT_PARENTHESIS)
    {
        if (actToken.type == STATE_EOF)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        current_LL = current_item->inval;
        output = Zoznam_parametrov();
        if (output != ERROR_OK)
            return output;
    }

    //")"
    if (actToken.type != RIGHT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);

    //: AK NEMA ZIADNE OUTVALS PRESKOCI DOLEZITE AK MA OUTVALS ROVNAKE
    if (actToken.type != COLON)
        return ERROR_OK;

    output = get_token(&actToken);
    //<TYPY>

    current_LL = current_item->outval;
    output = COMPARE_Typy();

    func_val_t *outs = current_item->outval;
    while (current_item->outval != NULL)
    {

        current_item->outval = current_item->outval->next;
    }

    while (current_item->inval != NULL)
    {
        current_item->inval = current_item->inval->next;
    }

    gen_function_start(current_item->key, *outs);

    return output;
}

int COMPARE_Typy()
{
    int output;
    output = COMPARE_Typ();
    if (output != ERROR_OK)
        return output;

    output = get_token(&actToken);
    // IF , MEANS THERE IS NEXT
    if (actToken.type == COMMA)
    {
        // get token
        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        return COMPARE_Typy();
    }
    else
        return ERROR_OK;
    return 69;
}

int COMPARE_Typ()
{
    Token_type current_type;
    switch (actToken.value.keyword)
    {
    case KEYWORD_NUMBER:
        current_type = NUMBER;
        break;
    case KEYWORD_STRING:
        current_type = STRING;
        break;
    case KEYWORD_INTEGER:
        current_type = INT;
        break;
    default:
        break;
    }
    // IF THE TYPE IS CORRECT IN FUNCTION
    if (current_type == current_LL->type)
    {

        // ADDS VARIABLE NAME TO TYPE

        var_name = actToken.value.string.string;

        // SETS ITS TYPE IN LOCAL TABLE
        current_LL->var_name = var_name;

        // MOVES IN LL
        current_LL = current_LL->next;
        return ERROR_OK;
    }
    else
    {

        return ERROR_SEMANTIC;
    }
    return ERROR_OK;
}

Token_type Typ()
{

    if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_INTEGER)
    {
        return INT;
    }
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_NUMBER)
    {
        return NUMBER;
    }
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_STRING)
    {
        return STRING;
    }
    else
    {

        return ERROR_SYNTAX_ANALYSIS;
    }
    return 69;
}

int Zoznam_parametrov()
{
    int output;
    output = Parameter();

    if (output != ERROR_OK)
        return output;

    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_SYNTAX_ANALYSIS;

    if (actToken.type == COMMA)
    {
        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;

        return Zoznam_parametrov();
    }
    else
        return ERROR_OK;
    return 69;
}

int Parameter()
{
    int output;

    // ID
    if (actToken.type != ID)
        return ERROR_SYNTAX_ANALYSIS;
    // SAVING VARIABLE NAME

    output = get_token(&actToken);
    // ":"
    if (actToken.type != COLON)
        return ERROR_SYNTAX_ANALYSIS;

    get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_SYNTAX_ANALYSIS;

    output = COMPARE_Typ();
    return output;
}

int Telo_Funkcie() // TELO_FUNKCIE -> <SEKVENCIA_PRIKAZOV> END
{
    int output;
    if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_END)
    {

        gen_function_end();
        return ERROR_OK;
        // KONIEC FUNKCIE
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_LOCAL)
    {
        /* VNORENA DEKLARACIA, UNDECLARE KED JE VONKU z ifu whileu funkcie */
        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != ID)
            return ERROR_SYNTAX_ANALYSIS;
        var_name = actToken.value.string.string;

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != COLON)
            return ERROR_SYNTAX_ANALYSIS;

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != KEYWORD)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.value.keyword != KEYWORD_STRING && actToken.value.keyword != KEYWORD_INTEGER && actToken.value.keyword != KEYWORD_NUMBER)
            return ERROR_SYNTAX_ANALYSIS;

        ht_item_t *element = ht_insert(local, var_name);
        element->var_type = actToken.value.keyword;

        switch (element->var_type)
        {
        case KEYWORD_INTEGER:
            element->var_type = INT;
            break;
        case KEYWORD_NUMBER:
            element->var_type = NUMBER;
            break;
        case KEYWORD_STRING:
            element->var_type = STRING;
            break;
        default:
            break;
        }

        gen_var_def(var_name);

        // MUSIM PRIDAT DO LOCAL tabulky
        Symbol var;
        var.id = var_name;
        // ulozit premennu niekam
        // token type

        if (actToken.type == ASSIGN)
        {

            get_token(&actToken);

            get_token(&actToken);

            output = expression_analysis(&actToken, local, &var, NULL);

            // skontrolovat typ s premennou
            if (element->var_type != var.result_type)
            {

                return ERROR_SEMANTIC;
            }

            gen_var_setval(var);
        }
        else
        {
            get_token(&actToken);
        }
    }

    else if (actToken.type == ID)
    {
        get_token(&actToken);
        get_token(&actToken);
        Symbol s;
        s.id = actToken.value.string.string;
        expression_analysis(&actToken, local, &s, NULL);
        gen_var_setval(s);
    }
    // ID UKLADAM DO LISTU
    // POSTUPNE OVERUJEM CI SU V LOKALNEJ
    // expressions volanie prvej var, if ciarka expressions volanie dalsiej , atd atd
    // VIAC ID = VIAC VYRAZOV

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_IF)
    {

        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;

        // var musi byt nula
        expression_analysis(&actToken, local, NULL, NULL);
        // ak je then inak chyba
        gen_if_start();
        output = get_token(&actToken);
        // overit token
        output = Telo_Funkcie_v_IF();
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        // if else
        gen_if_else();
        output = get_token(&actToken);
        // over token
        output = Telo_Funkcie_v_IF();
        return ERROR_OK;
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        // if END
        gen_if_end();
        return ERROR_OK;
        // problem treba vymazavat lokalne ifove deklaracie zo stromu
    }

    return Telo_Funkcie();
}
int Telo_Funkcie_v_IF() // TELO_FUNKCIE -> <SEKVENCIA_PRIKAZOV> END
{
    int output;
    if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_END)
    {
        gen_function_end();
        return ERROR_OK;
        // KONIEC FUNKCIE
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_LOCAL)
    {
        /* VNORENA DEKLARACIA, UNDECLARE KED JE VONKU z ifu whileu funkcie */
        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != ID)
            return ERROR_SYNTAX_ANALYSIS;
        var_name = actToken.value.string.string;

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != COLON)
            return ERROR_SYNTAX_ANALYSIS;

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != KEYWORD)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.value.keyword != KEYWORD_STRING && actToken.value.keyword != KEYWORD_INTEGER && actToken.value.keyword != KEYWORD_NUMBER)
            return ERROR_SYNTAX_ANALYSIS;

        ht_item_t *element = ht_insert(local, var_name);
        element->var_type = actToken.value.keyword;

        switch (element->var_type)
        {
        case KEYWORD_INTEGER:
            element->var_type = INT;
            break;
        case KEYWORD_NUMBER:
            element->var_type = NUMBER;
            break;
        case KEYWORD_STRING:
            element->var_type = STRING;
            break;
        default:
            break;
        }

        gen_var_def(var_name);

        // MUSIM PRIDAT DO LOCAL tabulky
        Symbol var;
        var.id = var_name;
        // ulozit premennu niekam
        // token type
        get_token(&actToken);
        get_token(&actToken);

        output = expression_analysis(&actToken, local, &var, NULL);
        // skontrolovat typ s premennou
        if (element->var_type != var.result_type)
        {

            return ERROR_SEMANTIC;
        }

        gen_var_setval(var);
    }

    else if (actToken.type == ID)
    {
        get_token(&actToken);
        get_token(&actToken);
        Symbol s;
        s.id = actToken.value.string.string;
        expression_analysis(&actToken, local, &s, NULL);
        gen_var_setval(s);
    }
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_IF)
    {

        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;

        // var musi byt nula
        expression_analysis(&actToken, local, NULL, NULL);
        gen_if_start();

        output = Telo_Funkcie();
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        gen_if_else();

        output = Telo_Funkcie();
        return ERROR_OK;
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        gen_if_end();
        return ERROR_OK;
    }
    return ERROR_OK;
}