#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"

typedef struct line_st {
  int valid;
  unsigned long tag;
} Line;

typedef struct set_st {
  Line * line;
  int lruInd;
} Set;

void printUsage(char * arg) {
  printf("\nUsage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n\n", arg);
}

int main(int argc, char * * argv) {
  int s = 0;// Number of set index bits
  int E = 0;// Number of lines per set
  int b = 0;// Number of block bits
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

  // Open the trace file
  FILE * fptr = fopen(fileStr, "r");
  if (fptr == NULL) {
    printf("Unable to open file \"%s\"\n", fileStr);
    return(EXIT_FAILURE);
  }

  char op;// Type of operation on memory in the trace file
  unsigned long addr;// 64-bit hexadecimal memory address
  unsigned size;// Number of bytes accessed by the operation
  unsigned long tag;// Tag bits
  unsigned setInd;// Index of set where the block belongs to

  // The result values to be returned
  int timeHit = 0, timeMiss = 0, timeEvict = 0;
  
  while (fscanf(fptr, " %c %lx,%d\n", &op, &addr, &size) != EOF) {
    tag = addr >> (s + b);
    setInd = (addr >> b) & ((1 << s) - 1);

    if (op == 'I') continue;
    if (verbose) printf("%c %lx,%d ", op, addr, size);
    printf("\t\ttag=%lx\tset=%d\n", tag, setInd);

    
  }

  if (verbose) printf("\n");
  printSummary(timeHit, timeMiss, timeEvict);

  // Frees the data structure and close file before exiting
  fclose(fptr);
  for (int i = 0; i < S; i++) {
    free(set[i].line);
  }
  free(set);
  return(EXIT_SUCCESS);
}
