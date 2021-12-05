/**
* Project - Compiler for IFJ21
* 
* @brief Scanner for IFJ21 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file scanner.h
*
**/

#ifndef _SCANNER_H
#define _SCANNER_H


#include <stdio.h>
#include <stdlib.h>
#include "dyn_string.h"
#include <error.h>
#include <ctype.h>


/**
 * @enum Keywords
 * @brief Keywords that have a specific meaning and therefore must not occur as identifiers
 */

typedef enum
{
	KEYWORD_DO,
	KEYWORD_ELSE,
	KEYWORD_END,
	KEYWORD_FUNCTION,
    	KEYWORD_GLOBAL,
	KEYWORD_IF,
	KEYWORD_INTEGER,
	KEYWORD_LOCAL,
	KEYWORD_NIL,
	KEYWORD_NUMBER,
	KEYWORD_REQUIRE,
	KEYWORD_RETURN,
	KEYWORD_STRING,
	KEYWORD_THEN,
	KEYWORD_WHILE
}
    Keyword;


/**
 * @enum Token_types
 * @brief Token types, also used as states of finite automata
 */

typedef enum
{
    START,

    ASSIGN,                         //  =
    EG_ASSIGN,                      //  ~=
    MULTIPLY,                       //  *
    DIVIDE,                         //  /
    INTEGER_DIVIDE,                 //  //
    PLUS,                           //  +
    MINUS,                          //  -
    DOLLAR,                         // stack bottom

    LESS_THAN,                      //  <
    LESS_or_EQUALS,                 //  <=
    GREATER_THAN,                   //  >
    GREATER_or_EQUALS,              //  >=
    EQUALS,                         //  ==
    NOT_EQUAL,                      //  ~=

    LEFT_PARENTHESIS,               // (
    RIGHT_PARENTHESIS,              // )
  
    ID_or_KEYWORD,                  
    ID,        
    KEYWORD,

    INT,
    DECIMAL_NUMBER,
    NUMBER,
    DECIMAL_POINT,
    INDEX_CHAR,                     //  'E' or 'e'
    EXPONENT,
    EXPONENT_NUMBER,  
    EXPONENT_SIGN,                  //  + or -     


    BUILT_IN_READN,
    BUILT_IN_READI,
    BUILT_IN_READS,
    BUILT_IN_WRITE,
    BUILT_IN_SUBSTR,
    BUILT_IN_TOINTEGER,
    BUILT_IN_ORD,
    BUILT_IN_CHR,                      


    NIL,

    STRING,
    ESCAPE,

    LINE_COMMENTARY,                //  --
    BLOCK_COMMENTARY,               //  --[[ ]]

    HASH,                           //  # 
    COMMA,                          // ,
    COLON,                          // :
    DOT,                            // .
    STATE_EOF,


    CONCATENATE,                    // ..
    GET_LENGTH,                     // #


    ERROR = -1
}
    Token_type;



/**
 * @union Token_value
 * @brief Definition of token values - word (id), keyword, integer or float
 */
typedef struct
{
	Dyn_string string;
	Keyword keyword; 
	int integer_value;
    double decimal_value; 

} Token_value;



/**
 * @struct Token
 * @brief Definition of token with its value, type and other information
 */

typedef struct {
    Token_value value;
    Token_type type;
} Token;



/**
 * @brief Main function to get tokens
 * @param token token
 * @return function 'free_memory'
 */
int get_token(Token *token);

/**
 * @brief Function to set source for reading
 * @param FILE f
 * @return void
 */
void set_source(FILE *f) ;


/**
 * @brief Function to free allocated memory
 * @param int exit_code
 * @param Dyn_String string
 * @return exit_code
 */
int free_memory(int exit_code, Dyn_string *string);

/**
 * @brief Function to set token type to ID or KEYWORD
 * @param Dyn_string dynamic_sstring
 * @param Token token
 * @return Errors
 */
int identifier_check(Dyn_string * dynamic_string, Token *token);

#endif /* SCANNER_H*/
