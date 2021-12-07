/**
 * @file parser.c
 * @author xpyczl00
 * @brief 
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* POZNAMKY
vsade pred keywordy overit ze su keywordy lebo koren nevie ani šunku nakrájať
OSETRENIE ZIADNEHO VYSTUPU
VSADE OVERENIA EOFOV
BUDE TREBA SKONROLOVAT SYMTABLE
HLOZIMU POSIELAT FUNKCIE S PARAMETRAMI
MAREKOVI BUDEM MUSIET POSIELAT LOKALNU TABULKU NA KONTROLU PREMENNYCH 

CHYBNE SPRAVY MOZU BYT ROZLISNE

VSADE OVERIT CI TOKEN JE OK
*/

#include "parser.h"
#include "code_gen.h"
#include "expressions.h"


int parse()
{
    
    //Global symtable for functions
    ht_table_t tmp_global;
    global = &tmp_global;
    ht_init(global);

    //Local symtable for variables
    ht_table_t tmp_local;
    local = &tmp_local;
    ht_init(local);
    


    return Program();
}

int Program()   //Program -> <Prolog> <Deklaracie or Definicie> EOF
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

    //header () zavola hlozimu
    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;


    return Deklaracie_or_Definicie();
}

int Deklaracie_or_Definicie()
{
    int output;
    
    //IF EOF-> KONIEC
    if (actToken.type == STATE_EOF){
        fprintf(stdout,"KONIEC PROGRAMU");
        return ERROR_OK;
    }
    
    //IF GLOBAL -> DEKLARACIA_FUNKCIE
    else if (actToken.value.keyword == KEYWORD_GLOBAL)//KOREN
    {
        output = Deklaracie_Funkcie();
    }
    //IF LOCAL -> DEFINICIA_FUNKCIE
    else if (actToken.value.keyword == KEYWORD_FUNCTION)//KOREN
    {
        output = Definicia_Funkcie();  
    }
    //ELSE ERROR
    else
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    if (output != ERROR_OK)
        return output;

    //REKURZIVNE VOLANIE
    output = Deklaracie_or_Definicie();
    return output;
   
}

int Deklaracie_Funkcie()    //  DEKLARACIE_FUNKCIE -> GLOBAL ID : FUNCTION (<TYPY>):<TYPY>
{

    int output;
    output = get_token(&actToken);
    //ID
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != ID)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    else
    {
        //SAVING FUNCTION NAME
        func_name = actToken.value.string.string;

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
        //TEMPORARY LL
            func_val_t *Types = malloc(sizeof(struct func_val));
        //INSERTING FUNCTION INTO GLOBAL
            ht_item_t *current = ht_insert(global, func_name);
            current->inval = Types;
        //REWRITING LL IN THE TYPES
            output = Typy(Types);

        }

        if (output != ERROR_OK)
            return output;

        // ")"
        if (actToken.type != RIGHT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;

        //TODO FUNKCIA MOZE BYT BEZ VYSTUPU

        output = get_token(&actToken);
        // ":"
        if (output != ERROR_OK)
            return ERROR_LEXICAL_ANALISYS;

        if (actToken.type != COLON)
            return ERROR_SYNTAX_ANALYSIS;

        //MOZNO BY SOM MAL DAT FREE
        //<TYPY>
        func_val_t *outTypes = malloc(sizeof(struct func_val));
        ht_item_t *current2 = ht_search(global, func_name);
        current2->outval = outTypes;
        output = get_token(&actToken);
        output = Typy(outTypes);

        /* Hloziho_func(ID_name, ); */
        return output;
    }
}

int Definicia_Funkcie() //DEFINICIA_FUNKCIE -> <HLAVICKA> <TELO>
{
    int output;
    output = Hlavicka_Funkcie();
    if (output != ERROR_OK)
        return output;

    output = Telo_Funkcie();
    if (output != ERROR_OK)
        return output;

    return ERROR_OK;
}

int Hlavicka_Funkcie()// HLAVICKA-> FUNCTION ID (<ZOZNAM_PARAM>):<TYPY>
{
    //FUNCTION
    if (actToken.type != KEYWORD)
        return ERROR_SYNTAX_ANALYSIS;

    if (actToken.value.keyword != KEYWORD_FUNCTION)
        return ERROR_SYNTAX_ANALYSIS;

    int output;
    output = get_token(&actToken);
    //ID
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != ID)
        return ERROR_SYNTAX_ANALYSIS;

    //TMP LL FOR INPUT VALS
    func_name = actToken.value.string.string;
    current_item = ht_search(global, func_name);
    current_LL = current_item->inval->next;

    //CHECKING IF ID IS IN GLOBAL SYMT
    if (current_item == NULL) 
        return ERROR_SEMANTIC;

    output = get_token(&actToken);
    //"("
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != LEFT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    // ")" or <ZOZNAM_PARAM>
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != RIGHT_PARENTHESIS)
    {
        output = Zoznam_parametrov();
    }

    if (output != ERROR_OK)
        return output;
    //")"
    if (actToken.type != RIGHT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);

    //: AK NEMA ZIADNE OUTVALS PRESKOCI 
    if (actToken.type != COLON)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    //<TYPY>
    current_LL = current_item->outval->next;
    output = COMPARE_Typy();

    return output;
}

int COMPARE_Typy()
{
    int output;
    output = COMPARE_Typ();

    if (output != ERROR_OK)
        return output;

    output = get_token(&actToken);
    //IF , MEANS THERE IS NEXT
    if (actToken.type == COMMA)
    {
        //get token
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

    //IF THE TYPE IS CORRECT IN FUNCTION
    if (actToken.value.keyword == current_LL->typp)
    {
        //ADDS VARIABLE NAME TO TYPE
        current_LL->var_name = var_name;
        //INSERTS VARIABLE TO LOCAL TABLE
        ht_item_t *insert = ht_insert(local, var_name);
        //SETS ITS TYPE IN LOCAL TABLE
        insert->var_type = actToken.value.keyword;
        fprintf(stdout, "\n %d == %d ", actToken.value.keyword, current_LL->typp);
        //MOVES IN LL
        current_LL = current_LL->next;
        return ERROR_OK;
    }
    else
    {
        fprintf(stdout, "\n %d != %d ", actToken.value.keyword, current_LL->typp);
        return ERROR_SEMANTIC;
    }
    return ERROR_OK;
}

int Typy(func_val_t *Types)
{
    int output;
    output = Typ(Types);

    if (output != ERROR_OK)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    //IF COMA, THERE IS MORE
    if (actToken.type == COMMA)
    {

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;

        return Typy(Types);
    }
    else
        return ERROR_OK;
    return 69;
}

int Typ(func_val_t *Types)
{
    //CHECKS IF ITS CORRECT TYPE
    func_val_t *p, *tmp;
    switch (actToken.value.keyword)
    {
    case KEYWORD_INTEGER:
    case KEYWORD_STRING:
    case KEYWORD_NUMBER:
        //PUTS TYPES IN ORDER INTO LL
        tmp = malloc(sizeof(struct func_val));
        tmp->typp = actToken.value.keyword;

        if (Types == NULL)
        {
            Types = tmp;
        }
        else
        {
            p = Types;
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = tmp;
        }
        return ERROR_OK;
    default:
        break;
    }
    return ERROR_SYNTAX_ANALYSIS;
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

    //ID
    if (actToken.type != ID)
        return ERROR_SYNTAX_ANALYSIS;
    //SAVING VARIABLE NAME
    var_name = actToken.value.string.string;

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

int Telo_Funkcie()  //TELO_FUNKCIE -> <SEKVENCIA_PRIKAZOV> END
{
    int output;
    if(actToken.type = KEYWORD && actToken.value.keyword == KEYWORD_END)
    {
       gen_function_end();
       return ERROR_OK;
        //KONIEC FUNKCIE
    }  
    else if(actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_LOCAL)
    {

        /* VNORENA DEKLARACIA, UNDECLARE KED JE VONKU z ifu whileu funkcie */
        output = get_token(&actToken);
        if(output!=ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if(actToken.type != ID)
            return ERROR_SYNTAX_ANALYSIS;
        var_name = actToken.value.string.string;
/*         ht_insert(local,var_name);
        gen_var_def(var_name); */
        
        output = get_token(&actToken);
        if(output!=ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if(actToken.type != COLON)
            return ERROR_SYNTAX_ANALYSIS;
        
        output = get_token(&actToken);
        if(output!=ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if(actToken.type != KEYWORD)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER ||  actToken.value.keyword !=KEYWORD_NUMBER )
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
        ht_insert(local,var_name);
        gen_var_def(var_name);

        //MUSIM PRIDAT DO LOCAL tabulky

        //ulozit premennu niekam
        expression_analysis(&actToken);
        Symbol var;
        var->id = var_name;
        //type neviem ale musim nejak zistit
        
        gen_var_setval(var);
        //expression pre marecka
        

    }
    else if (actToken.type == ID)
    {
        //ID UKLADAM DO LISTU 
        //POSTUPNE OVERUJEM CI SU V LOKALNEJ
        //expressions volanie prvej var, if ciarka expressions volanie dalsiej , atd atd
        //VIAC ID = VIAC VYRAZOV
    }
    else if (actToken.type == BUILTIN)
    {
        //BUILTIN FUNKCIE, LEN ZAVOLAM HLOZIMU 
    }
    else if(actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_IF)
    {
        //expressions pre mareka,
        //if
        //telo funkcie
        
    }
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_WHILE)
    {
        //WHILE
        //linked list 
        //vzdy ked bude while zavola podmienku marekovi atd
    }
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_RETURN)
    {
        //netusim
        //RETURN
    }
    else //ak nieco ine
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    
    //toto je asi zbytocne 
    if(output!=ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
    return Telo_Funkcie(); 

}