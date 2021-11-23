/**
 * @file symtable.c
 * @author xpyczl00
 * @brief Symtable using hashtable
 * @version 0.1
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "symtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/**
 * @brief Gives the given key an index from interval 0-HT_SIZE
 * 
 * @param key 
 * @return int 
 */
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
  {
    result += key[i];
  }
  return (result % HT_SIZE);
}


/**
 * @brief Table initialization
 * 
 * @param table 
 */
void ht_init(ht_table_t *table)
{
  for (int i = 0; i < HT_SIZE; i++)
  {
    (*table)[i] = NULL;
  }
}
/**
 * @brief Searching for an item from the table, if not found returns NULL
 * 
 * @param table 
 * @param key 
 * @return ht_item_t* 
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  //colision
  ht_item_t *searched = (*table)[get_hash(key)];
  while (searched)
  {
    if (strcmp(key, searched->key) == 0)
      return searched;
    searched = searched->next;
  }

  return NULL;
}

/**
 * @brief Inserting a value to a item with given key, if item already exists the value is replaced with new given
 * 
 * @param table 
 * @param key 
 * @param value 
 */
void ht_insert(ht_table_t *table, char *key, float value)
{

  ht_item_t *insert = ht_search(table, key);
  if (insert)
  {
    insert->value = value;
    return;
  }
  else
  {
    insert = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (!insert)
      return;
    insert->key = key;
    insert->value = value;
    insert->next = NULL;
    int hash_index = get_hash(key);

    insert->next = (*table)[hash_index];
    (*table)[hash_index] = insert;
  }
}

/**
 * @brief Getting a pointer of the item with given key, if not found returns NULL
 * 
 * @param table 
 * @param key 
 * @return float* 
 */
float *ht_get(ht_table_t *table, char *key)
{
  ht_item_t *get = ht_search(table, key);

  return get ? &(get->value) : NULL;
}

/**
 * @brief Deleting an item with given key
 * 
 * @param table 
 * @param key 
 */
void ht_delete(ht_table_t *table, char *key)
{
  int hash_index = get_hash(key);
  ht_item_t *tmp = (*table)[hash_index];
  ht_item_t *before = NULL, *after = NULL;

  while (tmp)
  {
    after = tmp->next;

    if (strcmp(key, tmp->key) == 0)
    {
      free(tmp);
      if (!before)
      {
        (*table)[hash_index] = after;
        return;
      }
      before->next = after;
    }

    before = tmp;
    tmp = tmp->next;
  }
  return;
}

/**
 * @brief Disposing a whole table
 * 
 * @param table 
 */
void ht_delete_all(ht_table_t *table)
{
  ht_item_t *tmp, *delete;
  for (int i = 0; i < HT_SIZE; i++)
  {
    tmp = (*table)[i];
    while (tmp)
    {
      delete = tmp;
      tmp = tmp->next;
      free(delete);
    }
    (*table)[i] = NULL;
  }
}
