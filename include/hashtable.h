//
//  hashtable.h
//  
//
//  Created by Justin Brower on 10/9/15.
//  A simple chaining hashtable.
//

#ifndef hashtable_h
#define hashtable_h

#include <stdbool.h>

#define MIN_SIZE 8
#define MIN_LOAD_FACTOR .1
#define MAX_LOAD_FACTOR .8

/* A bucket in the hashtable -- stores a datum and forms a linked list.*/
typedef struct bucket {
    void *datum;
    char *key;
    struct bucket *next;
    struct bucket *prev;
} bucket_t;


/* The hashtable */
typedef struct hashtable {
    
    /* The buckets of the hashtable. These will be pointers. */
    bucket_t **buckets;
    
    /* The size of the internal representation of the hash table. The len(buckets) */
    int size;
    
    /* The total number of elements in the hash table.*/
    int count;
    
} hashtable_t;


hashtable_t *hashtable_new(size_t initial_size);
void hashtable_destroy(hashtable_t *table);

/* Returns the data stored at that key in the table. NULL if non-existent or error.*/
void *hashtable_get(hashtable_t *table, char *key);

/* Stores a key,value pair or overwrites. Returns true upon success, otherwise false. */
// False indiciates an out of memory error.
bool hashtable_put(hashtable_t *table, char *key, void *value);

/* Removes an item from the hashtable. Returns true if the item was found and removed. */
bool hashtable_remove(hashtable_t *table, char *key);

/* Returns the number of key value pairs in the hashtable. */
int hashtable_size(hashtable_t *table);

#endif /* hashtable_h */
