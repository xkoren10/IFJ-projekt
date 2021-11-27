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
    if(/* !string  */){
        return ERROR_SYNTAX_ANALYSIS;
    }
    else{
        //TODO načítanie stringu

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
        output = Typy();

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

        return ERROR_OK;


    }
}
    int Typy(){

    }

    int Definicia_Funckie(){
        int output;
        output = get_token(&actToken);
        output = Is_EOL();

        output = Hlavicka_Funkcie()
      
        output = Telo_Funkcie();






    }
    



}



