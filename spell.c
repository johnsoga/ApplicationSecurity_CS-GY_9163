#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void toLowercase(char *word) {

  char* src = word;

  while(*src) {
    if(isalpha(*src)) {
      *src = tolower((int)*src);
    }
    *src++;
  }
}
void trim(char *word) {

  char* src = word;
  char* dest = word;
  bool check = false;
  int counter;

  while(*src) {
    if(!check) {
      if(ispunct(*src)) {
        src++;
      } else {
        check = true;
      }
    } else {
      *dest++ = *src++;
    }
  }
  *dest = '\0';

  counter = strlen(word)-1;
  check = false;
  while(counter > 0) {
    if(!check) {
      if(ispunct(word[counter])) {
        word[counter] = '\0';
      } else {
        check = true;
        break;
      }
    }
    counter--;
  }
}
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

  char buf[LENGTH+1] = {'\0'};
  int num_incorrect = 0;
  int counter = 0;

  while(fscanf(fp, "%45s", buf) != EOF) {
    if(!check_word(buf, hashtable)) {
      misspelled[num_incorrect] = malloc(sizeof(buf));
      strncpy(misspelled[num_incorrect], buf, sizeof(buf));
      num_incorrect++;
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
    cursor = cursor->next;
  }

  toLowercase(word);
  trim(word);
  bucket_value = hash_function(word);
  cursor = hashtable[bucket_value];
  while(cursor != NULL) {
    if(strcmp(word, cursor->word) == 0) {
      return true;
    }
    cursor = cursor->next;
  }

  return false;
}
