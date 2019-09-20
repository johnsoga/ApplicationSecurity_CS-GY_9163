#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// PROF REPO: https://github.com/kcg295/AppSecAssignment1
// words separated by spaces
// strip punctuation at the end only
// punctuation in the middle is misspelled
// "" etc at the beginning should be removed
void fix_word(char word[], size_t word_length) {

  char* dest = word;
  char* src = word;
  char buf[LENGTH+1];
  int counter;

  // //remove trailing punctuation
  // src = word;
  // counter = 0;
  // // printf("%c\n", src[counter-1]);
  //
  // while(word[counter+1] != \) {
  //   // printf("%s\n", word[counter-1]);
  //   if(ispunct(word[counter0])) {
  //     word[counter-1] = '\0';
  //   }
  //   counter--;
  // }
  //strip numbers from string
  src = dest = word;
  while(*src) {
    if(ispunct(*src)) {
      src++;
    } else {
      *dest++ = *src++;
    }
  }
  *dest = '\0';



  //strip any punctuation out of the word
  // src = word;
  // dest = word;
  // while (*src) {
  //   printf("SRC is %c\n", src[0]);
  //   printf("DEST is %c\n", dest[0]);
  //   if (ispunct((int)src[0])) {
  //     src++;
  //   }
  //   else if (src == dest) {
  //     printf("\n\n???\n\n\n");
  //     src++;
  //     dest++;
  //   } else {
  //     *dest++ = *src++;
  //   }
  // }
  // *dest = '\0';

}
void toLowercase(char *word) {

  char* src = word;

  while(*src) {
    if(isalpha(*src)) {
      *src = tolower((int)*src);
    }
    *src++;
  }
}
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

  char buf[LENGTH+1];
  int num_incorrect = 0;
  int counter = 0;

  while(fscanf(fp, "%s", buf) != EOF) {
    if(!check_word(buf, hashtable)) {
      num_incorrect++;
      misspelled[counter] = malloc(sizeof(buf));
      strncpy(misspelled[counter], buf, sizeof(buf));
    }
  }

  return num_incorrect;
}
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

  FILE *fp;
  char buf[LENGTH+1];
  int bucket_value;
  hashmap_t new_node;
  int counter;

  //  Initialize all values in hash table to NULL.
  for(counter = 0; counter < HASH_SIZE; counter++) {
    hashtable[counter] = NULL;
  }
  //  Open dict_file from path stored in dictionary.
  fp = fopen(dictionary_file, "r");

  //  If dict_file is NULL: return false
  if (fp == NULL) {
    return false;
  }
  // While word in dict_file is not EOF (end of file):
  while(fgets(buf, LENGTH+1, fp) != NULL) {
    buf[strcspn(buf, "\n")] = 0;
    // toLowercase(buf);
    new_node = malloc(sizeof(struct node));
    new_node->next = NULL;
    strncpy(new_node->word, buf, sizeof(buf));
    bucket_value = hash_function(buf);

    if(hashtable[bucket_value] == NULL) {
      hashtable[bucket_value] = new_node;
    } else {
      new_node->next = hashtable[bucket_value];
      hashtable[bucket_value] = new_node;
    }
  }

  fclose(fp);
  return true;
}
bool check_word(const char* word, hashmap_t hashtable[]) {

  int bucket_value;
  hashmap_t cursor;

  bucket_value = hash_function(word);
  cursor = hashtable[bucket_value];

  while(cursor != NULL) {
    if(strcmp(word, cursor->word) == 0) {
      return true;
    }
    tmp = tmp->next;
  }

  bucket_value = hash_function(word);
  cursor = hashtable[bucket_value];
  toLowercase(word);
  while(cursor != NULL) {
    if(strcmp(word, cursor->word) == 0) {
      return true;
    }
    tmp = tmp->next;
  }

  return false;
}
// int main(int argc, char **argv) {
//
//   char *wordlist = argv[2];
//   char *text = argv[1];
//   char *misspelled[MAX_MISSPELLED];
//   FILE *fp;
//
//   hashmap_t hashtable[HASH_SIZE];
//   fp = fopen(text, "r");
//
//   load_dictionary(wordlist, hashtable);
//   printf("Found %d bad words\n", check_words(fp, hashtable, misspelled));
//   return 0;
// }
