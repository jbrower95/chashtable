#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

const char *get_truth(bool assertion);

int main(int argc, char *argv[]) {
    
    hashtable_t *table = hashtable_new(10);
    
    
    printf("Running test suite:\n\n");
    hashtable_put(table, "yo", (void *)123);
    

    printf("Add one item -- \n");
    printf("\tSimple key fetch: %s", get_truth((long)hashtable_get(table, "yo") == 123));

    int size = hashtable_size(table);
    printf("\tSize validation: %s (expected %d, got %d)", get_truth(1 == size), 1, size);
    
    hashtable_put(table, "justin", (void *)19);
    hashtable_put(table, "anjali", (void *)20);
    hashtable_put(table, "emily", (void *)21);
    
    printf("Add a few items --\n");
    printf("\tSimple size verification: %s", get_truth(hashtable_size(table) == 4));
    printf("\tItem verification one: %s", get_truth((long)hashtable_get(table, "justin") == 19));
    printf("\tItem verification two: %s", get_truth((long)hashtable_get(table, "anjali") == 20));
    printf("\tItem verification three: %s", get_truth((long)hashtable_get(table, "emily") == 21));
    
    printf("Removing items --\n");
    printf("\tSimple size re-verification: %s", get_truth(hashtable_size(table) == 4));
    printf("\tRemove verification: %s", get_truth(hashtable_remove(table, "justin")));
    printf("\tRemove size verification: %s", get_truth(hashtable_size(table) == 3));
    printf("\tMultiple remove\n");
    
    hashtable_remove(table, "anjali");
    hashtable_remove(table, "emily");
    hashtable_remove(table, "yo");
    
    printf("\t\t Size = 0?: %s", get_truth(hashtable_size(table) == 0));
    printf("\t\t Actually removed?: %s", get_truth(hashtable_get(table, "justin") == NULL));
    printf("\t\t Actually removed?: %s", get_truth(hashtable_get(table, "anjali") == NULL));
    printf("\t\t Actually removed?: %s", get_truth(hashtable_get(table, "emily") == NULL));
    printf("\t\t Actually removed?: %s", get_truth(hashtable_get(table, "yo") == NULL));

    printf("Forcing Grow Tests\n");

    hashtable_put(table, "justin", (void *)20);
    hashtable_put(table, "stephanie", (void *)17);
    hashtable_put(table, "kimberly", (void *)23);
    hashtable_put(table, "donna", (void *)49);
    hashtable_put(table, "nick", (void *)30);
    hashtable_put(table, "harley", (void *)20);
    hashtable_put(table, "gerard", (void *)20);
    hashtable_put(table, "david", (void *)23);
    hashtable_put(table, "becca", (void *)17);
    hashtable_put(table, "kelly", (void *)23);
    hashtable_put(table, "cara", (void *)21);

    printf("\t\t Size = 11 after adds?: %s", get_truth(hashtable_size(table) == 11));


}

const char *true_string = "true\n";
const char *false_string = "false\n";

const char *get_truth(bool assertion) {
    return (assertion ? true_string : false_string);
}
