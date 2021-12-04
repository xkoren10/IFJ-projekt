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
    char* var;
    char* value_type;
    Values value;
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
 * @brief Generates instruction
 * @param type Instruction type
 * @param id_res Variable where the result will be stored
 * @param op_l First operator
 * @param op_r Second operator
 */
void gen_instruction(char* type, char* id_res, Symbol op_l, Symbol op_r);

/**
 * @brief Generates variable declaration
 * @param id Name of the variable
 */
void gen_var_def(char* id);

/**
 * @brief Generates setting of a variable with given type and value
 * @param var Name, value type and value of variable 
 */
void gen_var_valset(Symbol var);

/**
 * @brief Generates start of an user function
 * @param func_name hmmm
 */
void gen_function_start(char* func_name, char* args,char* arg_types, char* return_types, char* returns);// rework

/**
 * @brief Generates end of an user function
 */
void gen_function_end();

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
#endif