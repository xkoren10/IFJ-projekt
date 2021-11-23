/**
* Project - Compiler for IFJ21
* 
* @brief Scanner for IFJ21 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file scanner_tests.c
*
**/


#include <stdlib.h>
#include "../error_codes.h"
#include "../dyn_string.h"
#include "../scanner.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

Dyn_string s;
Dyn_string z;
Token token ;
FILE *file;

int main(){

    fprintf(stdout,"---------------- Scanner tests ----------------\n");
    fprintf(stdout,"-----------------------------------------------\n"); 
    fprintf(stdout,"------------- Dynamic string tests ------------\n");



assert(dyn_string_init(&s)==0);

    dyn_string_clear(&s);

assert(s.length==0);
assert(s.size==0);
assert(s.string[0]=='\0');
assert(dyn_string_add_char(&s, 'i')==true);
assert(s.length==1);
assert(s.string[0]=='i');

assert(dyn_string_add_const_str(&s, "ahoj")==true);
assert(s.string[1]=='a');
assert(s.length==5);


assert(dyn_string_copy(&s, &z)==true);
assert(s.length==z.length);

assert(dyn_string_compare(&s, "iahoj")==0);



    fprintf(stdout,"                     PASSED                    \n"); 
    fprintf(stdout,"-----------------------------------------------\n");
    fprintf(stdout,"------------ Lexical analysis tests -----------\n");


// The important thing to remember in relative paths is that it is relative to the current working directory when the (f*cking) EXECUTABLE is run.

// file = "+ return"
file = fopen("IFJ21_codes/first_test.ifj21","r");
set_source(file);

get_token(&token); 
assert(token.line==0);
assert(token.start==0);
assert(token.end==0);
assert(token.lenght==1);
assert(token.type==PLUS);

get_token(&token);
assert(token.line==0);
assert(token.start==0);
assert(token.end==0);
assert(token.lenght==8);                        //don't question the elevated one
assert(token.type==KEYWORD);
assert((token.value.keyword=KEYWORD_RETURN));
fclose(file);

// file = "local x: number = 0.5"
file = fopen("IFJ21_codes/second_test.ifj21", "r");
set_source(file);

get_token(&token);
assert(token.line==1);
assert(token.start==0);
assert(token.end==4);
assert(token.lenght==5);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);

get_token(&token);
assert(token.line==1);
assert(token.start==6);
assert(token.end==6);
assert(token.lenght==1);
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);

get_token(&token);
assert(token.line==1);
assert(token.start==7);
assert(token.end==7);
assert(token.lenght==1);
assert(token.type==COLON);

get_token(&token);
assert(token.line==1);
assert(token.start==9);
assert(token.end==14);
assert(token.lenght==6);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_NUMBER);

get_token(&token);
assert(token.type==ASSIGN);
get_token(&token);
assert(token.type==NUMBER);
assert(token.value.decimal_value==0.5);
get_token(&token);
assert(token.type==EOL);
get_token(&token);
assert(token.type==STATE_EOF);

fclose(file);

//file = "global z: chyba = "TESTING""
file = fopen("IFJ21_codes/third_test.ifj21", "r");
set_source(file);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_GLOBAL);
get_token(&token);
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x"));
get_token(&token);
assert(token.type==COLON);
get_token(&token);
assert(token.type==ERROR);
get_token(&token);
assert(token.type==EQUALS);
get_token(&token);
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "TESTING"));
fclose(file);

    fprintf(stdout,"                     PASSED                    \n"); 
    fprintf(stdout,"-----------------------------------------------\n");

   return 0;
}
