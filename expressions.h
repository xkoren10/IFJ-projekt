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

/**
 * @brief Initialization and start of the expresion analysis
 * @param token Structure Token
 * @return Error code
 */
int expression_analysis(Token *token);

/**
 * @brief Expresion analysis
 * @return Error code
 */
int analysis();

/**
 * @brief Skips EOL tokens
 * @return Error code
 */
int eol();

/**
 * @brief Finds index in the precedence table
 * @param i1 pointer to return index of row
 * @param token pointer to return index of column
 * @return Error code
 */
int find_index(int *i1,int *i2);