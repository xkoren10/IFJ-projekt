#include "scanner.h"

char next_char;
int state = START;


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
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MUL;
        case '(':
            return L_PAR;
        case ')':
            return R_PAR;
        case ',':
            return COMMA;
        case ';':
            return SEMICOL;
        case '{':
            return L_BR;
        case '}':
            return R_BR;
        case '\n':
            return EOL;
        case EOF:
            return ENDFILE;



        case '/':
            state = S_DIV;
            break;
        case '=':
            state = S_ASSIGN;
            break;
        case '<':
            state = S_L;
            break;
        case '>':
            state = S_G;
            break;
        case '!':
            state = S_NEG;
            break;
        case ':':
            state = S_VAR_DEF;
            break;
        }
    }
}
    return 0;
}
