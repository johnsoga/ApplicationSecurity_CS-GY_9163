#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

  char buf[LENGTH];
  int num_correct = 0;

  while(fscanf(fp, "%s", buf) != EOF) {
    if(check_word(buf, hashtable)) {
      num_correct++;
    } else {
    }
  }

  return true;
      return false;
}
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

  FILE *fp;
  char buf[LENGTH];
  int bucket_value;
  hashmap_t tmp;
  hashmap_t tmp2;

  fp = fopen(dictionary_file, "r");
  if (fp != NULL) {
    while(fscanf(fp, "%s", buf) != EOF) {
      bucket_value = hash_function(buf);
      tmp = hashtable[bucket_value];
      if(tmp->next == NULL && tmp->word[0] == '\0') {
        strcpy(tmp->word, buf);
      } else {
        while(tmp->next != NULL) {
          tmp = tmp->next;
        }
        tmp2 = malloc(sizeof(node));
        if(tmp2 != NULL) {
          strcpy(tmp2->word, buf);
          tmp2->next = NULL;
          tmp->next = tmp2;
        } else {
          return false;
        }
      }
    }
    fclose(fp);
    return true;
  }

  return false;
}
bool check_word(const char* word, hashmap_t hashtable[]) {

  int bucket_value;
  hashmap_t tmp;

  bucket_value = hash_function(word);
  tmp = hashtable[bucket_value];

  do {
    if(!strcmp(word, tmp->word)) {
      return true;
    } else {
      tmp = tmp->next;
    }
  } while(tmp->next != NULL);

  return false;
}
// int main(int argc, char **argv) {
//
//   int i;
//   char *wordlist = argv[2];
//   char *inputlist = argv[1];
//   char *misspelled;
//   hashmap_t hashtable[HASH_SIZE];
//   hashmap_t tmp;
//
//   for (i = 0; i < HASH_SIZE; i++) {
//     tmp = malloc(sizeof(node));
//     if(tmp != NULL) {
//       tmp->word[0] = '\0';
//       tmp->next = NULL;
//       hashtable[i] = tmp;
//     } else {
//       printf("Failed to allocate memory");
//       exit(0);
//     }
//   }
//
//   load_dictionary(wordlist, hashtable);
//   check_words(fp, hashtable, misspelled);
//   return 0;
// }
