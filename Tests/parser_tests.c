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
//#include "../expressions.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

Token token;
FILE *file;
int res;

int main(){
    fprintf(stdout,"\x1B[31m""---------------- Parser tests ----------------\n""\x1B[0m");
    fprintf(stdout,"-----------------------------------------------\n");
    fprintf(stdout,"--------------- Expressions tests -------------\n");

/* file = fopen("IFJ21_codes/Expressions/basic.txt", "r");
set_source(file);
get_token(&token);
res = expression_analysis(&token);
fclose(file); */

    fprintf(stdout,"\x1B[32m""                     PASSED                    \n""\x1B[0m");
    fprintf(stdout,"-----------------------------------------------\n");
    fprintf(stdout,"------------- Parser tests ------------\n");

file = fopen("IFJ21_codes/func_test.ifj21", "r");
set_source(file);
int res = parse();
assert(res==0);
fclose(file);

    fprintf(stdout,"\x1B[32m""                     PASSED                    \n""\x1B[0m"); 
}