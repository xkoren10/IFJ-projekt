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
*/


#include "parser.h"

//fukncia na dalsi token
int Next_token(){
    int output;
    output = get_token(&actToken);
    if(output==ERROR)
        return ERROR_LEXICAL_ANALISYS;
    return 0;
}


//funkcia na skipnutie EOLu
int Is_Eol(){
    int output = ERROR_OK;
    if(actToken.type==EOL){
       output = get_token(&actToken);
        Is_EOL();
    }
    return output;
}


int parse(){

//inicializacia SYMTABLES

    ht_init(global);
    ht_init(local);

    int output;
    output = get_token(&actToken);
    output = Is_EOL();
    
    if (output != ERROR_OK)
    {
        return ERROR_LEXICAL_ANALISYS;
    }
    else
    {
        if(actToken.type == STATE_EOF){
            return ERROR_SYNTAX_ANALYSIS;
        }
        else
            return Program();

    }

}
int Program(){
    int output;
    if(actToken.value.keyword != KEYWORD_REQUIRE){
        return ERROR_SYNTAX_ANALYSIS;
    }
    else
    {
        output = get_token(&actToken);
        output = Is_EOL();
        if (output != ERROR_OK)
        {
            return ERROR_LEXICAL_ANALISYS;
        }
        if(actToken.type == STATE_EOF)
                return ERROR_SYNTAX_ANALYSIS;

        if(!actToken.type != ID)// todo string este neprocesujem - matej koren 2021
            return ERROR_SYNTAX_ANALYSIS; 

        
        //header ()   
    }

    return Deklaracie_or_Definicie();

}

int Deklaracie_or_Definicie(){
        
        int output;
        output = get_token(&actToken);
        output = Is_EOL();
        
        if (output != ERROR_OK)
        {
            return ERROR_LEXICAL_ANALISYS;
        }
        if(actToken.type == STATE_EOF)
            return 0;
        
        else if(actToken.value.keyword == KEYWORD_GLOBAL){
            output = Deklaracie_Funckie();

        }
        else if (actToken.value.keyword == KEYWORD_FUNCTION)
        {
            output = Definicia_Funkcie();
        }
        else{
            return ERROR_SYNTAX_ANALYSIS;
        }
    if(output != ERROR_OK){
        return output;

    }
    else{
        Deklaracie_or_Definicie();

    }
 
    //ci je 
    //if eof 
    //retunr 0
    //else 
    //rekurzivne     
}
int Deklaracia_Funkcie(){

    //vsade overit eof?
    //ID, parametre nacitat do symtable a potom kontrolovat ci je/ neni vyskyt

    int output;
        output = get_token(&actToken);
        output = Is_EOL();

    //TODO
    if(actToken.type != ID ){
        return ERROR_SYNTAX_ANALYSIS;
    }
    else{
        //TODO načítanie stringu
        // nacitanie stringu do symtable
        //poslat hlozimu parametre,


        //VYTVORIM SI STRUKTURU DO KTOREJ DAM ID ALEBO LEN TMP PREMENNA NECH USETRIM VOLANIE
        ID_name = actToken.value.string.string;

        //ja by som to ale robil inak
        /* ht_insert(global,actToken.value.string); */
        

        output = get_token(&actToken);
        output = Is_EOL();
        
         if (actToken.type!=COLON)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
        
        output = get_token(&actToken);
        output = Is_EOL();
        
        
        if(actToken.value.keyword!=KEYWORD_FUNCTION){
            return ERROR_SYNTAX_ANALYSIS;
        }
        //NENI PREMENNA, VALUE == NULL
        /* ht_insert(global,ID_name,value == NULL,); */






        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type!=LEFT_PARENTHESIS){
            return ERROR_SYNTAX_ANALYSIS;
        }
        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type !=RIGHT_PARENTHESIS){
        
            func_val_t *Types= malloc(sizeof(struct func_val));
            Types->next=NULL;
            output = Typy(&Types); //poslem do typov meno a zapisu sa vstupne typy
            //TU ZACINA SRANDA, MUSIM SI VYTVORIT PREDTYM LL KTORY POSLEM DO TYPOV


            ht_item_t *current;
            ht_insert(global,ID_name);
            current = ht_search(global,ID_name);
            current->inval = Types;
            /* current->string_val =NULL;
            current->var_value = NULL; */

        }

        if(output!=ERROR_OK)
            return output;
        //asi dobre ale niesom si isty
        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type!=RIGHT_PARENTHESIS){
            return ERROR_SYNTAX_ANALYSIS;
        }

        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type!=COLON){
            return ERROR_SYNTAX_ANALYSIS;
        }
        //ll
        //novy ll
        func_val_t *outTypes= malloc(sizeof(struct func_val));
            outTypes->next=NULL;
        output = Typy(&outTypes); //vystupne typy

        Hloziho_func(ID_name,);

        return output;


    }
}
    int Typy(func_val_t *Types){ //vrati mi pointer na head linked listu
        //PRVY TYP MUSIM NASTAVIT NA HEAD, AK HEAD SA NEROVNA NULL
        

        




        int output;
        output = Typ(&Types);

        if(output != ERROR_OK)

                return output;

        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type == COMMA)
        {    
            Typy (&Types);}
        else
            return ERROR_OK;

    }
    int COMPARE_typ(char* ID_name){
        int output;
        output = get_token(&actToken);
        output = Is_EOL();
         switch (actToken.type)
        {
        case KEYWORD_INTEGER:
        case KEYWORD_STRING:
        case KEYWORD_NUMBER:
            
            //proste bude prechadzad 

            }




    }

    int Typ(func_val_t *Types){
        int output;
        output = get_token(&actToken);
        output = Is_EOL();
        /* if(actToken.type!= KEYWORD)  //TOTO CO KUR*A */

        switch (actToken.type)
        {
        case KEYWORD_INTEGER:
        case KEYWORD_STRING:
        case KEYWORD_NUMBER:
            func_val_t *p,*tmp;
            tmp = malloc(sizeof(struct func_val));
            tmp->typp=actToken.type;
            /* tmp->var_string = NULL;
            tmp->var_val = NULL; */
            if (Types == NULL)
            {
                Types = tmp;
            }
            else{
                p = Types;
                while(p->next!=NULL){
                    p = p->next;



                }
                p->next = tmp;

            }
            

        //NASTAVIM FUNC_VAL.PAR_TYPE = ACTTOKEN.TYPE
            return ERROR_OK;       
        }
        return ERROR_SYNTAX_ANALYSIS;
    }

    int Definicia_Funckie(){
        //EFOY
        int output;
        output = get_token(&actToken);
        output = Is_EOL();

        output = Hlavicka_Funkcie();
        if(output != ERROR_OK)
            return output;
      
        output = Telo_Funkcie();
        if(output != ERROR_OK)
            return output;

        return ERROR_OK;
    }

    int Hlavicka_funkcie(){
        //eofy
        int output;
        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type != KEYWORD_FUNCTION)
            return ERROR_SYNTAX_ANALYSIS;

        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type != ID)
            return ERROR_SYNTAX_ANALYSIS;
        
        
        //overenie ci je v tabulke
        ID_name = actToken.value.string.string;
        if (ht_search(global,ID_name)!=0)
            return ERROR_SEMANTIC;
        

        


        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type !=LEFT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;
        
        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type != RIGHT_PARENTHESIS){
            output = Zoznam_parametrov(ID_name);


        }
            

        output = Zoznam_parametrov();
        if(output!=ERROR_OK)
            return output;

            //tento asi preskoci jeden token 

        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type !=RIGHT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;

        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type != COLON)
            return ERROR_SYNTAX_ANALYSIS;

        output = get_token(&actToken);
        output = Is_EOL();

        output = Typy();
        

        return output;


    }
int Zoznam_parametrov(char *ID_name){
    int output;
    output = Parameter(ID_name);

    if(output != ERROR_OK)
        return output;

    output = get_token(&actToken);
    output = Is_EOL();

    if(actToken.type == COMMA)
        {    
            Zoznam_parametrov ();}
        else
            return ERROR_OK;






}

int Parameter(){
      int output;
        output = get_token(&actToken);
        output = Is_EOL();

        //pridat meno do lokalnej  
        if(actToken.type!=ID)
            return ERROR_SYNTAX_ANALYSIS;

        char* ID_name = actToken.value.string.string;
        //asi nacitat string atd actToken.value.string


        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type!=COLON)
            return ERROR_SYNTAX_ANALYSIS;

        //No, takže takto : do funkcie sa pošle nazov premennej ktory sa pripiše
        //a taktiež okrem ineho sa pošle aj typ ktory ziskame z linked listu ktorym 
        //budem musiet postupne prechadzat ale ten typ budem musiet poslat este skor, a overi sa ci je typ spravny 
        //a taktiez ci je typ uz spomenuty predtym
        
        output = COMPARE_Typ(ID_name);
        return output;
}


    







