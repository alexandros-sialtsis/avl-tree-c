
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "htable.h"

int main() {
    int num, ret, printt;
    char buf[3], sel, prev_sel;
    hash_t *hash;

    htable_init(&hash);
    strcpy(buf, "");

    while(1) {
        if(!printt){
            ret = scanf("%2s", buf);
            sel = (isalpha(buf[0])) ? buf[0] : buf[1];
        }
        switch (sel) {
        case 'i':
            prev_sel = 'i'; printt = 0;
            while(1) {
                if(!scanf(" %d", &num)) break;
                ret = htable_insert(hash, num);
                if(ret == -1) printf("\nNOT INSERTED %d\n", num);
                else printf("\nINSERTED %d\n", num);
            }
            break;
        case 'd':
            prev_sel = 'd'; printt = 0;
            while(1) {
                if(!scanf(" %d", &num)) break;
                ret = htable_delete(hash, num);
                if(ret) printf("\nDELETED %d\n", num);
                else printf("\nNOT DELETED %d\n", num);
            }
            break;
        case 'f':
            prev_sel = 'f'; printt = 0;
            while(1) {
                if(!scanf(" %d", &num)) break;
                ret = htable_find(hash, num);
                if(ret) printf("\nFOUND %d\n", num);
                else printf("\nNOT FOUND %d\n", num);
            }
            break;
        case 'p':
            sel = prev_sel; printt = 1;
            htable_print(hash);
            break;
        case 'r':
            sel = prev_sel; printt = 1;
            printf("rehash enable: %d\n", hash->rehash_en);
            hash->rehash_en = !hash->rehash_en;
            printf("rehash enable: %d\n", hash->rehash_en);
            break;
        case 'q':
            htable_free(&hash);
            return(0);
        default:
            printt = 0;
            break;
        }
    }
}