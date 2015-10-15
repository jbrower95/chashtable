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
#define MIN_LOAD_FACTOR .4
#define MAX_LOAD_FACTOR 1.25

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


hashtable_t *hashtable_new(int initial_size);
void hashtable_destroy(hashtable_t *table);

void *hashtable_get(hashtable_t *table, char *key);
void hashtable_put(hashtable_t *table, char *key, void *value);
bool hashtable_remove(hashtable_t *table, char *key);
void hashtable_resize_if_necessary(hashtable_t *table);
void hashtable_resize(hashtable_t *table, int new_size);
int hashtable_size(hashtable_t *table);

bucket_t *bucket_new(char *key, void *datum, bucket_t *next, bucket_t *prev);
bool bucket_is_root(bucket_t *bucket);
void bucket_destroy(bucket_t *bucket);

int hash(char *key, int len);

#endif /* hashtable_h */
