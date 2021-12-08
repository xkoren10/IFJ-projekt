/**
* Project - Compiler for IFJ21
* 
* @brief Code generator for IFJ21
* 
* @author Matej Hložek <xhloze02>
* @file code_gen.c
*
**/

#include "code_gen.h"
#include "scanner.h"

unsigned if_i = 0;
unsigned loop_i = 0;

void print_push(Symbol op){
    fprintf(stdout, "PUSHS ");
    if ( strcmp(op.value_type, "integer")==0 ){
        fprintf(stdout, "int@%d\n", op.value.integer);
    }
    else if ( strcmp(op.value_type, "float")==0 ){
        fprintf(stdout, "float@%f\n", op.value.number);  
    }
    else if ( strcmp(op.value_type, "string")==0 ){
        fprintf(stdout, "string@%s\n", op.value.string);
    }
    else if ( strcmp(op.value_type, "id")==0 ){
        fprintf(stdout, "LF@%s\n", op.id);
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
    if ( strcmp(op_l.value_type, "E")!=0 ){
        print_push(op_l);
    }
    // Second operand PUSH to stack if it exists
    if ( strcmp(op_r.value_type, "E")!=0 ){
        print_push(op_r);
    }
    // Print operation
    print_type(type);
}

void gen_var_def(char* id){
    fprintf(stdout, "DEFVAR LF@%s\n", id);
}

void gen_var_setval(Symbol var){
    if ( strcmp(var.value_type, "E")==0){
        fprintf(stdout, "POPS LF@%s\n", var.id);

    }
    else if ( strcmp(var.value_type, "integer")==0 ){
        fprintf(stdout, "MOVE LF@%s %s@%d\n", var.id, var.value_type, var.value.integer);
        }
    else if ( strcmp(var.value_type, "float")== 0 ){
        fprintf(stdout, "MOVE LF@%s %s@%f\n", var.id, var.value_type, var.value.number);
        }
    else if ( strcmp(var.value_type, "string")==0 ){
        fprintf(stdout, "MOVE LF@%s %s@%s\n", var.id, var.value_type, var.value.string);
        }

    else{
        fprintf(stdout, "MOVE LF@%s LF@%s\n", var.id, var.value_type);
    }
    
}

void gen_function_start(char* func_name,func_val_t returns){
    fprintf(stdout, "LABEL $%s\n", func_name);
    fprintf(stdout, "PUSHFRAME\n");
    func_val_t *iter = &returns;
    while(iter!=NULL){
        fprintf(stdout, "DEFVAR LF@%s\n",iter ->var_name);
        fprintf(stdout, "MOVEVAR LF@%s nil@nil\n", iter->var_name);
        iter = iter->next;
    }
    
}

void gen_function_end(){
    fprintf(stdout, "POPSTACK\n");
    fprintf(stdout, "RETURN\n");
}

void gen_function_call(char* func_name, func_val_t args, func_val_t returns){
    func_val_t* iter;
    iter = args.next;
    do{
        fprintf(stdout, "DEFVAR LF@%s\n", iter->var_name);
        fprintf(stdout, "MOVEVAR LF@%s ", iter->var_name);
        if ( iter->type == ID ){
            fprintf(stdout, "LF@%s\n", iter->var_string);
        }
        else if ( iter->type == INT){

            fprintf(stdout, "integer@%.0f\n", iter->var_val);   

        }
        else if ( iter->type == NUMBER ){
            fprintf(stdout, "float@%a\n", iter->var_val);
        }
        else if ( iter->type == STRING ){
            fprintf(stdout, "string@%s\n", iter->var_string);
        }
        else{
            fprintf(stderr, "\n!\nGEN FUNC CALL - WRONG TYPE\n!\n");
            return;
        }
        iter = returns.next;
    }
    while(iter!=NULL);
    fprintf(stdout, "CALL $%s\n", func_name);

}


void gen_condition(char* type, Symbol op_l, Symbol op_r){
    if ( strcmp(op_l.value_type, "E")!=0 ){
        print_push(op_l);
    }
    if ( strcmp(op_r.value_type, "E")!=0 ){
        print_push(op_r);
    } 

    if ( strcmp(type, ">")==0 ){
        fprintf(stdout, "GTS\n");
    }
    else if ( strcmp(type, "<")==0 ){
        fprintf(stdout, "LTS\n");
    }
    else if ( strcmp(type, "==")==0 ){
        fprintf(stdout, "EQS\n");
    }
    else if ( strcmp(type, "~=")==0 ){
        fprintf(stdout, "EQS\n");
        fprintf(stdout, "NOTS\n");
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



void gen_header(){
    fprintf(stdout, ".IFJcode21\n");
    fprintf(stdout, "DEFVAR GF@RESULT_L\n");
    fprintf(stdout, "DEFVAR GF@RESULT_R\n");
    fprintf(stdout, "DEFVAR GF@EXP_L\n");
    fprintf(stdout, "DEFVAR GF@EXP_R\n");
    fprintf(stdout, "DEFVAR GF@VARstring\n");
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

void gen_if_start(){
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

void gen_chr(Symbol var, Symbol index){
    fprintf(stdout, "PUSHS GF@VARstring\n");
    if ( strcmp(var.value_type, "id")==0 ){
        fprintf(stdout, "GETCHAR GF@VARstring LF@%s ", var.id);
        if ( strcmp(index.value_type,"id")==0 ){
            fprintf(stdout, "LF@%s\n", index.id);
        }
        else if ( strcmp(index.value_type, "integer")==0 ){
            fprintf(stdout, "integer@%d\n", index.value.integer);
        }
        else{
            fprintf(stderr, "\n!\nGEN_CHR - ID - WRONG TYPE\n!\n");
            return;
        }
    }
    else if ( strcmp(var.value_type, "string")==0 ){
        fprintf(stdout, "GETCHAR GF@VARstring LF@%s ", var.value.string);
        if ( strcmp(index.value_type,"id")==0 ){
            fprintf(stdout, "LF@%s\n", index.id);
        }
        else if ( strcmp(index.value_type, "integer")==0 ){
            fprintf(stdout, "integer@%d\n", index.value.integer);
        }
        else{
            fprintf(stderr, "\n!\nGEN_CHR - STRING - WRONG TYPE\n!\n");
            return;
        }
    }
    fprintf(stdout, "PUSHS GF@VARstring\n");
}

void gen_ordinal(Symbol var, Symbol index){

    gen_chr(var, index);
    fprintf(stdout, "STR2INTS\n");
}

void gen_toINT(Symbol var){
    if ( strcmp(var.value_type, "id")==0 ){
        fprintf(stdout, "PUSHS LF@%s\n", var.id);
        fprintf(stdout, "FLOAT2INTS\n");
    }
    else if ( strcmp(var.value_type,"number")==0 ){
        fprintf(stdout, "PUSHS %a\n", var.value.number);
        fprintf(stdout, "FLOAT2INTS\n");
    }
    else{
        fprintf(stderr, "\n!\nTO_INT - WRONG TYPE\n!\n");
        return;
    }   
}



void gen_substring(Symbol var, Symbol index_from, Symbol index_to){
    
    fprintf(stdout, "%s\n", var.value_type);
    fprintf(stdout, "%s\n", index_from.value_type);
    fprintf(stdout, "%s\n", index_to.value_type);
}

