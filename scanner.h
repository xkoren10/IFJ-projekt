#ifndef _SCANNER_H
#define _SCANNER_H


#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>

#define ADD         10  //  +
#define SUB         11  //  -
#define DIV         12  //  /
#define MUL         13  //  *


#define L_PAR       41  //  (
#define R_PAR       42  //  )
#define L_BR        43  //  {
#define R_BR        44  //  }
#define ASSIGN      45  //  =
#define COMMA       46  //  ,
#define SEMICOL     47  //  ;
#define VAR_DEF     48  //  :=

#define EOL         50
#define ENDFILE     51

#define START       60
#define S_ASSIGN    61
#define S_DIV       62
#define S_L         63
#define S_G         64
#define S_NEG       65
#define S_ID        66
#define S_KW        67
#define S_INT       68
#define S_FLOAT     69
#define S_FLOAT2    70
#define S_FLOAT_E   71
#define S_STRING    72
#define L_COM       73
#define S_VAR_DEF   74
#define S_BLOCK     75
#define S_ESC       76
#define S_HEX       77

#endif /* SCANNER_H*/