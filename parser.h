#ifndef _PARSER_H
#define _PARSER_H

#include "error_codes.h"
#include "dyn_string.h"
/* #include "stack.h" */
#include "symtable.h"
#include "scanner.h"
#include <stdbool.h>

typedef struct token_list{
    Token current;
    struct token_list_t *next;
}token_list_t;

typedef struct id_list{
    char *id;
    struct id_list_t* next;
}id_list_t;

//current token
token_list_t Tok;

Token actToken;

/* //next token ehm bud toto alebo nejaka funkcia
Token nextToken; */

ht_table_t *global;

ht_table_t *local;

ht_item_t *current_item;

func_val_t *current_LL;

func_val_t *current_LL2;


 char *func_name;
char *var_name;

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

int Telo_Funkcie_v_IF();

int Zoznam_parametrov();

int Parameter();

int Sekvencia_Prikazov();

int Funckie();

int Vyraz_or_Funkcia();

int Vyraz();

int Viac_ID();

int Prikaz();


<<<<<<< HEAD
Token_type Typ();
=======
int Typ(struct func_val *f);
>>>>>>> 94a93a0d28818f3555f1aca2f0d287ee60e16d52

int COMPARE_Typy();

int COMPARE_Typ();

#endif