#ifndef _PARSER_H
#define _PARSER_H

#include "error_codes.h"
#include "dyn_string.h"
/* #include "stack.h" */
#include "symtable.h"
#include "scanner.h"
/* #include "expressions.h" */
#include <stdbool.h>

//current token
Token actToken;

/* //next token ehm bud toto alebo nejaka funkcia
Token nextToken; */

ht_table_t *global;

ht_table_t *local;

ht_item_t *current_item;

struct func_val *current_LL;


char* func_name;
char* var_name;

//ALEBO AJ TOKEN
Token_type current;

int Next_token();

int Is_Eol();

int parse();

int Program();

int Deklaracie_or_Definicie();

int Deklaracie_Funkcie();

int Definicia_Funkcie();

int Hlavicka_Funkcie();

int Telo_Funkcie();

int Zoznam_parametrov();

int Parameter();

int Sekvencia_Prikazov();

int Funckie();

int Vyraz_or_Funkcia();

int Vyraz();

int Viac_ID();

int Prikaz();

int Typy(struct func_val *f);

int Typ(struct func_val *f);

int COMPARE_Typy();

int COMPARE_Typ();

#endif