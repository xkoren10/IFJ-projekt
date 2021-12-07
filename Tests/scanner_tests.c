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
    fprintf(stdout,"------------ Lexical analysis tests -----------\n\n");


// The important thing to remember in relative paths is that it is relative to the current working directory when the (f*cking) EXECUTABLE is run.

// file = "+ return"

file = fopen("IFJ21_codes/first_test.ifj21","r");
set_source(file);

get_token(&token); 
assert(token.type==PLUS);
get_token(&token);
assert(token.type==DECIMAL_NUMBER);
assert(token.value.decimal_value==0.5);
get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_RETURN);
fclose(file);


        fprintf(stdout,"--> IFJ21_codes/first_test.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


// file = "local x: number = 0.5"
file = fopen("IFJ21_codes/second_test.ifj21", "r");
set_source(file);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);


get_token(&token);
assert(token.type==ID);


assert(dyn_string_compare(token.value.string,"x")==0);      

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


        fprintf(stdout,"--> IFJ21_codes/second_test.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

//file = "global z: chyba = "TESTING""
file = fopen("IFJ21_codes/third_test.ifj21", "r");
set_source(file);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_GLOBAL);
get_token(&token);
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "z")==0);
get_token(&token);
assert(token.type==COLON);
get_token(&token);
assert(token.type==ID);
get_token(&token);
assert(token.type==ASSIGN);
get_token(&token);
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "TESTING")==0);
fclose(file);


        fprintf(stdout,"--> IFJ21_codes/third_test.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

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
assert(token.type==BUILT_IN_WRITE);
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

        fprintf(stdout,"--> IFJ21_codes/fourth_test.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

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
assert(token.type==BUILT_IN_WRITE);                         // supposed to be built-in function
get_token(&token); // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // str
assert(token.type==ID);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS); 
// line comment
get_token(&token); // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token); // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // "Hello world"
assert(token.type==STRING);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token); // end
assert(token.type==KEYWORD);
fclose(file);

        fprintf(stdout,"--> IFJ21_codes/hello.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


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
assert(token.value.keyword==KEYWORD_IF);
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

        fprintf(stdout,"--> IFJ21_codes/ifs&loops.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

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
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // s1
assert(token.type==ID);
get_token(&token);  // ,
assert(token.type== COMMA);
get_token(&token);  // "\010"
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "\\010")==0);
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

get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1len")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // s1len
assert(token.type==ID);
get_token(&token);  // -
assert(token.type==MINUS);
get_token(&token);  // 4
assert(token.type==INT);
assert(token.value.integer_value==4);

get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // substr
assert(token.type==BUILT_IN_SUBSTR);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // s2
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s2")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1len")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1len4")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1len")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1len")==0);
get_token(&token);  // +
assert(token.type==PLUS);
get_token(&token);  // 4
assert(token.type==INT);
assert(token.value.integer_value==1);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // 4 zaky od
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "4 znaky od")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1len")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token); // . znaku v
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, ". znaku v \\\"")==0);        // pozn.: \\ => '\' ; \" => " ; \\\" => \"
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);  // s2
assert(token.type==ID);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "\\\":")==0); 
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token); // \n
assert(dyn_string_compare(token.value.string, "\\010")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // 
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Zadejte serazenou posloupnost vsech malych pismen a-h, ")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS); 

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // 
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "pricemz se pismena nesmeji v posloupnosti opakovat: ")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // reads
assert(token.type==BUILT_IN_READS);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_IF);
get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1")==0);
get_token(&token);  // ~=
assert(token.type==NOT_EQUAL);
get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_NIL);
get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_THEN);

get_token(&token);//while
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_WHILE);
get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1")==0);
get_token(&token);  // ~=
assert(token.type==NOT_EQUAL);
get_token(&token); // 
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "abcdefgh")==0);
get_token(&token);//do
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_DO);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // \n
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "\\010")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);// Spatne zadana posloupnost, zkuste znovu:
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Spatne zadana posloupnost, zkuste znovu:")==0);        // pozn.: \\ => '\' ; \" => " ; \\\" => \"
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "s1")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // reads
assert(token.type==BUILT_IN_READS);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);
get_token(&token);  // else
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_ELSE);
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);
get_token(&token); // main
assert(token.type==ID);
get_token(&token); // ( 
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS);



fclose(file);

        fprintf(stdout,"--> IFJ21_codes/strings.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


//ifs&loops
file = fopen("IFJ21_codes/simple_ops.ifj21", "r");
set_source(file);

assert(get_token(&token)==0);  // require
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_REQUIRE);
assert(get_token(&token)==0);    // "ifj21"
assert(token.type==STRING);

get_token(&token);//global
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);//x
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);
get_token(&token);//:
assert(token.type==COLON);
assert(get_token(&token)==0);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // 5
assert(token.type==INT);
assert(token.value.integer_value==5);

get_token(&token);// function
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_FUNCTION);
get_token(&token); // main
assert(token.type==ID);
get_token(&token); // ( 
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);//local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);//y
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "y")==0);
get_token(&token);//:
assert(token.type==COLON);
assert(get_token(&token)==0);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);

get_token(&token);//local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);//z
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "z")==0);
get_token(&token);//:
assert(token.type==COLON);
assert(get_token(&token)==0);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // 5
assert(token.type==INT);
assert(token.value.integer_value==0);


get_token(&token);//y
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "y")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);//x
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);
get_token(&token);  // +
assert(token.type==PLUS);
get_token(&token);//z
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "z")==0);

get_token(&token);//x
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);//x
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);
get_token(&token);  // *
assert(token.type==MULTIPLY);
get_token(&token);//x
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);
get_token(&token);  // +
assert(token.type==PLUS);
get_token(&token);  // 5
assert(token.type==INT);
assert(token.value.integer_value==5);

get_token(&token);//z
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "z")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);//x
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);
get_token(&token);  // -
assert(token.type==MINUS);
get_token(&token);//y
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "y")==0);
get_token(&token);  // +
assert(token.type==PLUS);
get_token(&token);  // 0
assert(token.type==INT);
assert(token.value.integer_value==0);

get_token(&token);//y
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "y")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);//x
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "x")==0);
get_token(&token);  // /
assert(token.type==INTEGER_DIVIDE);
get_token(&token);//z
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "y")==0);
get_token(&token);  // *
assert(token.type==MULTIPLY);
get_token(&token);  // 1
assert(token.type==INT);
assert(token.value.integer_value==1);

get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);




fclose(file);


        fprintf(stdout,"--> IFJ21_codes/simple_ops.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


//factorial
file = fopen("IFJ21_codes/factorial.ifj21", "r");
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
get_token(&token);  // a
assert(token.type==ID);
assert(dyn_string_compare(token.value.string,"a")==0);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);

get_token(&token);  // local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);  // vysl
assert(token.type==ID);
assert(dyn_string_compare(token.value.string,"vysl")==0);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // 0
assert(token.type==INT);
assert(token.value.integer_value==0);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // Cislo pro vypocet faktorialu:\n
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Cislo pro vypocet faktorialu:\\010")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // readi
assert(token.type==BUILT_IN_READI);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // if
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_IF);
get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  // ==
assert(token.type==EQUALS);
get_token(&token);  // nil
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_NIL);
get_token(&token);  // then
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_THEN);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // a je nil\n
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "a je nil\\010")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  // return
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_RETURN);

get_token(&token);  // else
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_ELSE);

get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);

get_token(&token);  // if
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_IF);
get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  // <
assert(token.type==LESS_THAN);
get_token(&token);  // 0
assert(token.type==INT);
assert(token.value.integer_value==0);
get_token(&token);  // then
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_THEN);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // Faktorial nejde spocitat\n
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Faktorial nejde spocitat\\010")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  // else
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_ELSE);

get_token(&token);  // vysl
assert(token.type==ID);
assert(dyn_string_compare(token.value.string,"vysl")==0);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // 1
assert(token.type==INT);
assert(token.value.integer_value==1);

get_token(&token);  // while 
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_WHILE);
get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  //>
assert(token.type==GREATER_THAN);
get_token(&token);  // 0
assert(token.type==INT);
assert(token.value.integer_value==0);
get_token(&token);  // do 
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_DO);
get_token(&token);  // vysl
assert(token.type==ID);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // vysl
assert(token.type==ID);
get_token(&token);  // *
assert(token.type==MULTIPLY);
get_token(&token);  // a
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "a")==0);
get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  // -
assert(token.type==MINUS);
get_token(&token);  // 1
assert(token.type==INT);
assert(token.value.integer_value==1);
// line comment
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // Vysledok je:
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Vysledok je: ")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "vysl")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token); // . znaku v
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "\\010")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);      
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END); 

fclose(file);


        fprintf(stdout,"--> IFJ21_codes/factorial.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

//basic
file = fopen("IFJ21_codes/factorial-rec.ifj21", "r");
set_source(file);

assert(get_token(&token)==0);  // require
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_REQUIRE);
assert(get_token(&token)==0);    // "ifj21"
assert(token.type==STRING);

get_token(&token); //function
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_FUNCTION);
get_token(&token); //factorial
assert(token.type==ID);
get_token(&token);  //(
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  //n
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "n")==0);
get_token(&token);  //:
assert(token.type==COLON);
get_token(&token);  //integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  //)
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  //:
assert(token.type==COLON);
get_token(&token);  //integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);
assert(token.type==ID);
assert(dyn_string_compare(token.value.string,"n1")==0);      
get_token(&token);
assert(token.type==COLON);
get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);
assert(token.type==ASSIGN);
get_token(&token);  //n
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "n")==0);
get_token(&token);  // -
assert(token.type==MINUS);
get_token(&token);  // 4
assert(token.type==INT);
assert(token.value.integer_value==1);
get_token(&token);  // if
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_IF);
get_token(&token);  // n
assert(token.type==ID);
get_token(&token);  // <
assert(token.type==LESS_THAN);
get_token(&token);  // 2
assert(token.type==INT);
assert(token.value.integer_value==2);
get_token(&token);  // then
assert(token.type==KEYWORD);
get_token(&token);  // return
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_RETURN);
get_token(&token);  // 2
assert(token.type==INT);
assert(token.value.integer_value==1);

get_token(&token);  // else
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_ELSE);
get_token(&token);  // local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);  // tmp
assert(token.type==ID);
get_token(&token);  //:
assert(token.type==COLON);
get_token(&token);  //integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  //=
assert(token.type==ASSIGN);
get_token(&token);  // factorial
assert(token.type==ID);
get_token(&token);  //(
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  //n
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "n1")==0);
get_token(&token);  //)
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  //return
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_RETURN);
get_token(&token);  // n
assert(token.type==ID);
get_token(&token);  // *
assert(token.type==MULTIPLY);
get_token(&token);  // tmp
assert(token.type==ID);
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);

get_token(&token);// function
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_FUNCTION);
get_token(&token); // main
assert(token.type==ID);
get_token(&token); // ( 
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // Cislo pro vypocet faktorialu:\n
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Cislo prE vypocet faktorialu: ")==0);
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
get_token(&token);  // readi
assert(token.type==BUILT_IN_READI);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_IF);
get_token(&token);  // s1
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "a")==0);
get_token(&token);  // ~=
assert(token.type==NOT_EQUAL);
get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_NIL);
get_token(&token);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_THEN);

get_token(&token);  // if
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_IF);
get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  // <
assert(token.type==LESS_THAN);
get_token(&token);  // 0
assert(token.type==INT);
assert(token.value.integer_value==0);
get_token(&token);  // then
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_THEN);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // Faktorial nejde spocitat\n
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Faktorial nejde spocitat!")==0);
get_token(&token);  // ,
assert(token.type== COMMA);
get_token(&token);  // "\010"
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "\\010")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  // else
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_ELSE);

get_token(&token);  // local
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_LOCAL);
get_token(&token);  // vysl
assert(token.type==ID);
assert(dyn_string_compare(token.value.string,"vysl")==0);
get_token(&token);  // :
assert(token.type==COLON);
get_token(&token);  // integer
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_INTEGER);
get_token(&token);  // =
assert(token.type==ASSIGN);
get_token(&token); //factorial
assert(token.type==ID);
get_token(&token);  //(
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // a
assert(token.type==ID);
get_token(&token);  //)
assert(token.type==RIGHT_PARENTHESIS);

get_token(&token);  // write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  // (
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  // Vysledok je:
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Vysledok je ")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token);  // s1len
assert(token.type==ID);
assert(dyn_string_compare(token.value.string, "vysl")==0);
get_token(&token);  // ,
assert(token.type==COMMA);
get_token(&token); // . znaku v
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "\\010")==0);
get_token(&token);  // )
assert(token.type==RIGHT_PARENTHESIS);      
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);

get_token(&token);  // else
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_ELSE);
get_token(&token);  //write
assert(token.type==BUILT_IN_WRITE);
get_token(&token);  //(
assert(token.type==LEFT_PARENTHESIS);
get_token(&token);  //Chyba pri nacitani celeho cisla!\n
assert(token.type==STRING);
assert(dyn_string_compare(token.value.string, "Chyba pri nacitani celeho cisla!\\010")==0);
get_token(&token);  //)
assert(token.type==RIGHT_PARENTHESIS);
get_token(&token);  //end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);
get_token(&token);  // end
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_END);

get_token(&token); // main
assert(token.type==ID);
get_token(&token); // ( 
assert(token.type==LEFT_PARENTHESIS);
get_token(&token); // )
assert(token.type==RIGHT_PARENTHESIS);

fclose(file);

        fprintf(stdout,"--> IFJ21_codes/factorial-rec.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


//basic
file = fopen("IFJ21_codes/basic.ifj21", "r");
set_source(file);

get_token(&token);  // x
assert(token.type==ID);
get_token(&token);  // 5
assert(token.type==INT);
assert(token.value.integer_value==5);
get_token(&token);  // +
assert(token.type==PLUS);
get_token(&token);  // 4
assert(token.type==INT);
assert(token.value.integer_value==4);
get_token(&token);  // *
assert(token.type==MULTIPLY);
get_token(&token);  // 8
assert(token.type==INT);
assert(token.value.integer_value==8);





fclose(file);


        fprintf(stdout,"--> IFJ21_codes/basic.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

//janko
file = fopen("IFJ21_codes/janko.ifj21", "r");
set_source(file);

get_token(&token);
assert(token.type == ID);
assert(dyn_string_compare(token.value.string, "janko")==0);

 fclose(file);


        fprintf(stdout,"--> IFJ21_codes/janko.ifj21 => ");
        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

    fprintf(stdout,"-----------------------------------------------\n");

   return 0;
}
