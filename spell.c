#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void toLowercase(char *buf) {

  int i = 0;

  while(buf[i] != '\0') {
    if(isalpha(buf[i])) {
       buf[i] = tolower((int)buf[i]);
    }
    i++;
  }
}
void trim_end(char *buf, size_t length) {

  int i = length-1;

  while(!isalpha(buf[i-1]) && ispunct(buf[i])) {
      buf[i] = '\0';
      i--;
  }

  if(ispunct(buf[i])) {
    buf[i] = '\0';
  }
}
bool check_ascii(char *buf) {

  char* src = buf;

  while(*src) {
    if(isascii(*src)) {
      src++;
    } else {
      return false;
    }
  }

  return true;
}
bool check_punct(char *buf) {

  char* src = buf;

  while(*src) {
    if(ispunct(*src)) {
      if(strcmp(src, "'") != 0) {
        return true;
      }
    }
    src++;
  }

  return false;
}
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

  FILE *fp;
  char buf[LENGTH+1];
  int bucket_value;
  hashmap_t new_node;
  int counter;

  memset(buf, '\0', sizeof(buf));

  //  Initialize all values in hash table to NULL.
  for(counter = 0; counter < HASH_SIZE; counter++) {
    hashtable[counter] = NULL;
  }

  //  Open dict_file from path stored in dictionary.
  fp = fopen(dictionary_file, "r");

  //  If dict_file is NULL: return false
  if (fp == NULL) {
    fclose(fp);
    return false;
  }

  // While word in dict_file is not EOF (end of file):
  while(fgets(buf, LENGTH+1, fp) != NULL) {
    buf[strcspn(buf, "\n")] = 0;
    printf("%s\n", buf);
    new_node = malloc(sizeof(struct node));
    new_node->next = NULL;
    strncpy(new_node->word, buf, strlen(buf));
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
  int length = strlen(word);
  char buf[length];
  memset(buf, '\0', sizeof(buf));
  strcpy(buf, word);


  if(!check_ascii(word)) {
    return false;
  }

  if(length > LENGTH) {
    return false;
  }

  trim_end(buf, length);

  if(check_punct(buf)) {
    return false;
  }

  toLowercase(buf);

  bucket_value = hash_function(buf);
  cursor = hashtable[bucket_value];
  while(cursor != NULL) {
    if(strcmp(buf, cursor->word) == 0) {
      return true;
    }
    cursor = cursor->next;
  }

  printf("%s\t%s\n", buf, word);

  return false;
}
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

  char buf[LENGTH+1];
  char* overflow;
  int num_incorrect;

  //  Initialize all values in misspelled to NULL.
  for(num_incorrect = 0; num_incorrect < MAX_MISSPELLED; num_incorrect++) {
    misspelled[num_incorrect] = NULL;
  }

  num_incorrect = 0;
  while(fscanf(fp, "%ms", &overflow) != EOF) {
    strncpy(buf, overflow, LENGTH);
    free(overflow);
    buf[LENGTH] = '\0';
    if(!check_word(buf, hashtable)) {
      misspelled[num_incorrect] = malloc(sizeof(buf));
      strncpy(misspelled[num_incorrect], buf, strlen(buf));
      num_incorrect++;
    }
  }

  return num_incorrect;
}
