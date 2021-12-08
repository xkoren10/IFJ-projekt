/**
 *  Project - Compiler for IFJ21
 *
 * @brief Parser for IFJ21 - syntax analysis
 *
 * @author Lukasz Pycz <xpyczl00>
 * @file parser.h
 *
 **/

#ifndef _PARSER_H
#define _PARSER_H

#include "error_codes.h"
#include "dyn_string.h"
#include "symtable.h"
#include "scanner.h"
#include <stdbool.h>

typedef struct token_list
{
    Token current;
    struct token_list_t *next;
} token_list_t;

typedef struct id_list
{
    char *id;
    struct id_list_t *next;
} id_list_t;

token_list_t Tok;

Token actToken;

ht_table_t *global;

ht_table_t *local;

ht_item_t *current_item;

func_val_t *current_LL;

func_val_t *current_LL2;

char *func_name;
char *var_name;

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

Token_type Typ();

int COMPARE_Typy();

int COMPARE_Typ();

#endif