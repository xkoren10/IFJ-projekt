#ifndef _hash_table_
#define _hash_table_

#include <stdbool.h>

//Table Size
#define MAX_HT_SIZE 101
extern int HT_SIZE;

// Table Item
typedef struct ht_item {
  char *key;            // kľúč prvku
  float value;          // hodnota prvku
  struct ht_item *next; // ukazateľ na ďalšie synonymum
} ht_item_t;

// Table definition
typedef ht_item_t *ht_table_t[MAX_HT_SIZE];

//getting hash
int get_hash(char *key);
void ht_init(ht_table_t *table);
ht_item_t *ht_search(ht_table_t *table, char *key);
void ht_insert(ht_table_t *table, char *key, float data);
float *ht_get(ht_table_t *table, char *key);
void ht_delete(ht_table_t *table, char *key);
void ht_delete_all(ht_table_t *table);

#endif