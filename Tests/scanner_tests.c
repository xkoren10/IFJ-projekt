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

Dyn_string s;
Dyn_string z;
Token token;
FILE *file;
unsigned token_cnt = 1; // token counter just because


int main(){

    fprintf(stdout,"---------------- Scanner tests ----------------\n");
    fprintf(stdout,"-----------------------------------------------\n"); 
    fprintf(stdout,"------------- Dynamic string tests ------------\n");



assert(dyn_string_init(&s)==true);

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

file = fopen("../IFJ21_codes/test_sc1.ifj21", "r");
get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt); token_cnt++;
assert(token.line==1);
fprintf(stdout, "line check ok");
assert(token.start==0);
fprintf(stdout, "start check ok");
assert(token.end==4);
fprintf(stdout, "end check ok");
assert(token.lenght==5);
fprintf(stdout, "lenght check ok");
assert(token.type==KEYWORD);
fprintf(stdout, "type check ok");
assert(token.value.keyword==KEYWORD_LOCAL);
fprintf(stdout, "keyword check ok");
fprintf(stdout, "token n. %d ok", --token_cnt);

get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt); token_cnt++;
assert(token.line==1);
fprintf(stdout, "line check ok");
assert(token.start==6);
fprintf(stdout, "start check ok");
assert(token.end==6);
fprintf(stdout, "end check ok");
assert(token.lenght==1);
fprintf(stdout, "lenght check ok");
assert(token.type==ID);
fprintf(stdout, "type check ok");
//assert(strcmp((token.value.string),"x")==0);
fprintf(stdout, "token n. %d ok", --token_cnt);

get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt); token_cnt++;
assert(token.line==1);
assert(token.start==7);
assert(token.end==7);
assert(token.lenght==1);
assert(token.type==COLON);
fprintf(stdout, "token n. %d ok", --token_cnt);

get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt); token_cnt++;
assert(token.line==1);
assert(token.start==9);
assert(token.end==14);
assert(token.lenght==6);
assert(token.type==KEYWORD);
assert(token.value.keyword==KEYWORD_NUMBER);
fprintf(stdout, "token n. %d ok", --token_cnt);

get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt); 
assert(token.type==ASSIGN);
fprintf(stdout, "token n. %d ok", token_cnt); token_cnt++;
get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt);
assert(token.type==NUMBER);
assert(token.value.decimal_value==0.5);
fprintf(stdout, "token n. %d ok", token_cnt);token_cnt++;
get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt);
assert(token.type==EOL);
fprintf(stdout, "token n. %d ok", token_cnt); token_cnt++;
get_token(&token);
fprintf(stdout, "got token n. %d", token_cnt);
assert(token.type==STATE_EOF);
fprintf(stdout, "token n. %d ok", token_cnt); token_cnt++;
//?assert(token.type==ERROR);
fclose(file);



    fprintf(stdout,"                     PASSED                    \n"); 
    fprintf(stdout,"-----------------------------------------------\n");

   return 0;
}
