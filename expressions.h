/**
* Project - Compiler for IFJ21
* 
* @brief Syntax analysis of expressions
* 
* @author Marek Krizan <xkriza08>
* @file expressions.h
*
**/

#include "scanner.h"
#include "stack.h"
#include "error_codes.h"
#include "symtable.h"

/**
 * @brief Initialization and start of the expresion analysis
 * @param token Structure Token
 * @return Error code
 */
int expression_analysis(Token *token,ht_table_t *symtable);

/**
 * @brief Expresion analysis
 * @return Error code
 */
int analysis();

/**
 * @brief Finds index in the precedence table
 * @param i1 pointer to return index of row
 * @param token pointer to return index of column
 * @return Error code
 */
int find_index(int *i1,int *i2);

/**
 * @brief Turns terminals into expression
 * @return Error code
 */
int reduce(TStack_element el1,TStack_element el2, TStack_element el3);

/**
 * @brief Turns tokens with hash and string into a number
 * @return Error code
 */
int hash();

/**
 * @brief Semantic check
 * @return Error code
 */
int check_id_and_type(TStack_element *el1, TStack_element *el2, TStack_element *el3, Token_type *return_type);