
#include <stdio.h>
#include <stdlib.h>
#include "htable.h"

void htable_init(hash_t **hash) {
    int i;

    *hash = (hash_t *) malloc(sizeof(hash_t));
    (*hash)->inserted = 0;
    (*hash)->deleted = 0;
    (*hash)->rehash_en = 1;
    (*hash)->size = 2;
    (*hash)->table = (int **) malloc(2 * sizeof(int *));

    for(i = 0; i < 2; i++) {
        (*hash)->table[i] = NULL;
    }
}

int htable_search(hash_t *hash, int n) {
    int i = 0, j = 0;

    while(1) {
        i = (n + j) % hash->size;

        if(hash->table[i] == NULL
        || hash->table[i] == (int *) 0x1
        || *(hash->table[i]) == n) return(i);
        else if(j == hash->size) return(-1);
        else j++;
    }
}

int htable_insert(hash_t *hash, int n) {
    int i;
    double load_factor;

    i = htable_search(hash, n);
    if(i == -1 || (hash->table[i] && *(hash->table[i]) == n)) return(-1);

    load_factor = (double) hash->inserted / (double) hash->size;

    if(hash->rehash_en && load_factor >= 0.5) {
        htable_rehash(hash, 1);
        i = htable_search(hash, n);
    }

    hash->table[i] = (int *) malloc(sizeof(int));
    *(hash->table[i]) = n;
    hash->inserted++;
    return(i);
}

int htable_delete(hash_t *hash, int n) {
    int i;
    double load_factor;

    i = htable_search(hash, n);
    if(i == -1 || !hash->table[i]) return(0);
    while(hash->table[i] == (int *) 0x1) {
        i++;
        if(i == hash->size) i = 0;
    }
    if(!hash->table[i] || *(hash->table[i]) != n) return(0);

    free(hash->table[i]);
    hash->table[i] = (int *) 0x1;
    hash->deleted++;
    hash->inserted--;
    
    load_factor = (double) hash->inserted / (double) hash->size;
    if(hash->rehash_en && load_factor <= 0.125) htable_rehash(hash, 0);
    return(1);
}

void htable_rehash(hash_t *hash, int bigger) {
    int i, **old_table = hash->table, old_size = hash->size;

    if(bigger) hash->size = hash->size * 2;
    else if(hash->size > 2) hash->size = hash->size / 2;
    else return;

    hash->table = (int **) malloc(hash->size * sizeof(int *));
    hash->deleted = 0;
    hash->inserted = 0;
    for(i = 0; i < hash->size; i++) {
        hash->table[i] = NULL;
    }

    for(i = 0; i < old_size; i++) {
        if(old_table[i] != NULL && old_table[i] != (int *) 0x1) {
            htable_insert(hash, *(old_table[i]));
            free(old_table[i]);
        }
    }
    free(old_table);
}

void htable_print(hash_t *hash) {
    int i;

    printf("\nSIZE: %d, INSERTED: %d, DELETED: %d\n", hash->size, hash->inserted, hash->deleted);

    for(i = 0; i < hash->size; i++) {
        printf("%3d ", i);
    }
    printf("\n");
    for(i = 0; i < hash->size; i++) {
        if(!hash->table[i]) printf("  * ");
        else if(hash->table[i] == (int *) 0x1) printf("  # ");
        else printf("%3d ", *(hash->table[i]));
    }
    printf("\n");
}

int htable_find(hash_t *hash, int n) {
    int i;

    i = htable_search(hash, n);
    if(i == -1
    || hash->table[i] == NULL
    || hash->table[i] == (int *) 0x1) return(0);
    else return(1);
}

void htable_free(hash_t **hash) {
    int i;

    for(i = 0; i < (*hash)->size; i++) {
        if((*hash)->table[i] != NULL && (*hash)->table[i] != (int *) 0x1) {
            free((*hash)->table[i]);
        }
    }
    free((*hash)->table);
    free(*hash);
    *hash = NULL;
}

/*int main() {
    hash_t *hash;

    htable_init(&hash);
    htable_print(hash);
    htable_insert(hash, 0);
    htable_print(hash);
    htable_insert(hash, 4);
    htable_print(hash);
    htable_insert(hash, 2);
    htable_print(hash);
    htable_insert(hash, 10);
    htable_print(hash);
    htable_insert(hash, 4);
    htable_print(hash);
    htable_insert(hash, 5);
    htable_print(hash);
    htable_insert(hash, 6);
    htable_print(hash);
    htable_insert(hash, 7);
    htable_print(hash);
    htable_insert(hash, 20);
    htable_print(hash);
    htable_insert(hash, 21);
    htable_print(hash);

    htable_delete(hash, 20);
    htable_print(hash);
    htable_delete(hash, 21);
    htable_print(hash);
    htable_delete(hash, 0);
    htable_print(hash);
    htable_delete(hash, 10);
    htable_print(hash);
    htable_delete(hash, 2);
    htable_print(hash);
    htable_delete(hash, 7);
    htable_print(hash);
    htable_delete(hash, 6);
    htable_print(hash);
    htable_delete(hash, 4);
    htable_print(hash);
    htable_delete(hash, 5);
    htable_print(hash);

    return(0);
} */