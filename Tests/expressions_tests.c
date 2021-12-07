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
#include "../expressions.h"
#include "../scanner.h"
#include "../symtable.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

Token token;
Token token_janko;
FILE *file;
int res;
ht_table_t test_symtable;

ht_table_t *symtable_ptr=&test_symtable;

ht_table_t test_symtable2;

ht_table_t *symtable_ptr2=&test_symtable2;

ht_item_t * item;

Symbol sym;
Symbol *symptr=&sym;

token_list_t t_list;
token_list_t * t_list_ptr=&t_list;

int main(){

ht_init(symtable_ptr);


ht_insert(symtable_ptr,"striger");

item = ht_search(symtable_ptr,"striger");
item->var_type=STRING;

ht_insert(symtable_ptr,"intengener");

item = ht_search(symtable_ptr,"intengener");
item->var_type=INT;

ht_insert(symtable_ptr,"strindger");

item = ht_search(symtable_ptr,"strindger");
item->var_type=STRING;

ht_insert(symtable_ptr2,"janko");

item = ht_search(symtable_ptr2,"janko");
item->var_type=ID;





//fprintf(stdout,"%d type -- %d res -- %s --id %s string\n", token_janko.type, res, sym.value_type, sym.id); prípadný test

    fprintf(stdout,"\x1B[31m""---------------- Expressions tests ----------------\n""\x1B[0m");

file = fopen("IFJ21_codes/Expressions/basic.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 5);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

get_token(&token);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 69);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==9);     // kde res == 9??

get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 42);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == DECIMAL_NUMBER);
assert(res==2);                                             // Syntax analysis

fclose(file);


                        fprintf(stdout,"--> IFJ21_codes/Expressions/basic.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

file = fopen("IFJ21_codes/Expressions/basic2.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 3);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(strcmp(sym.value_type,"E")==0);
assert(sym.result_type==INT);
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
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/conditions.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

file = fopen("IFJ21_codes/Expressions/conditions2.txt", "r");
set_source(file);
get_token(&token_janko);
assert(token_janko.type == INT);
assert(token_janko.value.integer_value == 20);
res = expression_analysis(&token_janko,symtable_ptr,symptr,t_list_ptr);
assert(token_janko.type == KEYWORD);
assert(token_janko.value.keyword == KEYWORD_THEN);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/conditions2.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 

file = fopen("IFJ21_codes/Expressions/conditions3.txt", "r");
set_source(file);
get_token(&token);
assert(token.type == STRING);
assert(dyn_string_compare(token.value.string, "test")==0);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/conditions3.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 



file = fopen("IFJ21_codes/Expressions/concatenate.txt", "r");
set_source(file);
//string and int
get_token(&token);
assert(token.type == STRING);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==6);

//string and string

get_token(&token);
assert(token.type == STRING);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);

//int and int

get_token(&token);
assert(token.type == ID);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==6);

//string and ()
get_token(&token_janko);
assert(token_janko.type == STRING);
res = expression_analysis(&token_janko,symtable_ptr,symptr,t_list_ptr);
assert(token_janko.type == KEYWORD);
assert(token_janko.value.keyword == KEYWORD_THEN);
//fprintf(stdout,"%d type -- %d res -- %s\n", token.type, res, sym.value_type);
assert(res==2);

// #string
get_token(&token);
assert(token.type == GET_LENGTH);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);





ht_delete_all(symtable_ptr);
fclose(file);


                        fprintf(stdout,"--> IFJ21_codes/Expressions/concatenate.txt => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


file = fopen("IFJ21_codes/Expressions/val_types.txt", "r");
set_source(file);

get_token(&token);
assert(token.type == INT);
assert(token.value.integer_value == 5);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(strcmp(sym.value_type,"integer")==0);
assert(token.type == KEYWORD);
assert(token.value.keyword == KEYWORD_THEN);
assert(res==0);
assert(sym.value.integer==5);
assert(sym.result_type==INT);


get_token(&token);
assert(token.type == LEFT_PARENTHESIS);
assert(token.value.integer_value == 5);
res = expression_analysis(&token,symtable_ptr,symptr,t_list_ptr);
assert(strcmp(sym.value_type,"integer")==0);
assert(token.type == STATE_EOF);
assert(res==0);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/val_types.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 


file = fopen("IFJ21_codes/Expressions/ids.txt", "r");
set_source(file);

get_token(&token_janko);
assert(dyn_string_compare(token_janko.value.string, "janko")==0);
res = expression_analysis(&token_janko,symtable_ptr2,symptr,t_list_ptr);
assert(res==0);
assert(strcmp(sym.value_type,"id")==0);
assert(token_janko.type == STATE_EOF);
assert(strcmp(sym.id,"janko")==0);
assert(sym.result_type==ID);

fclose(file);

                        fprintf(stdout,"--> IFJ21_codes/Expressions/ids.ifj21 => ");
                        fprintf(stdout,"\x1B[32m"" PASSED \n""\x1B[0m"); 
}