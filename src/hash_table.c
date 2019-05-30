// Implment an open-address, double hashed hash map
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "prime.h"


#include "hash_table.h"

/////////////////////////////static utils///////////////////////////////////

// The DELETED flag element use in the delete. The element cannot be simplely 
// deleted with an open address. So the deleted element are point to this.
static ht_item HT_DELETED_ITEM = {NULL, NULL};


// Initialize the hash item, the function is only use internally
static ht_item* ht_new_item(const char* k, const char* v) {
  ht_item* item = (ht_item*)malloc(sizeof(ht_item));
  item->key = strdup(k);
  item->value = strdup(v);
  return item;
}


// Delete item from a hash table
static void ht_del_item(ht_item* i) {
  free(i->key);
  free(i->value);
  free(i);
}


// Hash function: convert every char into a number and get the mod of `m`
// of the sum. `a` should be a prime number and larger than the alphabet.
//     a > 128 for the ASCII case.`m` is the number of buckets.
static int ht_hash(const char* s, const int a, const int m) {
  long long hash = 0;
  const int len_s = strlen(s);
  for (int i = 0; i < len_s; i++) {
    hash += (long long)pow(a, len_s - (i+1)) * s[i];
    hash = hash % m;
  }
  return (int)hash;
}


// Double hash to handle collisions.
// index = hash(string) + i * (hash(string) + 1) % num_bucket
// `i` indicates the collesions times
static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}


/***
 * Compute the usage ratio: count / size
* resize up if ratio > 0.7
* resize down if ratio < 0.1
**/

// sized new
static ht_hash_table* ht_new_sized(const int base_size) {
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));
  ht->base_size = base_size;

  ht->size = next_prime(base_size);

  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

// resize the hash map according to the rules above
static void ht_resize(ht_hash_table* ht, const int base_size) {
  if (base_size < ht->base_size) {
    return;
  }
  ht_hash_table* new_ht = ht_new_sized(base_size);
  // Copy old to new
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_insert(new_ht, item->key, item->value);
    }
  }

  ht->base_size = new_ht->base_size;
  ht->count = new_ht->count;

  // TO delete the new_ht, change size and items
  const int tmp_size = ht->size;
  ht->size = new_ht->size;
  new_ht->size = tmp_size;

  ht_item** tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_del_hash_table(new_ht);
}

// Resize down 
static void ht_size_down(ht_hash_table* ht) {
  const int size = ht->base_size / 2;
  ht_resize(ht, size);
}

static void ht_size_up(ht_hash_table* ht) {
  const int size = ht->base_size * 2;
  ht_resize(ht, size);
}



//////////////////////////////API/////////////////////////////////////////

// Initialize the hash map with a defined size, it can be expended
// with `resize`. Every bucket will be filled with `NULL` bytes, which 
// indicates that the bucket is empty
ht_hash_table* ht_new() {
  return ht_new_sized(INIT_SIZE);
}


// Delete a hash table
void ht_del_hash_table(ht_hash_table* ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}


// Insert a (k, v) into a ht
// In order to update of one item's value, when meet into the same key
// del the old element and insert new one
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
  // Check if need to resize
  const int load = ht->count * 100 / ht->size;
  if (load > 70) {
    ht_size_up(ht);
  }
  
  ht_item* item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);

  //Deal with collisions
  ht_item* cur_item = ht->items[index];
  int i = 1;
  // The item can be insert into the deleted position
  while (cur_item != NULL) {
    // Update
    if (cur_item != &HT_DELETED_ITEM) {
      if (strcmp(cur_item->key, key) == 0) {
        ht_del_item(cur_item);
        ht->items[index] = item;
        return ;
      }
    }

    index = ht_get_hash(item->key, ht->size, i);
    cur_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  ht->count++;
}


// Search the value according key
char* ht_search(ht_hash_table* ht, const char* key) {
  int index = ht_get_hash(key, ht->size, 0);
  int i = 1;
  ht_item* item = ht->items[index];

  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        return item->value;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  return NULL;
}


// Delete element from the hash table
void ht_delete(ht_hash_table* ht, const char* key) {
  const int load = ht->count * 100 / ht->size;
  if (load < 10) {
    ht_size_down(ht);
  }

  int index = ht_get_hash(key, ht->size, 0);
  ht_item* item = ht->items[index];
  int i = 1;

  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        ht_del_item(item);
        ht->items[index] = &HT_DELETED_ITEM;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  ht->count--;
}



