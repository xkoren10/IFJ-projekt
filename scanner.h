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
//#include <dyn_string.h>


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
 * @brief Token types, also used as states of finite automata.
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
    

    LESS_THAN,                      //  <
    LESS_or_EQUALS,                 //  <=
    GREATER_THAN,                   //  >
    GREATER_or_EQUALS,              //  >=
    EQUALS,                         //  ==

    LEFT_PARENTHESIS,               // (
    RIGHT_PARENTHESIS,              // )
  
    ID_or_KEYWORD,                  
    ID,        
    KEYWORD,

    INT,
    NUMBER,
    INDEX_CHAR,                     //  'E' or 'e'
    EXPONENT,
    EXPONENT_SIGN,                  //  + or -                           

    NIL,

    STRING,
    ESCAPE,

    LINE_COMMENTARY,                //  --
    BLOCK_COMMENTARY_START,         //  --[[
    BLOCK_COMMENTARY_END,           //  ]]

    HASH,                           //  #
    DOUBLEDOT,                      // ..
    COMMA,                          // ,
    COLON,                          // :

    EOL,
    STATE_EOF,

    ERROR = -1
}
    Token_type;



/**
 * @struct Token
 * @brief Definition of token with its value, type and other information
 */

typedef struct {
    //dyn_string value;//                                                     -- needs header and .c file to define actual value from stdin 
    Token_type type;
    int line, lenght, start, end;
} Token;





/**
 * @brief Main function to get tokens
 * @return -something-
 */
Token get_token();

#endif /* SCANNER_H*/