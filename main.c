#include "dictionary.h"
#include <stdlib.h>

int main(int argc, char **argv) {

  char *wordlist = argv[2];
  char *text = argv[1];
  char *misspelled[MAX_MISSPELLED];
  FILE *fp;
  int i;
  hashmap_t curr;
  hashmap_t next;
  char *iw = "Hello";

  hashmap_t hashtable[HASH_SIZE];
  fp = fopen(text, "r");

  load_dictionary(wordlist, hashtable);
  check_word(iw, hashtable);
  // check_words(fp, hashtable, misspelled);

  // for(i = 0; i < HASH_SIZE; i++) {
  //   curr = hashtable[i];
  //   while(curr != NULL) {
  //     next = curr->next;
  //     free(curr);
  //     curr = next;
  //   }
  // }
  // for(i = 0; i < MAX_MISSPELLED; i++) {
  //   if(misspelled[i] != NULL) {
  //     free(misspelled[i]);
  //   }
  // }

  fclose(fp);
  return 0;
}
