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
vsade pred keywordy overit ze su keywordy lebo koren nevie ani šunku nakrájať -- excuse me
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
    printf("parse\n");
    return Program();
}

int Program() //Program -> <Prolog> <Deklaracie or Definicie> EOF
{

    int output;
    output = get_token(&actToken);
    printf("program\n");

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
    printf("dek/def\n");

    //IF EOF-> KONIEC
    if (actToken.type == STATE_EOF)
    {
        fprintf(stdout, "KONIEC PROGRAMU"); // TO DEL
        return ERROR_OK;
    }

    //IF GLOBAL -> DEKLARACIA_FUNKCIE
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_GLOBAL) //KOREN
    {
        printf("to dek\n");
        output = Deklaracie_Funkcie();
    }
    //IF FUNCTION -> DEFINICIA_FUNKCIE
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_FUNCTION) //KOREN
    {
        printf("to def\n");
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

int Deklaracie_Funkcie() //  DEKLARACIE_FUNKCIE -> GLOBAL ID : FUNCTION (<TYPY>):<TYPY>
{
    printf("dek\n");
    int output;
    ht_item_t *current;
    output = get_token(&actToken);
    //ID
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != ID)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
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
        printf("preLL\n");
        func_val_t INPUT_TYPES;
        //INSERTING FUNCTION INTO GLOBAL
        current = ht_search(global, func_name);
        if(current != NULL){
            return ERROR_SYNTAX_ANALYSIS;
        }
        current = ht_insert(global, func_name);
        current->is_defined = false;

        //REWRITING LL IN THE TYPES
        output = Typy(&INPUT_TYPES);
        if (output != ERROR_OK)
            return output;
        current->inval = &INPUT_TYPES;
    }

    if (output != ERROR_OK)
        return output;

    // ")"
    if (actToken.type != RIGHT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    //TODO FUNKCIA MOZE BYT BEZ VYSTUPU
    printf("preCOLON\n");
    output = get_token(&actToken);
    // ":"
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;
    //IF NO OUTPUT
    if (actToken.type != COLON)
        return ERROR_OK;

    if (actToken.type == STATE_EOF)
        return ERROR_SYNTAX_ANALYSIS;
    //<TYPY>
    func_val_t *OUTPUT_TYPES = NULL;

    output = get_token(&actToken);
    if (output != ERROR_OK)
        return output;
    output = Typy(OUTPUT_TYPES);
    if (output != ERROR_OK)
        return output;
    current->outval = OUTPUT_TYPES;
printf("EEEEE%d", OUTPUT_TYPES);
   /*  while (OUTPUT_TYPES!=NULL){
        
        OUTPUT_TYPES = OUTPUT_TYPES->next;
    } */

    return output;
}

int Definicia_Funkcie() //DEFINICIA_FUNKCIE -> <HLAVICKA> <TELO>
{
    int output;
    output = Hlavicka_Funkcie();
    if (output != ERROR_OK)
        return output;
    fprintf(stdout, "sem som sa dostal");
    output = Telo_Funkcie();
    return ERROR_OK;
    if (output != ERROR_OK)
        return output;

    return ERROR_OK;
}

int Hlavicka_Funkcie() // HLAVICKA-> FUNCTION ID (<ZOZNAM_PARAM>):<TYPY>
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
    

    //CHECKING IF ID IS IN GLOBAL SYMT
    if (current_item == NULL){
        current_item = ht_insert(global, func_name);  // move to end, create new item instead
        current_item->is_defined = true;
    }
    else{
        if(current_item->is_defined == true){
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
    // ")" or <ZOZNAM_PARAM>
    if (output != ERROR_OK)
        return ERROR_LEXICAL_ANALISYS;

    if (actToken.type != RIGHT_PARENTHESIS)
    {
        if (actToken.type == STATE_EOF)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
        output = Zoznam_parametrov();
        if (output != ERROR_OK)
            return output;
    }

    //")"
    if (actToken.type != RIGHT_PARENTHESIS)
        return ERROR_SYNTAX_ANALYSIS;

    output = get_token(&actToken);

    //: AK NEMA ZIADNE OUTVALS PRESKOCI
    if (actToken.type != COLON)
        return ERROR_SYNTAX_ANALYSIS;
    printf("helpme");
    output = get_token(&actToken);
    //<TYPY>
    current_LL = current_item->outval->next;
    func_val_t *invals = current_item->inval->next;
    output = COMPARE_Typy();
    return ERROR_OK;
    gen_function_start(func_name, *current_LL);
    return output;
}

int COMPARE_Typy()
{
    int output;
    output = COMPARE_Typ();
    printf("som za compare");
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
    if (actToken.value.keyword == current_LL->type)
    {
        //ADDS VARIABLE NAME TO TYPE
        current_LL->var_name = var_name;
        //INSERTS VARIABLE TO LOCAL TABLE
        ht_item_t *insert = ht_insert(local, var_name);
        //SETS ITS TYPE IN LOCAL TABLE
        insert->var_type = actToken.value.keyword;

        fprintf(stdout, "\n %d == %d ", actToken.value.keyword, current_LL->type);

        //MOVES IN LL
        current_LL = current_LL->next;
        return ERROR_OK;
    }
    else
    {
        printf("kokotina");
        printf("TU JE NEZHODA");
        fprintf(stdout, "\n %d != %d ", actToken.value.keyword, current_LL->type);
        return ERROR_SEMANTIC;
    }
    return ERROR_OK;
}

int Typy(func_val_t *Types)
{
    int output;
    output = Typ(Types);
    printf("Typ After> %d\n", Types);

    if (output != ERROR_OK){
        printf("dojebalo sa\n");return output;
    }

    output = get_token(&actToken);
    if (output != ERROR_OK)
        return output;
    //IF COMA, THERE IS MORE
    if (actToken.type == COMMA)
    {

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return output;

        return Typy(Types);
    }
    else
        return ERROR_OK;
    return 69;
}

int Typ(func_val_t *Types)
{
    func_val_t *new = (func_val_t *)malloc(sizeof(func_val_t));
    if (new == NULL){
        return ERROR_INTERN;
    }
        
    func_val_t *last = Types;
    printf("new> %d Typ> %d\n", new, Types);
    if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_INTEGER)
    {
        new->type = INT;
    }
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_NUMBER)
    {
        new->type = NUMBER;
    }
    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_STRING)
    {
        new->type = STRING;
    }
    else{
        printf("TYP err\n");
        return ERROR_SYNTAX_ANALYSIS;
    }
    new->next = NULL;

    if (Types == NULL)
    {
        printf("prvy\n");
        Types = new;
        printf("new> %d Typ> %d\n", new, Types);
        return ERROR_OK;
    }
    while (last->next != NULL)
    {
        printf("neprvy\n");
        last = last->next;
    }
    last->next = new;
    return ERROR_OK;
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

int Telo_Funkcie() //TELO_FUNKCIE -> <SEKVENCIA_PRIKAZOV> END
{
    fprintf(stdout, "sem som sa dostal");
    int output;
    if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_END)
    {
        gen_function_end();
        printf("koniec");
        return ERROR_OK;
        //KONIEC FUNKCIE
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_LOCAL)
    {
        fprintf(stdout, "LOCAL");
        /* VNORENA DEKLARACIA, UNDECLARE KED JE VONKU z ifu whileu funkcie */
        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != ID)
            return ERROR_SYNTAX_ANALYSIS;
        var_name = actToken.value.string.string;
        /*         ht_insert(local,var_name);
        gen_var_def(var_name); */
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

        element->var_type = actToken.type;

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

        //MUSIM PRIDAT DO LOCAL tabulky
        Symbol var;
        var.id = var_name;
        //ulozit premennu niekam
        //token type
        
            output = expression_analysis(&actToken, local, &var, NULL);
        fprintf(stdout, "%d", output); //null ak if
        fprintf(stdout, "%d", var.result_type);
        //skontrolovat typ s premennou
        if (element->var_type != var.result_type)
        {
            fprintf(stdout, "%d != %d", element->var_type, var.result_type);
            printf("PRED DEFINICIOU");
            return ERROR_SEMANTIC;
        }

        gen_var_setval(var);
        fprintf(stdout, "koniec locali");
    }

    /*     else if (actToken.type == ID)
    {
        id_list_t *id_list = malloc(sizeof(id_list_t));
        id_list_t *head = malloc(sizeof(id_list_t));
        head = id_list;

        while (actToken.type != ASSIGN)
        {

            id_list->id = actToken.value.string.string;
            if (ht_search(local, id_list->id) == NULL)
                return ERROR_SEMANTIC;
            id_list = id_list->next;
            output = get_token(&actToken);
            if (output != ERROR_OK)
                return ERROR_SYNTAX_ANALYSIS;
            if (actToken.type != COMMA && actToken.type != ASSIGN)
                return ERROR_SYNTAX_ANALYSIS;
        }
        while (head != NULL)
        {
            output = get_token(&actToken);
            if (output != ERROR_OK)
                return ERROR_SYNTAX_ANALYSIS;

            Symbol var;
            var.id = head->id;
            ht_item_t *searched = ht_search(local, head->id);
            var. = searched->var_type;
            //ulozit premennu niekam
            //token type
            expression_analysis(&actToken, local, &var, NULL); //null ak if

            //skontrolovat typ s premennou
            if (searched->var_type != var.value_type) //result type
                return ERROR_SEMANTIC;

            gen_var_setval(var);
            head = head->next;
        }

        //ID UKLADAM DO LISTU
        //POSTUPNE OVERUJEM CI SU V LOKALNEJ
        //expressions volanie prvej var, if ciarka expressions volanie dalsiej , atd atd
        //VIAC ID = VIAC VYRAZOV
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_IF)
    {
        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER || actToken.value.keyword != KEYWORD_NUMBER)
            return ERROR_SYNTAX_ANALYSIS;
        //var musi byt nula
        expression_analysis(&actToken, local, NULL);
        gen_if_start(co je type?);

        output = Telo_Funkcie();
        .if (output != ERROR_OK) return ERROR_SYNTAX_ANALYSIS;
        gen_if_else();

        output = Telo_Funkcie();
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        gen_if_end();
        //problem treba vymazavat lokalne ifove deklaracie zo stromu
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_WHILE)
    {
        token_list_t tmp,head = Tok;
        while (//actToken.type!=KEYWORD && actToken.value.keyword != KEYWORD_END)
        {
            Tok.current = actToken;
            if (actToken.value.keyword == KEYWORD_LOCAL && actToken.type == KEYWORD)
            {
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
                if (actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER || actToken.value.keyword != KEYWORD_NUMBER)
                    return ERROR_SYNTAX_ANALYSIS;

                ht_item_t *element = ht_insert(local, var_name);
                element->var_type = actToken.value.keyword;

                gen_var_def(var_name);

                Symbol var;
                var.id = var_name;

                expression_analysis(&actToken, local, &var); //null ak if

                //skontrolovat typ s premennou
                if (element->var_type != var.value_type)
                    return ERROR_SEMANTIC;

                gen_var_setval(var);
                Tok.current = actToken;
            }
            tmp = malloc(sizeof(token_list_t));
            tmp.current = actToken;
            Tok.next = tmp;
            Tok = Tok.next;
           
        }
    Tok = head;
    if(Tok.current.type!=KEYWORD || Tok.current.value.keyword!= KEYWORD_WHILE)
        return ERROR_SYNTAX_ANALYSIS;
    gen_loop_start();
    Tok=Tok.next;
    if(actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER || actToken.value.keyword != KEYWORD_NUMBER)
        return ERROR_SYNTAX_ANALYSIS;
    expression_analysis(Tok,local,NULL);
    Telo_Funkcie_vo_while();
    gen_loop_end();
    
        //WHILE
        //linked list
        //vzdy ked bude while zavola podmienku marekovi atd
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_RETURN) //TODO
    {
        //HLOZI MUSI NIECO VYMYSLIET
    }

    else if (actToken.type == BUILT_IN_READI) //ak nieco ine
    {
        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != LEFT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != RIGHT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;
    }
    else if (actToken.type == BUILT_IN_READN) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_READS) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_CHR) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_ORD) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_SUBSTR) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_WRITE) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_TOINTEGER) //ak nieco ine
    {
    }

    //toto je asi zbytocne
    if (output != ERROR_OK)
        return ERROR_SYNTAX_ANALYSIS;
 */
    //nejaky error ok
    return Telo_Funkcie();
}

/* int Telo_Funkcie_vo_while()
{
    //namiesto TOKEN iterujem TOK.current
        int output;
    if (actToken.type = KEYWORD && actToken.value.keyword == KEYWORD_END)
    {
        gen_function_end();
        return ERROR_OK;
        //KONIEC FUNKCIE
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_LOCAL)
    {

        //VNORENA DEKLARACIA, UNDECLARE KED JE VONKU z ifu whileu funkcie
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
        if (actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER || actToken.value.keyword != KEYWORD_NUMBER)
            return ERROR_SYNTAX_ANALYSIS;

        ht_item_t *element = ht_insert(local, var_name);
        element->var_type = actToken.value.keyword;

        gen_var_def(var_name);

        //MUSIM PRIDAT DO LOCAL tabulky
        Symbol var;
        var.id = var_name;
        //ulozit premennu niekam
        //token type
        expression_analysis(&actToken, local, &var); //null ak if

        //skontrolovat typ s premennou
        if (element->var_type != var.value_type)
            return ERROR_SEMANTIC;

        gen_var_setval(var);
    }

    else if (actToken.type == ID)
    {
        id_list_t *id_list = malloc(sizeof(id_list_t));
        id_list_t *head = malloc(sizeof(id_list_t));
        head = id_list;

        while (actToken.type != ASSIGN)
        {

            id_list->id = actToken.value.string.string;
            if (ht_search(local, id_list->id) == NULL)
                return ERROR_SEMANTIC;
            id_list = id_list->next;
            output = get_token(&actToken);
            if (output != ERROR_OK)
                return ERROR_SYNTAX_ANALYSIS;
            if (actToken.type != COMMA && actToken.type != ASSIGN)
                return ERROR_SYNTAX_ANALYSIS;
        }
        while (head != NULL)
        {
            output = get_token(&actToken);
            if (output != ERROR_OK)
                return ERROR_SYNTAX_ANALYSIS;

            Symbol var;
            var.id = head->id;
            ht_item_t *searched = ht_search(local, head->id);
            var.value_type = searched->var_type;
            //ulozit premennu niekam
            //token type
            expression_analysis(&actToken, local, &var); //null ak if

            //skontrolovat typ s premennou
            if (element->var_type != var.value_type) //result type
                return ERROR_SEMANTIC;

            gen_var_setval(var);
            head = head->next;
        }

        //ID UKLADAM DO LISTU
        //POSTUPNE OVERUJEM CI SU V LOKALNEJ
        //expressions volanie prvej var, if ciarka expressions volanie dalsiej , atd atd
        //VIAC ID = VIAC VYRAZOV
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_IF)
    {
        output = get_token(&actToken);

        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER || actToken.value.keyword != KEYWORD_NUMBER)
            return ERROR_SYNTAX_ANALYSIS;
        //var musi byt nula
        expression_analysis(&actToken, local, NULL);
        gen_if_start();

        output = Telo_Funkcie();
        .if (output != ERROR_OK) return ERROR_SYNTAX_ANALYSIS;
        gen_if_else();

        output = Telo_Funkcie();
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        gen_if_end();
        //problem treba vymazavat lokalne ifove deklaracie zo stromu
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_WHILE)
    {
        token_list_t head = Tok;
        while (//actToken.type!=KEYWORD && actToken.value.keyword != KEYWORD_END)
        {
            Tok.current = actToken;
            if (actToken.value.keyword == KEYWORD_LOCAL && actToken.type == KEYWORD)
            {
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
                if (actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER || actToken.value.keyword != KEYWORD_NUMBER)
                    return ERROR_SYNTAX_ANALYSIS;

                ht_item_t *element = ht_insert(local, var_name);
                element->var_type = actToken.value.keyword;

                gen_var_def(var_name);

                Symbol var;
                var.id = var_name;

                expression_analysis(&actToken, local, &var); //null ak if

                //skontrolovat typ s premennou
                if (element->var_type != var.value_type)
                    return ERROR_SEMANTIC;

                gen_var_setval(var);
                Tok.current = actToken;
            }
            Tok = Tok.next;
           
        }
    Tok = head;
    if(Tok.current.type!=KEYWORD || Tok.current.value.keyword!= KEYWORD_WHILE)
        return ERROR_SYNTAX_ANALYSIS;
    gen_loop_start();
    Tok=Tok.next;
    if(actToken.value.keyword != KEYWORD_STRING || actToken.value.keyword != KEYWORD_INTEGER || actToken.value.keyword != KEYWORD_NUMBER)
        return ERROR_SYNTAX_ANALYSIS;
    expression_analysis(Tok,local,NULL);
    Telo_Funkcie_vo_while();
    gen_loop_end();
    
        //WHILE
        //linked list
        //vzdy ked bude while zavola podmienku marekovi atd
    }

    else if (actToken.type == KEYWORD && actToken.value.keyword == KEYWORD_RETURN) //TODO
    {
        //HLOZI MUSI NIECO VYMYSLIET
    }

    else if (actToken.type == BUILT_IN_READI) //ak nieco ine
    {
        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != LEFT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;

        output = get_token(&actToken);
        if (output != ERROR_OK)
            return ERROR_SYNTAX_ANALYSIS;
        if (actToken.type != RIGHT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;
    }
    else if (actToken.type == BUILT_IN_READN) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_READS) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_CHR) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_ORD) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_SUBSTR) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_WRITE) //ak nieco ine
    {
    }
    else if (actToken.type == BUILT_IN_TOINTEGER) //ak nieco ine
    {
    }
    return Telo_Funkcie_vo_while();
} */