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

 //TODO
tStack stack;
//EOL
bool scanner_EOL;

string Function_name;

int Param_counter;

int label_counter;

ht_table_t global;

ht_table_t local;





int Next_token();

void Is_Eol();

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