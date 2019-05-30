// Implement a Hash map 
// Define the basic struct of the map

#define INIT_SIZE 53
#define HT_PRIME_1 157
#define HT_PRIME_2 211

// Item
typedef struct {
  char* key;
  char* value;
} ht_item;


// Basic hash table
// size: the max number of elements can be stored
// count: the number of elements in the table
typedef struct {
  int base_size;
  int size;
  int count;
  ht_item** items;
} ht_hash_table;


// The hash table API
ht_hash_table* ht_new();
void ht_del_hash_table();
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);
