/**
 * Project: Compiler for IFJ21
 *
 * @brief Main file 
 *
* @author Matej Koreň <xkoren10
 * @file ifj21_compiler.c
 */


#include <stdio.h>
#include <stdlib.h>

#include "parser.h"



//#define DEBUG 1


/**
 * Main function.
 *
 * @return EXIT_SUCCESS (0), if compilation was seccessful, appropriate error code otherwise.
 */
int main()
{
	FILE* source_file;
	source_file = stdin;

	set_source(source_file);

	int output;
	output = parse();


	return output;
}