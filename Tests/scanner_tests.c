/**
* Project - Compiler for IFJ21
* 
* @brief Tests for scanner for IFJ21 - Lexical analysis
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
Token token;
FILE *file;

int main(){

    fprintf(stdout,"\x1B[31m""---------------- Scanner tests ----------------\n""\x1B[0m");
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

assert(dyn_string_compare(z, "iahoj")==0);



    fprintf(stdout,"\x1B[32m""                     PASSED                    \n""\x1B[0m"); 
    fprintf(stdout,"-----------------------------------------------\n");
    fprintf(stdout,"------------ Lexical analysis tests -----------\n");


// The important thing to remember in relative paths is that it is relative to the current working directory when the (f*cking) EXECUTABLE is run.

// file = "+ return"
file = fopen("IFJ21_codes/first_test.ifj21","r");
set_source(file);

get_token(&token); 
assert(token.type==PLUS);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_RETURN);
fclose(file);

// file = "local x: number = 0.5"
file = fopen("IFJ21_codes/second_test.ifj21", "r");
set_source(file);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);

get_token(&token);
assert(token.type==ID);
assert(dyn_string_compare(token.value.string,"x")==0);           //the fuk

get_token(&token);

assert(token.type==COLON);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_NUMBER);

get_token(&token);
assert(token.type==ASSIGN);
get_token(&token);
assert(token.type==DECIMAL_NUMBER);
assert(token.value.decimal_value==0.5e4);
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
//assert(dyn_string_compare(token.value.string, "x"));
get_token(&token);
assert(token.type==COLON);
get_token(&token);
assert(token.type==ID);
get_token(&token);
assert(token.type==ASSIGN);
get_token(&token);
assert(token.type==STRING);
//assert(dyn_string_compare(token.value.string, "TESTING")==0);
fclose(file);

//hello.ifj21
file = fopen("IFJ21_codes/hello.ifj21", "r");
set_source(file);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
 // line comment
get_token(&token);
assert(token.type==KEYWORD); // require 
assert(token.value.keyword==KEYWORD_REQUIRE);
get_token(&token); // "ifj21"
assert(token.type==STRING);
get_token(&token);// function
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_FUNCTION);
get_token(&token); // main
assert(token.type==ID);
get_token(&token); // ( 
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token); // local
assert(token.type==KEYWORD);
get_token(&token); // str
assert(token.type==ID);
get_token(&token); // :
assert(token.type==COLON);
get_token(&token); // string
assert(token.type==KEYWORD);
get_token(&token); // =
assert(token.type==ASSIGN);
get_token(&token); // "Hello World"
assert(token.type==STRING);
get_token(&token); // write
assert(token.type==ID);                         // supposed to be built-in function
get_token(&token); // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // str
assert(token.type==ID);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS); 
// line comment
get_token(&token); // write
assert(token.type==ID);
get_token(&token); // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // "Hello world"
assert(token.type==STRING);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token); // end
assert(token.type==KEYWORD);
fclose(file);


// fourth_test.ifj21
file = fopen("IFJ21_codes/fourth_test.ifj21", "r");
set_source(file);

get_token(&token); //require
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_REQUIRE);
get_token(&token);  //"ifj21"
assert(token.type==STRING);
get_token(&token);  //local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);  // z
assert(token.type==ID);
get_token(&token);  //:
assert(token.type==COLON);
get_token(&token);  //string
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_STRING);
get_token(&token);  //=
assert(token.type==ASSIGN);
get_token(&token);  //"test"
assert(token.type==STRING);
// /n
// block comment
get_token(&token); //function
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_FUNCTION);
get_token(&token); //fun
assert(token.type==ID);
get_token(&token);  //(
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  //x1
assert(token.type==ID);
get_token(&token);  //:
assert(token.type==COLON);
get_token(&token);  //string
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_STRING);
get_token(&token);  //)
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  //write
assert(token.type==ID);
get_token(&token);  //(
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  //x1
assert(token.type==ID);
get_token(&token);  //)
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  //end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);
fclose(file);                               // end of file ? no token type?

// ifs and loops
file = fopen("IFJ21_codes/ifs&loops.ifj21", "r");
set_source(file);

get_token(&token);  // require
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_REQUIRE);
get_token(&token);  // "ifj21"
assert(token.type==STRING);
get_token(&token);  // function
assert(token.type==KEYWORD);
get_token(&token);  // main
assert(token.type==ID);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  // local
assert(token.type==KEYWORD);
get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // integer
assert(token.type==KEYWORD);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token); // 5
assert(token.type==INT);
get_token(&token);  // while 
assert(token.type==KEYWORD);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // <
assert(token.type==LESS_THAN);
get_token(&token);  // 10
assert(token.type==INT);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  // do
assert(token.type==KEYWORD);
get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // +
assert(token.type==PLUS);
get_token(&token);  // 1
assert(token.type==INT);
get_token(&token);  // end
assert(token.type==KEYWORD);
get_token(&token);  // if
assert(token.type==KEYWORD);
get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // >=
assert(token.type==GREATER_or_EQUALS);
get_token(&token);  // 5
assert(token.type==INT);
get_token(&token);  // then
assert(token.type==KEYWORD);
get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // 50
assert(token.type==INT);
get_token(&token);  // else
assert(token.type==KEYWORD);
get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // 0
assert(token.type==INT);
get_token(&token);  // end
assert(token.type==KEYWORD);
get_token(&token);  // end
assert(token.type==KEYWORD);

get_token(&token);  // main
assert(token.type==ID);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

fclose(file);

// strings
file = fopen("IFJ21_codes/strings.ifj21", "r");
set_source(file);

assert(get_token(&token)==0);  // require
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_REQUIRE);
assert(get_token(&token)==0);    // "ifj21"
assert(token.type==STRING);

get_token(&token);  // function
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_FUNCTION);
get_token(&token);  // main
assert(token.type==ID);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
assert(get_token(&token)==0);  // s1
assert(token.type==ID);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // string
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_STRING);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // "Toto je nejaky text"
assert(token.type==STRING);

get_token(&token);  // local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);  // s2
assert(token.type==ID);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // string
assert(token.type==KEYWORD);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string,"s1")==0); 
get_token(&token);  // ..
assert(token.type==CONCATENATE);
get_token(&token);  // " ktery jeste trochu obohatime"
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string,", ktery jeste trochu obohatime")==0); 
get_token(&token);  // write
assert(token.type==ID);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // s1
assert(token.type==ID);
get_token(&token);  // ,
assert(token.type== COMMA);
get_token(&token);  // "\010"
assert(token.type==STRING);
get_token(&token);  // ,
assert(token.type== COMMA);
get_token(&token);  // s2
assert(token.type==ID);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  // local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);  // s1len
assert(token.type==ID);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // #
assert(token.type==GET_LENGTH);
get_token(&token);  // s1
assert(token.type==ID);
get_token(&token);  // local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);  // s1len4
assert(token.type==ID);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // s1len
assert(token.type==ID);






fclose(file);


    fprintf(stdout,"\x1B[32m""                     PASSED                    \n""\x1B[0m"); 
    fprintf(stdout,"-----------------------------------------------\n");

   return 0;
}
