/**
* Project - Compiler for IFJ21
* 
* @brief Tests for parser for IFJ21 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file parser_tests.c
*
**/
#include <stdlib.h>
#include "../error_codes.h"
#include "../dyn_string.h"
#include "../parser.h"
#include "../expressions.h"
#include "../scanner.h"
#include "../symtable.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

Token token;
FILE *file;
int res;
ht_table_t *symtable_ptr;
ht_table_t symtable;


int main(){

//ht_init(symtable);   not needed now
//fprintf(stdout,"%d type -- %d res\n", token.type, res); prípadný test

    fprintf(stdout,"\x1B[31m""---------------- Expressions tests ----------------\n""\x1B[0m");

file = fopen("IFJ21_codes/Expressions/basic.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 5);
res = expression_analysis(&token,&symtable);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

get_token(&token);
res = expression_analysis(&token,&symtable);
assert(token.type == STATE_EOF);
assert(res==0);

fclose(file);


                        fprintf(stdout,"--> IFJ21_codes/Expressions/basic.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

file = fopen("IFJ21_codes/Expressions/basic2.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 3);
res = expression_analysis(&token,&symtable);
assert(token.type == STATE_EOF);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/basic2.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


file = fopen("IFJ21_codes/Expressions/conditions.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 5);
res = expression_analysis(&token,&symtable);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/conditions.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

file = fopen("IFJ21_codes/Expressions/conditions2.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 20);
res = expression_analysis(&token,&symtable);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/conditions2.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

file = fopen("IFJ21_codes/Expressions/conditions3.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == STRING);
assert(dyn_string_compare(token.value.string, "test")==0);
res = expression_analysis(&token,&symtable);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/conditions3.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

file = fopen("IFJ21_codes/Expressions/strlen.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == GET_LENGTH);
res = expression_analysis(&token,&symtable);
assert(token.type == STATE_EOF);
assert(res==0);

fclose(file);


                        fprintf(stdout,"--> IFJ21_codes/Expressions/strlen.txt => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


file = fopen("IFJ21_codes/Expressions/concatenate.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == STRING);
res = expression_analysis(&token,&symtable);
assert(token.type == STATE_EOF);
assert(res==6);

fclose(file);


                        fprintf(stdout,"--> IFJ21_codes/Expressions/concatenate.txt => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

}