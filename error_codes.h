/**
* Project - Compiler for IFJ21
* 
* @brief Error definitions.
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file error_codes.h
*
**/

#ifndef _ERROR_CODES_H
#define _ERROR_CODES_H

#define ERROR_OK                          0
#define ERROR_LEXICAL_ANALISYS            1
#define ERROR_SYNTAX_ANALYSIS             2
#define ERROR_SEMANTIC                    3
#define ERROR_SEMANTIC_ASSIGN_TYPE        4
#define ERROR_SEMANTIC_PARAMETERS         5
#define ERROR_SEMANTIC_EXPRESSION_TYPE    6
#define ERROR_SEMANTIC_OTHER              7
#define ERROR_RUNTIME_NIL                 8
#define ERROR_RUNTIME_DIVISON_BY_ZERO     9
#define ERROR_INTERN                      99

#endif  /* ERROR_CODES_H*/