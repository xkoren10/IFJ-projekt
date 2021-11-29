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
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

FILE *file;

int main(){
    fprintf(stdout,"\x1B[31m""---------------- Parser tests ----------------\n""\x1B[0m");
    fprintf(stdout,"-----------------------------------------------\n"); 

file = fopen("IFJ21_codes/simple_ops", "r");
set_source(file);

    fprintf(stdout,"\x1B[32m""                     PASSED                    \n""\x1B[0m"); 
}