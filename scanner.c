/**
* Project - Compiler for IFJ21
* 
* @brief Scanner for IFJ21 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file scanner.c
*
**/

#include "scanner.h"

char next_char;
int state = START;


/*
Fixed enough to be without errors. 
In special cases we need to check for the succeeding character to determine token type 
Function to choose keywords would be useful
*/

int main() {

while (1) {

    next_char = getc(stdin);

    switch (state) {

    case START:


        if (isspace(next_char)) {
            state = START;
            if (next_char != '\n' && next_char != EOF)
                break;
        }



        switch (next_char) {

        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '*':
            return MULTIPLY;
        case '(':
            return LEFT_PARENTHESIS;
        case ')':
            return RIGHT_PARENTHESIS;
        case ',':
            return COMMA;
        case '\n':
            return EOL;
        case EOF:
            return STATE_EOF;



        case '/':
            state = DIVIDE;
            break;
        case '=':
            state = ASSIGN;
            break;
        case '<':
            state = LESS_THAN;
            break;
        case '>':
            state = GREATER_THAN;
            break;
        case ':':
            state = COLON;
            break;
        }
    }
}
    return 0;
}
