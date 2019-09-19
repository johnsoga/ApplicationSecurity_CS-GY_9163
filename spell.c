#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// PROF REPO: https://github.com/kcg295/AppSecAssignment1
// words separated by spaces
// strip punctuation at the end only
// punctuation in the middle is misspelled
// "" etc at the beginning should be removed
void fix_word(char word[], size_t word_length) {

  char* dest = word;
  char* src = word;
  int counter = 0;

  //strip any numbers out of the word
  while(*src) {
    if (isdigit(*src)) {
      src++;
      continue;
    }
    *dest++ = *src++;
  }
  *dest = '\0';
}
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

  char buf[LENGTH];
  int num_correct = 0;

  printf("Attempting to Scan File");
  while(fscanf(fp, "%s", buf) != EOF) {
    printf("%s\n", buf);
    if(check_word(buf, hashtable)) {
      num_correct++;
    } else {
      return false;
    }
  }

  return true;
}
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

  FILE *fp;
  char buf[LENGTH+1];
  char * rtn_value;
  int bucket_value;
  hashmap_t tmp;
  hashmap_t tmp2;

  if(strlen(dictionary_file) < 1) {
    return false;
  }

  fp = fopen(dictionary_file, "r");
  if (fp != NULL) {
    while((rtn_value = fgets(buf, LENGTH+1, fp)) != NULL) {
      buf[strcspn(buf, "\n")] = 0;
      fix_word(buf, LENGTH+1);
      bucket_value = hash_function(buf);
      if((tmp = hashtable[bucket_value]) == NULL) {
        if((tmp = malloc(sizeof(node))) != NULL) {
          strncpy(tmp->word, buf, sizeof(buf));
          tmp->next = NULL;
          hashtable[bucket_value] = tmp;
        } else {
          return false;
        }
      } else {
        printf("tmp word: %s\ntmp next: %s\n", tmp->word, tmp->next);
        while(tmp->next != NULL) {
          tmp = tmp->next;
        }
        printf("C\n");
        if((tmp2 = malloc(sizeof(node)))!= NULL) {
          strncpy(tmp2->word, buf, sizeof(buf));
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

  if(tmp != NULL) {
    do {
      if(strcmp(word, tmp->word) == 0) {
        return true;
      } else {
        tmp = tmp->next;
      }
    } while(tmp != NULL);
  }

  return false;
}
int main(int argc, char **argv) {

  char *wordlist = argv[1];
  hashmap_t hashtable[HASH_SIZE];

  load_dictionary(wordlist, hashtable);
  // check_words(fp, hashtable, misspelled);
  return 0;
}
