/**
 *  Project - Compiler for IFJ21
 *
 * @brief Table of symbols for IFJ21 - semantic analysis
 *
 * @author Lukasz Pycz <xpyczl00>
 * @file symtable.h
 *
 **/

#ifndef _symtable_
#define _symtable_

#include <stdbool.h>
#include "scanner.h"
/**
 * @brief Table Size definition
 * 
 */

#define MAX_HT_SIZE 101
extern int HT_SIZE;

/**
 * @brief Table item definiton
 * 
 */

typedef struct func_val {
float var_val;
char* var_string;
char* var_name;
Token_type type;
struct func_val *next;
}func_val_t;




typedef struct ht_item {
  char *key;            // kľúč prvku, meno premennej alebo funkcie
  float var_value;
  char *string_val;       //hodnota premennej string
  Token_type var_type;          
  func_val_t *inval; //vstupne parametre obe budu heady
  func_val_t *outval;   //vystupne parametre
  struct ht_item *next; // ukazateľ na ďalšie synonymum nanic mi to je ale sak neham
  bool is_defined;
} ht_item_t;

/**
 * @brief Table item definiton
 * 
 */
typedef ht_item_t *ht_table_t[MAX_HT_SIZE];

/**
 * @brief Gives the given key an index from interval 0-HT_SIZE
 * 
 * @param key 
 * @return int 
 */
int get_hash(char *key);

/**
 * @brief Table initialization
 * 
 * @param table 
 */
void ht_init(ht_table_t *table);

/**
 * @brief Searching for an item from the table, if not found returns NULL
 * 
 * @param table 
 * @param key 
 * @return ht_item_t* 
 */
ht_item_t *ht_search(ht_table_t *table, char *key);

/**
 * @brief Inserting a value to a item with given key, if item already exists the value is replaced with new given
 * 
 * @param table 
 * @param key 
 * @param data 
 */
ht_item_t * ht_insert(ht_table_t *table, char *key);

/**
 * @brief Getting a pointer of the item with given key, if not found returns NULL
 * 
 * @param table 
 * @param key 
 * @return float* 
 */
float *ht_get(ht_table_t *table, char *key);

/**
 * @brief Deleting an item with given key
 * 
 * @param table 
 * @param key 
 */
void ht_delete(ht_table_t *table, char *key);

/**
 * @brief Disposing a whole table
 * 
 * @param table 
 */
void ht_delete_all(ht_table_t *table);

#endif