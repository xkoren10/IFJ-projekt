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

        if(!actToken.)// todo string este neprocesujem - matej koren 2021
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
    //if neni global == return error nejaky 

    //else ak dalsi neni return error
    actToken.type=COLON
    //else 
    



}



