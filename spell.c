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
void trim_end(char *buf) {

  int length = strlen(buf);
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
void trim_front(char *buf) {

  int length = strlen(buf);

  while(ispunct(buf[0])) {
    memcpy(buf, &buf[1], length - 1);
  }
}
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

  FILE *fp;
  char buf[LENGTH+1];
  int bucket_value;
  hashmap_t new_node = NULL;
  int counter;

  //erase buf to be empty
  memset(buf, '\0', sizeof(buf));

  //initialize all values in hash table to NULL
  for(counter = 0; counter < HASH_SIZE; counter++) {
    hashtable[counter] = NULL;
  }

  //open dictionary from path stored in dictionary_file
  fp = fopen(dictionary_file, "r");

  //if dictionary_file is NULL: return false
  if (fp == NULL) {
    fclose(fp);
    return false;
  }

  //while read word from dictionary_file is not EOF (end of file):
  while(fgets(buf, LENGTH+1, fp) != NULL) {

    //read buf again as string minus whitespace to remove any newline
    sscanf(buf , "%s", buf);

    //create a new node to hold the new word
    //set next to NULL as it doesnt point to anything yet
    //copy the read word into the new node
    if((new_node = malloc(sizeof(struct node))) != NULL) {
      new_node->next = NULL;
      strncpy(new_node->word, buf, strlen(buf));

      //find correct buck for word in hashtable
      bucket_value = hash_function(buf);

      //if its the first item in the bucket then just
      //point the bucket to the new node otherwise
      //add it to the front of the list for that bucket
      if(hashtable[bucket_value] == NULL) {
        hashtable[bucket_value] = new_node;
      } else {
        new_node->next = hashtable[bucket_value];
        hashtable[bucket_value] = new_node;
      }
    } else {
      return false;
    }
    memset(buf, '\0', sizeof(buf));
  }

  //close the file
  fclose(fp);
  return true;
}
bool check_word(const char* word, hashmap_t hashtable[]) {

  int bucket_value;
  hashmap_t cursor;
  char buf[LENGTH];
  int length;

  //if read word is greater than maximum length of a word than it cannot be valid
  if((length = strlen(word)) > LENGTH) {
      return false;
  }

  //erase buf memory space and then copy the word to check into it
  memset(buf, '\0', sizeof(buf));
  strcpy(buf, word);

  toLowercase(buf);
  bucket_value = hash_function(buf);
  cursor = hashtable[bucket_value];
  while(cursor != NULL) {
    if(strcmp(buf, cursor->word) == 0) {
      return true;
    }
    cursor = cursor->next;
  }

  return false;
}
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

  int index = 0;
  ssize_t read;
  char *line = NULL;
  size_t len = 0;
  char* token;
  int num_incorrect = 0;

  //initialize all values in misspelled to NULL
  for(index = 0; index < MAX_MISSPELLED; index++) {
    misspelled[index] = NULL;
  }

  //if fp is NULL: return 0
  if (fp == NULL) {
    return 0;
  }

  //read a line from the file until eof
  while ((read = getline(&line, &len, fp)) != -1) {

    //strip the newline at the end
    line[strcspn(line, "\n")] = '\0';

    //split the line into tokens split by space and check if each
    //token is in the hashtable
    token = strtok(line, " ");
    while (token != NULL) {

        //trim any punctuation from the end of token
        trim_end(token);

        //trim any punctuation from the front of token
        trim_front(token);

        //if there are non ascii characters then the word is not valid
        if(!check_ascii(token)) {
          return false;
        }

      if(!check_word(token, hashtable)) {
        misspelled[num_incorrect] = malloc(sizeof(token));
        strncpy(misspelled[num_incorrect], token, strlen(token));
        num_incorrect++;
      }
      token = strtok(NULL, " ");
    }
    free(line);
  }

  return num_incorrect;
}
