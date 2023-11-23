
#ifndef __HTABLE_H_
#define __HTABLE_H_

typedef struct _hash {
    int **table;
    int inserted;
    int deleted;
    int rehash_en;
    int size;
} hash_t;

// initializes the hash table
void htable_init(hash_t **hash);

// searches for n and returns its place or
// the place it should be inserted
int htable_search(hash_t *hash, int n);

// inserts n
int htable_insert(hash_t *hash, int n);

// delete n
int htable_delete(hash_t *hash, int n);

// rehash
void htable_rehash(hash_t *hash, int bigger);

// print
void htable_print(hash_t *hash);

// returns if n is in the table
int htable_find(hash_t *hash, int n);

// frees the allocated space of the hash table
void htable_free(hash_t **hash);

#endif 
