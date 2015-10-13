#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

typedef struct line_st {
  int valid;
  unsigned tag;
} Line;

typedef struct set_st {
  Line * line;
  int lruInd;
} Set;

void printUsage(char * arg) {
  printf("\nUsage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n\n", arg);
}

int main(int argc, char * * argv) {
  int s = 0;
  int E = 0;
  int b = 0;
  int verbose = 0;
  char * fileStr = NULL;
  char ch;
  while ((ch = getopt(argc, argv, "hvs:E:b:t:")) != EOF) {
    switch (ch) {
    case 's':
      s = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      b = atoi(optarg);
      break;
    case 't':
      fileStr = optarg;
      break;
    case 'v':
      verbose = 1;
      break;
    case 'h':
      printUsage(argv[0]);
      return(EXIT_SUCCESS);
    default:
      printUsage(argv[0]);
      return(EXIT_FAILURE);
    }
  }
  // In case user forgets any of the options
  if (s == 0 || E == 0 || b == 0 || fileStr == NULL) {
    printUsage(argv[0]);
    return(EXIT_FAILURE);
  }
  int S = (1 << s);
  // Initiates the data structure for the cache
  Set * set = malloc(S * sizeof(Set));
  for (int i = 0; i < S; i++) {
    set[i].line = malloc(E * sizeof(Line));
    for (int j = 0; j < E; j++) {
      set[i].line[j].valid = 0;
    }
    set[i].lruInd = 0;
  }

  if (verbose);
  
  // Frees the data structure before exiting
  for (int i = 0; i < S; i++) {
    free(set[i].line);
  }
  free(set);
  return(EXIT_SUCCESS);
}
