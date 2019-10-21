#include "dictionary.h"
#include <stdlib.h>

void freeHashtable(hashmap_t hashtable[]) {

    int i;
    hashmap_t curr;
    hashmap_t next;

    for(i = 0; i < HASH_SIZE; i++) {
        curr = hashtable[i];
        while(curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }
}
void freeCharArray(char* misspelled[]) {

    int i;

    for(i = 0; i < MAX_MISSPELLED; i++) {
        if(misspelled[i] != NULL) {
            free(misspelled[i]);
        }
    }
}
void printHashtable(hashmap_t hashtable[]) {

    int i;
    hashmap_t curr;

    for(i = 0; i < HASH_SIZE; i++) {
        printf("Index %d: ", i);
        curr = hashtable[i];
        while(curr != NULL) {
            printf("%s > ", curr->word);
            curr = curr->next;
        }
        printf("\n\n\n");
    }
}
void printMisspelled(char* misspelled[]) {

    int i;

    for(i = 0; i < MAX_MISSPELLED; i++) {
        if(misspelled[i] != NULL) {
            printf("%s is incorrect\n", misspelled[i]);
        }
    }
}
int main(int argc, char **argv) {

    char *wordlist = argv[2];
    char *text = argv[1];
    FILE *fp;
    hashmap_t hashtable[HASH_SIZE];
    char *misspelled[MAX_MISSPELLED];

    fp = fopen(text, "r");

    // load the hashtable with words from the dictionary
    load_dictionary(wordlist, hashtable);

    // check the words from the input file to see which are in the dictionary
    check_words(fp, hashtable, misspelled);

    // free the hashtable memory
    freeHashtable(hashtable);

    // free the misspelled memory
    freeCharArray(misspelled);

    fclose(fp);
    return 0;
}
