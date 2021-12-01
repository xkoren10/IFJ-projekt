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
VSADE OVERENIA EOFOV
BUDE TREBA SKONROLOVAT SYMTABLE
HLOZIMU POSIELAT FUNKCIE S PARAMETRAMI
MAREKOVI BUDEM MUSIET POSIELAT LOKALNU TABULKU NA KONTROLU PREMENNYCH 
PREPISAT PARAMETRE V PARSER.H

GETTOKEN VRACIA BUD OK ALEBO ERROR
AK ERROR VRACIA ERROR LEXICAL ANALISYS
AK NIE 0

VSADE OVERIT CI TOKEN JE OK
*/

#include "parser.h"

//fukncia na dalsi token
int Next_token()
{
    int output;
    output = get_token(&actToken);
    if (output == ERROR)
        return ERROR_LEXICAL_ANALISYS;
    return 0;
}

//funkcia na skipnutie EOLu
int Is_Eol()
{
    int output = ERROR_OK;
    if (actToken.type == EOL)
    {
        output = get_token(&actToken);
        Is_EOL();
    }
    return output;
}

int parse()
{

    //inicializacia SYMTABLES

    ht_init(global);
    ht_init(local);

    int output;
    output = get_token(&actToken);
    

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;
        
    return Program();

}
int Program()
{
    int output;

    if (actToken.value.keyword != KEYWORD_REQUIRE)
        return ERROR_SYNTAX_ANALYSIS;
        
        output = get_token(&actToken);
        
        if (output != ERROR_OK)     //čo vracia token
            return ERROR_LEXICAL_ANALISYS;
        
        if (actToken.type == STATE_EOF)
            return ERROR_SYNTAX_ANALYSIS;

        if (!actToken.type != ID) 
            return ERROR_SYNTAX_ANALYSIS;

        //header () zavola hlozimu
    return Deklaracie_or_Definicie(); //malo by vratit nulu alebo error
}

int Deklaracie_or_Definicie()
{

    int output;
    output = get_token(&actToken);
    

    if (output != ERROR_OK)
    {
        return ERROR_LEXICAL_ANALISYS;
    }
    if (actToken.type == STATE_EOF)
        return ERROR_OK;

    else if (actToken.value.keyword == KEYWORD_GLOBAL)
    {
        output = Deklaracie_Funckie();
    }
    else if (actToken.value.keyword == KEYWORD_FUNCTION)
    {
        output = Definicia_Funkcie();
    }
    else
    {
        return ERROR_SYNTAX_ANALYSIS;
    }


    if (output != ERROR_OK)
        return output;
        
    Deklaracie_or_Definicie();

    //TOTO BY MALO BYT FAJN


}
int Deklaracia_Funkcie()
{

    //vsade overit eof?
    //ID, parametre nacitat do symtable a potom kontrolovat ci je/ neni vyskyt

    int output;
    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;
    
    

    //TODO
    if (actToken.type != ID)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    else
    {

        //ULOZIM SI PREMENNU A POTOM JU PRIDAM DO HT
        ID_name = actToken.value.string.string;

        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_LEXICAL_ANALISYS;

        if (actToken.type == STATE_EOF)
            return ERROR_SYNTAX_ANALYSIS;
        

        if (actToken.type != COLON)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_LEXICAL_ANALISYS;

        if (actToken.type == STATE_EOF)
            return ERROR_SYNTAX_ANALYSIS;

        if (actToken.value.keyword != KEYWORD_FUNCTION)
            return ERROR_SYNTAX_ANALYSIS;


        output = get_token(&actToken);
        
        if (output != ERROR_OK)
            return ERROR_LEXICAL_ANALISYS;

        if (actToken.type == STATE_EOF)
            return ERROR_SYNTAX_ANALYSIS;


        if (actToken.type != LEFT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;


        output = get_token(&actToken);


        if (output != ERROR_OK)
            return ERROR_LEXICAL_ANALISYS;

        if (actToken.type == STATE_EOF)
            return ERROR_SYNTAX_ANALYSIS;



        if (actToken.type != RIGHT_PARENTHESIS)
        {
            //Dočasny LL predam do typy
            func_val_t *Types = malloc(sizeof(struct func_val));
            Types->next = NULL;
           
            output = Typy(&Types); 
            
            //vrati mi LL na ktorom su typy v poradi
            
            //Novy element dam mu ID a priradil LL, staci aby insert vracial nemusim ani definovat
            ht_item_t *current = ht_insert(global, ID_name);
            current->inval = Types;

        }

        if (output != ERROR_OK)
            return output;
        
        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_LEXICAL_ANALISYS;

        if (actToken.type == STATE_EOF)
            return ERROR_SYNTAX_ANALYSIS;


        if (actToken.type != RIGHT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;


        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_LEXICAL_ANALISYS;

        if (actToken.type == STATE_EOF)
            return ERROR_SYNTAX_ANALYSIS;
        

        if (actToken.type != COLON)
            return ERROR_SYNTAX_ANALYSIS;

        //Dočasny LL predam do typy //MOZNO BY SOM MAL DAT FREE
        func_val_t *outTypes = malloc(sizeof(struct func_val));
        outTypes->next = NULL;
        
        //vystupne typy
        output = Typy(&outTypes);

        ht_item_t *current2 = ht_insert(global, ID_name);
        current2->outval = outTypes;


        //predam mu UDAJE NA FUKNCIU
        /* Hloziho_func(ID_name, ); */

        return output;
    }
}
int Definicia_Funckie()
{
    //EFOY
    int output;
    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;


    output = Hlavicka_Funkcie();
    if (output != ERROR_OK)
        return output;

    output = Telo_Funkcie();
    if (output != ERROR_OK)
        return output;

    return ERROR_OK;
}

int Hlavicka_funkcie()
{
    //eofy
    int output;
    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)         //TEORETICKY EOFY NETREBA
        return ERROR_SYNTAX_ANALYSIS;
    

    if (actToken.type != KEYWORD_FUNCTION)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;

    if (actToken.type != ID)
        return ERROR_SYNTAX_ANALYSIS;

    //DOCASNA PREMENNA A HLADANIE CI JE V TABULKE
    func_name = actToken.value.string.string;
    if (ht_search(global, func_name) != 0)
        return ERROR_SEMANTIC;

    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;    

    if (actToken.type != LEFT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;    

    if (actToken.type != LEFT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;    
    

    if (actToken.type != RIGHT_PARENTHESIS)
    {
        //POSIELAM MENO FUNKCIE NECH SI VIEM OVERIT TYPY
        //nacitam si prvy typ do tokenu
        current_item = ht_search(global,func_name);
        //ci tam je nieco ale to teoreticky overi OVERtyp;
        current_LL = current_item->inval;
        output = Zoznam_parametrov();
    }

    if (output != ERROR_OK)
        return output;

    //tento asi preskoci jeden token

    output = get_token(&actToken);
    

    if (actToken.type != RIGHT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    

    if (actToken.type != COLON)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);
    

    output = COMPARE_Typy();

    return output;
}


int COMPARE_Typy()
{ 
    //VIEM ZE NEBUDE PRAZDNE
    int output;
    output = COMPARE_Typ();

    if (output != ERROR_OK)
        return output;

    output = get_token(&actToken);


    if (actToken.type == COMMA)
    {
        COMPARE_Typy();
    }
    else
        return ERROR_OK;
}
int COMPARE_typ()
{
    int output;
    output = get_token(&actToken);
    //overit asi

    if (current_LL->typp == actToken.type)
    {
        current_LL->var_name = var_name;
        ht_item_t* tmp = ht_insert(local,var_name);
        tmp->var_type = actToken.type;
        current_LL = current_LL->next;
        return ERROR_OK;
    }
    else{
        return ERROR_SYNTAX_ANALYSIS;
    }
 }



int Typy(func_val_t *Types)
{ 
    //VIEM ZE NEBUDE PRAZDNE
    int output;
    output = Typ(&Types);

    if (output != ERROR_OK)
        return output;

    output = get_token(&actToken);


    if (actToken.type == COMMA)
    {
        Typy(&Types);
    }
    else
        return ERROR_OK;
}



int Typ(func_val_t *Types)
{
    int output;
    output = get_token(&actToken);

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    //EOF NETREBA OSETROVAT

    switch (actToken.type)
    {
    case KEYWORD_INTEGER:
    case KEYWORD_STRING:
    case KEYWORD_NUMBER:
        
        func_val_t *p, *tmp;
        tmp = malloc(sizeof(struct func_val));
        tmp->typp = actToken.type;

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

        //NASTAVIM FUNC_VAL.PAR_TYPE = ACTTOKEN.TYPE
        return ERROR_OK;
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
    

    if (actToken.type == COMMA)
    {
        Zoznam_parametrov();
    }
    else
        return ERROR_OK;
}

int Parameter()
{
    int output;
    output = get_token(&actToken);
    

    //pridat meno do lokalnej
    if (actToken.type != ID)
        return ERROR_SYNTAX_ANALYSIS;

    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;
    
    //meno premennej
    var_name = actToken.value.string.string;
    //asi nacitat string atd actToken.value.string

    output = get_token(&actToken);
    

    if (actToken.type != COLON)
        return ERROR_SYNTAX_ANALYSIS;

    //No, takže takto : do funkcie sa pošle nazov premennej ktory sa pripiše
    //a taktiež okrem ineho sa pošle aj typ ktory ziskame z linked listu ktorym
    //budem musiet postupne prechadzat ale ten typ budem musiet poslat este skor, a overi sa ci je typ spravny
    //a taktiez ci je typ uz spomenuty predtym

    output = COMPARE_Typ();
    return output;
}
