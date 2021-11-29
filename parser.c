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

    Stack_Init(&syntaxStack);
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

        //Vypis stringu "require ifj21"       
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
        
        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type!=LEFT_PARENTHESIS){
            return ERROR_SYNTAX_ANALYSIS;
        }
        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type !=RIGHT_PARENTHESIS)
            output = Typy();

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

        output = Typy();

        return output;


    }
}
    int Typy(){
        //TODO AK NENI PRVY
        int output;
        output = Typ();
        // ak chyba return
        if(output != ERROR_OK)
                return output;

        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type == COMMA)
        {    
            Typy ();}
        else
            return ERROR_OK;

    }
    int Typ(){
        int output;
        output = get_token(&actToken);
        output = Is_EOL();
        if(actToken.type!= KEYWORD)

        switch (actToken.type)
        {
        case KEYWORD_INTEGER:
        case KEYWORD_STRING:
        case KEYWORD_NUMBER:
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

        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type !=LEFT_PARENTHESIS)
            return ERROR_SYNTAX_ANALYSIS;
        
        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type != RIGHT_PARENTHESIS)
            output = Zoznam_parametrov();

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
int Zoznam_parametrov(){
    int output;
    output = Parameter();

    if(output != ERROR_OK)
        return output;

    output = get_token(&actToken);
    output = Is_EOL();

    if(actToken.type == COMMA)
        {    
            Parameter ();}
        else
            return ERROR_OK;






}

int Parameter(){
      int output;
        output = get_token(&actToken);
        output = Is_EOL();
        if(actToken.type!=ID)
            return ERROR_SYNTAX_ANALYSIS;

        //asi nacitat string atd


        output = get_token(&actToken);
        output = Is_EOL();

        if(actToken.type!=COMMA)
            return ERROR_SYNTAX_ANALYSIS;

        output = Typ();
        return output;
}


    







