#ifndef _PARSER_H
#define _PARSER_H

#include <error.h>

typedef enum{
    
}keywords;

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