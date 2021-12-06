/**
* Project - Compiler for IFJ21
* 
* @brief Code generator for IFJ21
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file code_gen.c
*
**/

#include "code_gen.h"
#include "parser.h"
#include "parser.h"

void print_push(Symbol op){
    fprintf(stdout, "PUSHS");
    if ( strcmp(op.value_type, "integer")==0 ){
        fprintf(stdout, "int@%d\n", op.value.integer);
    }
    else if( strcmp(op.value_type, "float")==0 ){
        fprintf(stdout, "float@%f\n", op.value.number);  // float to string? how many digits?
    }
    else if( strcmp(op.value_type, "string")==0 ){
        fprintf(stdout, "string@%s\n", op.value.string);
    }
    else{
        fprintf(stderr, "\n!\nPUSHS ERROR - WRONG INPUT VAR TYPE\n!\n");
        return;
    }
}

void print_type(char* type){
    if ( strcmp(type, "+")==0 ){
        fprintf(stdout, "ADDS\n");
    }
    else if ( strcmp(type, "-")==0 ){
        fprintf(stdout, "SUBS\n");
    }
    else if ( strcmp(type, "*")==0 ){
        fprintf(stdout, "MULS\n");
    }
    else if ( strcmp(type, "//")==0 ){
        fprintf(stdout, "IDIVS\n");
    }
    else if ( strcmp(type, "/")==0 ){
        fprintf(stdout, "DIVS\n");
    }
    else if ( strcmp(type, "..")==0 ){
        fprintf(stdout, "CONCAT\n");
    }
    else if ( strcmp(type, "&")==0 ){
        fprintf(stdout, "ANDS\n");
    }
    else if ( strcmp(type, "|")==0 ){
        fprintf(stdout, "ORS\n");
    }
    else if ( strcmp(type, "!")==0 ){
        fprintf(stdout, "NOTS\n");
    }
    else{
        fprintf(stderr, "\n!\nTYPE ERROR - WRONG OP TYPE\n!\n");
        return;
    }
}
void gen_instruction(char* type, Symbol op_l, Symbol op_r){
    // First operand PUSH to stack
    print_push(op_l);
    // Second operand PUSH to stack if it exists
    if ( strcmp(op_r.value_type, "E")!=0 ) print_push(op_r);
    // Print operation
    print_type(type);
}

void gen_var_def(char* id){
    fprintf(stdout, "DEFVAR LF@%s\n", id);
}

void gen_var_setval(Symbol var){
    if ( strcmp(var.value.string, "%%")==0 && var.id!=NULL){
        if ( strcmp(var.value_type, "integer")==0 ){
            fprintf(stdout, "POPS LF@%s\n", var.id);
        }
        else if ( strcmp(var.value_type, "float")==0 ){
            fprintf(stdout, "POPS LF@%s\n", var.id);
        }
        else if ( strcmp(var.value_type, "string")==0 ){
            fprintf(stdout, "POPS LF@%s\n", var.id);
        }
        else{
            fprintf(stderr, "\n!\nSETVAL ERROR - STACK - WRONG TYPE\n!\n");
            return;
        }
    }
    else{
        if ( strcmp(var.value_type, "integer")==0 ){
            fprintf(stdout, "MOVE LF@%s %s@%d\n", var.id, var.value_type, var.value.integer);
        }
        else if ( strcmp(var.value_type, "float")== 0 ){
            fprintf(stdout, "MOVE LF@%s %s@%f\n", var.id, var.value_type, var.value.number);
        }
        else if ( strcmp(var.value_type, "string")==0 ){
            fprintf(stdout, "MOVE LF@%s %s@%s\n", var.id, var.value_type, var.value.string);
        }
        else{
            fprintf(stderr, "\n!\nSETVAL ERROR - NONSTACK - WRONG TYPE\n!\n");
            return;
        }
        
        
    }
}

/* void gen_function_start(){
    fprintf(stdout, "LABEL $%s\n", func_name);
    fprintf(stdout, "PUSHFRAME\n");
    fprintf(stdout, "\n");
} */

void gen_function_end(){
    fprintf(stdout, "POPSTACK\n");
    fprintf(stdout, "RETURN\n");
}

void gen_function_call(char* func_name){
    fprintf(stdout, "CALL $%s\n", func_name);
}

// TO CHECK

void gen_condition(char* type, Symbol op_l, Symbol op_r){
    if ( strcmp(type, ">")==0 ){
        fprintf(stdout, "GTS\n");
    }
    else if ( strcmp(type, "<")==0 ){
        fprintf(stdout, "LTS\n");
    }
    else if ( strcmp(type, "=")==0 ){
        fprintf(stdout, "EQS\n");
    }
    else if ( strcmp(type, ">=")==0 ){
        fprintf(stdout, "POPS GF@EXP_L\n");
        fprintf(stdout, "POPS GF@EXP_R\n");
        fprintf(stdout, "EQ GF@RESULT_L GF@EXP_L GF@EXP_R\n");
        fprintf(stdout, "GT GF@RESULT_R GF@EXP_L GF@EXP_R\n");
        fprintf(stdout, "OR GF@RESULT_L GF@RESULT_L GF@RESULT_R\n");
        fprintf(stdout, "PUSHS GF@RESULT_L\n");
    }
    else if ( strcmp(type, "<=")==0 ){
        fprintf(stdout, "POPS GF@EXP_L\n");
        fprintf(stdout, "POPS GF@EXP_R\n");
        fprintf(stdout, "EQ GF@RESULT_L GF@EXP_L GF@EXP_R\n");
        fprintf(stdout, "LT GF@RESULT_R GF@EXP_L GF@EXP_R\n");
        fprintf(stdout, "OR GF@RESULT_L GF@RESULT_L GF@RESULT_R\n");
        fprintf(stdout, "PUSHS GF@RESULT_L\n");
    }
    else{
        fprintf(stderr, "\n!\nPRINT CONDITION ERROR - WRONG TYPE\n!\n");
        return;
    }
}

// DOLU HOTOVKA

void gen_header(){
    fprintf(stdout, ".IFJcode21\n");
    fprintf(stdout, "DEFVAR GF@RESULT_L\n");
    fprintf(stdout, "DEFVAR GF@RESULT_R\n");
    fprintf(stdout, "DEFVAR GF@EXP_L\n");
    fprintf(stdout, "DEFVAR GF@EXP_R\n");
    fprintf(stdout, "JUMP $$MAIN\n");
}

void gen_main(){
    fprintf(stdout, "LABEL $$MAIN\n");
}

void gen_loop_start(){
    fprintf(stdout, "JUMPIFNEQS $END_LOOP%d\n", loop_i);
    fprintf(stdout, "LABEL $LOOP%d\n", loop_i);
}

void gen_loop_end(){
    fprintf(stdout, "JUMP $LOOP%d\n", loop_i);
    fprintf(stdout, "LABEL $END_LOOP%d\n", loop_i);
    loop_i++;
}

void gen_if_start(char* type){
    fprintf(stdout, "JUMPIFEQS $IF%d\n", if_i);
    fprintf(stdout, "JUMPIFNEQS $ELSE%d\n", if_i);
    fprintf(stdout, "LABEL $IF%i\n", if_i);
}

void gen_if_else(){
    fprintf(stdout, "JUMP $END_IF%d\n", if_i);
    fprintf(stdout, "LABEL $ELSE%d\n", if_i);
}

void gen_if_end(){
    fprintf(stdout, "LABEL $END_IF%d\n", if_i);
    if_i++;
}

void gen_write(Symbol var){
    if ( strcmp(var.value_type, "string")==0 ){
        fprintf(stdout, "WRITE string@%s\n", var.value.string);
    }
    else if( strcmp(var.value_type, "id")==0 ){
        fprintf(stdout, "WRITE LF@%s\n", var.id);
    }
    else{
        fprintf(stderr, "\n!\nWRITE ERROR - WRONG TYPE\n!\n");
        return;
    }
}

void gen_read(char* id, char* type){
    fprintf(stdout, "READ LF@%s %s\n", id, type);
}