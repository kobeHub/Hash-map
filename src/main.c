#include <stdio.h>

#include "hash_table.h"

int main() {
  ht_hash_table* ht = ht_new();
  char* keys[4] = {"just", "12", "ds"};
  ht_insert(ht, "just", "1");
  ht_insert(ht, "12", "123");
  ht_insert(ht, "ds", "test");

  for (int i = 0; i < 3; i++)
    printf("Key: %s, element: %s\n", keys[i], ht_search(ht, keys[i]));
  printf("Update for `just`: %s\n", "wooo");
  ht_insert(ht, "just", "wooo");
  ht_delete(ht, "12");
  printf("Deleted `12`: %s\n", ht_search(ht, "12"));

  ht_del_hash_table(ht);
}
