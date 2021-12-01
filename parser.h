#ifndef _PARSER_H
#define _PARSER_H

#include "error_codes.h"
#include "dyn_string.h"
#include "stack.h"
#include "symtable.h"
#include "scanner.h"
#include <stdbool.h>

//current token
Token actToken;

/* //next token ehm bud toto alebo nejaka funkcia
Token nextToken; */

ht_table_t *global;

ht_table_t *local;

ht_item_t *current_item;

func_val_t *current_LL;


char* func_name;
char* var_name;

//ALEBO AJ TOKEN
Token_type current;

int Next_token();

int Is_Eol();

int parse();

int Program();

int Deklaracie_or_Definicie();

int Deklaracie_Funckie();

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

#endif