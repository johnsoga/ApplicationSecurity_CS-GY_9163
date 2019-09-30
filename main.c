#include "dictionary.h"
#include <stdlib.h>

int main(int argc, char **argv) {

  char *wordlist = argv[2];
  char *text = argv[1];
  char *misspelled[MAX_MISSPELLED];
  int num_words_found;
  FILE *fp;
  int i;
  hashmap_t curr;
  hashmap_t next;

  hashmap_t hashtable[HASH_SIZE];
  fp = fopen(text, "r");

  //load the hashtable with words from the dictionary
  load_dictionary(wordlist, hashtable);

  //check the words from the input file to see which
  //are in the dictionary
  num_words_found = check_words(fp, hashtable, misspelled);

  printf("Found %d words that were incorrect\n", num_words_found);
  
  //free the hashtable memory
  for(i = 0; i < HASH_SIZE; i++) {
    curr = hashtable[i];
    while(curr != NULL) {
      next = curr->next;
      free(curr);
      curr = next;
    }
  }

  //free the misspelled memory
  for(i = 0; i < MAX_MISSPELLED; i++) {
    if(misspelled[i] != NULL) {
      free(misspelled[i]);
    }
  }

  fclose(fp);
  return 0;
}
