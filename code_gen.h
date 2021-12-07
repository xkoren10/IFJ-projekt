/**
* Project - Compiler for IFJ21
* 
* @brief Code generator for IFJ21
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file code_gen.h
*
**/

#ifndef _CODE_GEN_H
#define _CODE_GEN_H

#include "parser.h"
/** @brief Iterator for ifs*/
unsigned if_i;
/** @brief Iterator for loops*/
unsigned loop_i;

/**
 * @union TStack
 * @brief Union of values used in Symbol
 */
typedef union{
    int integer;
    float number;
    char* string;
} Values;

/**
 * @struct TStack
 * @brief Structure used when function takes either variable or constant
 */
typedef struct{
    char* id;  // ID of variable
    char* value_type;   // Type of variable (string, float, integer, id, E)
    char* result_type;   // Type of returned expression
    Values value;   // Value (string, float or integer) of variable or string="%" for stack_pop
} Symbol;

/**
 * @brief Generates prologue of ifj21code file
 */
void gen_header();

/**
 * @brief Generates main function
 */
void gen_main();

/**
 * @brief Generates instruction with two operators and pushes the result to stack
 * @param type Instruction type
 * @param op_l First operator
 * @param op_r Second operator (value_type == "NONE" if second operand is not needed)
 */
void gen_instruction(char* type, Symbol op_l, Symbol op_r);

/**
 * @brief Generates variable declaration
 * @param id Name of the variable
 */
void gen_var_def(char* id);

/**
 * @brief Generates setting of a variable with given type and value
 * @param var Name, value type and value of variable 
 */
void gen_var_setval(Symbol var);
// MOVE <where> <what>  / MOVE <Symbol.var> <type@value>

/**
 * @brief Generates start of an user function
 * @param func_name hmmm
 */
void gen_function_start(char* func_name, func_val_t args, func_val_t returns); // TODO rework

/**
 * @brief Generates end of an user function
 */
void gen_function_end();

/**
 * @brief Generates call of an user function
 */
void gen_function_call(char* func_name);

/**
 * @brief Generates build-in function read
 * @param id The variable where data will be stored
 * @param type Type of incoming data
 */
void gen_read(char* id, char* type);

/**
 * @brief Generates build-in function write
 * @param var Variable or string to be printed
 */
void gen_write(Symbol var);

/**
 * @brief Generates start of a loop
 */
void gen_loop_start();

/**
 * @brief Generates end of a loop
 */
void gen_loop_end();

/**
 * @brief Generates start of IF
 * @param type Type of condition
 */
void gen_if_start(char* type);

/**
 * @brief Generates else branch of IF
 */
void gen_if_else();

/**
 * @brief Generates end of IF
 */
void gen_if_end();

/**
 * @brief Generates instructions for conditions 
 * @param type Instruction type
 * @param op_l First operator
 * @param op_r Second operator
 */
void gen_condition(char* type, Symbol op_l, Symbol op_r);

/**
 * @brief Generates instructions for conditions 
 * @param var Variable to substring
 * @param index_from Index from varible will be substringed
 * @param index_to Index where substring will end
 */
void gen_substring(Symbol var, int index_from, int index_to);

/**
 * @brief Generates build-in function for converting char to int
 * @param var Variable
 */
void gen_ordinal(Symbol var);

/**
 * @brief Generates build-in function for getting one char from string
 * @param var Variable
 * @param index Index of char
 */
void gen_chr(Symbol var, int index);

/**
 * @brief Generates build-in function for converting float/ number to integer
 * @param var Variable
 */
void gen_toINT(Symbol var);
#endif