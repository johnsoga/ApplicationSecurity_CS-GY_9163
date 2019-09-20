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

  //lowercase the word
  while(*src) {
    if(isalpha(*src)) {
      *src = tolower((int)*src);
    }
    *src++;
  }

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
  // //strip numbers from string
  // while(*src) {
  //   if(isdigit(*src)) {
  //     src++;
  //   } else {
  //     *dest++ = *src++;
  //   }
  // }
  // *dest = '\0';



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
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

  char buf[LENGTH+1];
  int num_incorrect = 0;
  int counter = 0;

  while(fscanf(fp, "%s", buf) != EOF) {
    fix_word(buf, LENGTH+1);
    // printf("Word is %s\n", buf);
    if(!check_word(buf, hashtable)) {
      printf("Not Found:\t%s\n", buf);
      num_incorrect++;
      // strncpy(misspelled[0], buf, sizeof(buf));
    } else {
      printf("Found:\t\t%s\n", buf);
    }
  }

  return num_incorrect;
}
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

  FILE *fp;
  char buf[LENGTH+1];
  char * rtn_value;
  int bucket_value;
  hashmap_t tmp;
  hashmap_t tmp2;
  int counter;

  for(counter = 0; counter < HASH_SIZE; counter++) {
    hashtable[counter] = NULL;
  }

  fp = fopen(dictionary_file, "r");
  if (fp == NULL) {
    return false;
  }
  while((rtn_value = fgets(buf, LENGTH+1, fp)) != NULL) {
    buf[strcspn(buf, "\n")] = 0;
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
      while(tmp->next != NULL) {
        tmp = tmp->next;
      }
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
bool check_word(const char* word, hashmap_t hashtable[]) {

  int bucket_value;
  hashmap_t tmp;

  bucket_value = hash_function(word);
  tmp = hashtable[bucket_value];

  while(tmp != NULL) {
    if(strcmp(word, tmp->word) == 0) {
      return true;
    } else {
      tmp = tmp->next;
    }
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
//   check_words(fp, hashtable, misspelled);
//   return 0;
// }
