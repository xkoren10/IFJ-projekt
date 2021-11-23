/**
 * @file symtable.h
 * @author xpyczl00
 * @brief header file for symtable.c
 * @version 0.1
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _symtable_
#define _symtable_

#include <stdbool.h>

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

typedef struct ht_item {
  char *key;            // kľúč prvku
  float value;          // hodnota prvku
  struct ht_item *next; // ukazateľ na ďalšie synonymum
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
void ht_insert(ht_table_t *table, char *key, float data);

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