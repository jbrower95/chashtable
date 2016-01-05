#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
/* *
 * A simple implementation of a chaining hashtable.
 * Used for holding pointers to generic pieces of data (literally anything).
 * */

//#define DEBUG_TABLE

static bucket_t *bucket_new(char *key, void *datum, bucket_t *next, bucket_t *prev);
static bool bucket_is_root(bucket_t *bucket);
static void bucket_destroy(bucket_t *bucket);
static void hashtable_resize_if_necessary(hashtable_t *table);
static void hashtable_resize(hashtable_t *table, int new_size);
static int hash(char *key, size_t len);
static void __hashtable_put(hashtable_t *table, char *key, void *value);
static void hashtable_insert(hashtable_t *table, char *key, void *value, bool resize);

/**
 * Initializes the hashtable.
 * Returns A hashtable pointer on success
 *         NULL o.w.
 * Every call to hashtable_init should have
 * a corresponding hashtable_destroy()
 */
hashtable_t *hashtable_new(size_t initial_size) {
    
    hashtable_t *table = (hashtable_t *)calloc(sizeof(hashtable_t), 1);
    
    if (table == NULL) {
        return NULL;
    }
    
    if (initial_size <= 0) {
        free(table);
        return NULL;
    }
    
    /* Perform initial allocation of buckets. */
    table->buckets = calloc(sizeof(bucket_t) * initial_size, 1);
    
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }
    
    table->size = initial_size;
    table->count = 0;
    
    /* Initialize to NULL.*/
    for (int i = 0; i < initial_size; i++) {
        table->buckets[i] = NULL;
    }
    
    return table;
}


void hashtable_destroy(hashtable_t *table) {
    for (int i = 0; i < table->size; i++) {
        // Loop through all the buckets, and delete them.
        if (table->buckets[i] != NULL) {
            bucket_t *current = table->buckets[i];
            do {
                bucket_destroy(current);
            } while ((current = current->next) != NULL);
            table->buckets[i] = NULL;
        }
    }
}



/**
 * Initializes a bucket struct. Returns NULL if the bucket couldn't be created.
 */
static bucket_t *bucket_new(char *key, void *datum, bucket_t *next, bucket_t *prev) {
    
    bucket_t *bucket = (bucket_t *)malloc(sizeof(bucket_t));
    
    if (bucket == NULL) {
        // Out of memory.
        return NULL;
    }

    bucket->datum = datum;
    
    bucket->key = malloc(sizeof(char) * (strlen(key) + 1));

    if (bucket->key == NULL) {
        // Out of memory.
        free(bucket);
        return NULL;
    }

    bucket->datum = datum;
    
    memcpy(bucket->key, key, strlen(key) + 1);
    
    // Store the key on the heap since we're referring to an arbitrarily
    // size(d string as the key.
    
    // The datum doesn't need to be stored on the heap since we're mandating
    // point(ers. (similarly, next & prev as well).
    bucket->next = next;
    bucket->prev = prev;
    
    return bucket;
}

// Returns true if this bucket is the first bucket in the linked list.
static bool bucket_is_root(bucket_t *bucket) {
    return (bucket->prev == NULL);
}

// Destroys a bucket -- Frees all allocated memory, and removes the bucket from its linked list.
static void bucket_destroy(bucket_t *bucket) {
    // free what we malloced((
    free(bucket->key);
    
    // remove from the list((((((((((((((
    bucket_t *previous = bucket->prev;
    bucket_t *next = bucket->next;
    
    if (previous != NULL) {
        previous->next = next;
    }
    
    if (next != NULL) {
        next->prev = previous;
    }
    
    free(bucket);
}

/**
 * Returns the value associated with a key from the hashtable.
 */
void *hashtable_get(hashtable_t *table, char *key) {
    if (key == NULL) {
        return NULL;
    }
    
    const int hash_value = hash(key, strlen(key));
    int bucket = hash_value % table->size;
    
    if (table->buckets[bucket] == NULL) {
        return NULL;
    } else {
        
        volatile bucket_t *current = table->buckets[bucket];
        
        do {
            if (strcmp(current->key, key) == 0) {
                return current->datum;
            }
        } while ((current = current->next) != NULL);
        
        return NULL;
    }
}

/**
 * Removes the key, value pair from the hashtable.
 * Returns true if the item was removed, o.w false if it wasn't in the hashtable.
 */
bool hashtable_remove(hashtable_t *table, char *key) {
    
    if (key == NULL) {
        return NULL;
    }
    
    const int hash_value = hash(key, strlen(key));
    int bucket = hash_value % table->size;
    
    if (table->buckets[bucket] == NULL) {
        return false;
    } else {
        
        bucket_t *current = table->buckets[bucket];
        
        do {
            if (strcmp(current->key, key) == 0) {
                break;
            }
        } while ((current = current->next) != NULL);
        
        if (current != NULL) {
            // We found it.
            if (bucket_is_root(current)) {
                // Reset the root
                table->buckets[bucket] = current->next;
            }
            // Destroy the node
            bucket_destroy(current);
            table->count--;
            hashtable_resize_if_necessary(table);
            return true;
        } else {
            return false;
        }
    }
}

static void hashtable_resize(hashtable_t *table, int new_size) {
    
    /* Resize the internal array to new_size */
    bucket_t **original_array = table->buckets;
    const int old_size = table->size;
    
    /* Reset the attributes of this table to new stuff */
    table->buckets = (bucket_t **)calloc(sizeof(bucket_t) * new_size, 1);
    table->size = new_size;
    table->count = 0;
    
    /* Re insert all of the existing buckets */
    for (volatile int i = 0; i < old_size; i++) {
        bucket_t *current = original_array[i];
        if (current != NULL) {
            /* Insert all of the values in these buckets. */
            do {
                #ifdef DEBUG_TABLE
                    printf("Inserting (%s, %p)\n", current->key, current->datum);
                #endif
                hashtable_put(table, current->key, current->datum);
                bucket_t *next = current->next;
                bucket_destroy(current);
                current = next;
            } while (current != NULL);
        }
    }
    
    /* Free our original representation */
    free(original_array);
}

/* Internal insertion method to get past*/
static void hashtable_insert(hashtable_t *table, char *key, void *value, bool resize) {
    __hashtable_put(table, key, value);
    if (resize) {
        hashtable_resize_if_necessary(table);
    }
}

void hashtable_put(hashtable_t *table, char *key, void *value) {
    hashtable_insert(table, key, value, true);
}

/**
 * Inserts a value into the hashtable.
 */
static void __hashtable_put(hashtable_t *table, char *key, void *value) {
    
    if (key == NULL) {
        return;
    }
    
    const int hash_value = hash(key, strlen(key));
    const int bucket = hash_value % table->size;
    
    if (table->buckets[bucket] == NULL) {
        #ifdef DEBUG_TABLE
            printf("Insert: Added new root bucket (slot %d).\n", bucket);
        #endif
        table->buckets[bucket] = bucket_new(key, value, NULL, NULL);;
    } else {
        bucket_t *current = table->buckets[bucket];
        while (current->next != NULL) {
            
            if (strcmp(current->key, key) == 0) {
                // We found our key: this becomes an update.
                #ifdef DEBUG_TABLE
                printf("Insert: Performed update.\n");
                #endif
                current->datum = value;
                return;
            }
            
            #ifdef DEBUG_TABLE
            printf("Insert: Added new non-root bucket.");
            #endif
            current = current->next;
        }
        
        // We need to append onto current -- we got to the end of the list.
        current->next = bucket_new(key, value, current, NULL);
    }
    
    table->count++;
}


static void hashtable_resize_if_necessary(hashtable_t *table) {
    
    const float load_factor = table->count / (float)table->size;
    
    if (load_factor > MAX_LOAD_FACTOR) {
        // We need to ~grow~. Double our size.
        #ifdef DEBUG_TABLE 
            printf("Growing hash table from %d to %d.", table->size, table->size*2);
        #endif
        hashtable_resize(table, table->size * 2);
    } else if (load_factor < MIN_LOAD_FACTOR && table->size > MIN_SIZE) {
        // We need to ~shrink~. Half our size.
        int new_size = (table->size / 2) < MIN_SIZE ? MIN_SIZE : (table->size / 2);
        #ifdef DEBUG_TABLE 
            printf("Shrinking hash table from %d to %d\n.", table->size, new_size);
        #endif
        hashtable_resize(table, new_size);
    }
}


/* Returns the number of K,V pairs in the hashtable. */
int hashtable_size(hashtable_t *table) {
    return table->count;
}

/**
 * A simple string hash function. Returns an integer s.t x >= 0
 */
static int hash(char *key, size_t len) {
    int hash, i;
    for(hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return abs(hash);
}






