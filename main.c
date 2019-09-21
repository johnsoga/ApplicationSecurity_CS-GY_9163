int main(int argc, char **argv) {

  char *wordlist = argv[2];
  char *text = argv[1];
  char *misspelled[MAX_MISSPELLED];
  FILE *fp;
  int i = 0;

  hashmap_t hashtable[HASH_SIZE];
  fp = fopen(text, "r");

  load_dictionary(wordlist, hashtable);
  printf("Found %d bad words\n", check_words(fp, hashtable, misspelled));

  for(i = 0; i<7; i++) {
    printf("%s\n", misspelled[i]);
  }
  return 0;
}
