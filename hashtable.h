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
#define MIN_LOAD_FACTOR .05
#define MAX_LOAD_FACTOR .95

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

void *hashtable_get(hashtable_t *table, char *key);
bool hashtable_put(hashtable_t *table, char *key, void *value);
bool hashtable_remove(hashtable_t *table, char *key);
int hashtable_size(hashtable_t *table);

#endif /* hashtable_h */
