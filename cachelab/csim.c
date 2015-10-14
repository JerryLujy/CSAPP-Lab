/*
 * 15213 cache lab Part A -- Cache Simulator
 * 
 * Author: Jieyu Lu
 * Andrew ID: jieyul1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"

// Structor that simulates the behavior of a cache line
typedef struct line_st {
  int valid;
  int timeStamp;
  unsigned long tag;
} Line;
typedef Line * Set;
typedef Set * Cache;

// Find the least recently used line in a set according to the time stamp associated with each line
int findLRU(Set s, int E) {
  int ind = 0;
  int maxStamp = 0;
  for (int i = 0; i < E; i++) {
    if (s[i].timeStamp > maxStamp) {
      ind = i;
      maxStamp = s[i].timeStamp;
    }
  }
  return ind;
}

// Helper function that prints out the usage of the program
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
  Cache cache = malloc(S * sizeof(Set));
  for (int i = 0; i < S; i++) {
    cache[i] = malloc(E * sizeof(Line));
    for (int j = 0; j < E; j++) {
      cache[i][j].valid = 0;
      cache[i][j].timeStamp = 0;
    }
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

  // The result values to be returned
  int timeHit = 0, timeMiss = 0, timeEvict = 0;
  
  while (fscanf(fptr, " %c %lx,%d\n", &op, &addr, &size) != EOF) {
    // If the operation is "I", ignore it
    if (op == 'I') continue;
    
    unsigned long tag = addr >> (s + b);
    unsigned setInd = (addr >> b) & ((1 << s) - 1);

    Set set = cache[setInd];// Reference to the set we are dealing with
    int hit = 0;// Indicator if we have encountered a hit

    if (verbose) printf("%c %lx,%d ", op, addr, size);

    // Update the time stamp for each of the lines in the cache
    for (int i = 0; i < S; i++) {
      for (int j = 0; j < E; j++) {
	(cache[i][j].timeStamp)++;
      }
    }
    // For 'M', 'L' and 'S' operations
    // Search through all lines in the set to see if we have a hit
    // If we have a hit, great, update the hit counter and reset the time stamp
    for (int i = 0; i < E; i++) {
      if (set[i].valid && set[i].tag == tag) {
	hit = 1;
	// Refresh the time stamp now to indicate that we have recently used this line
	set[i].timeStamp = 0;
	if (verbose) printf("hit ");
	timeHit++;
	break;
      }
    }
    // If we don't have a hit
    if (!hit) {
      if (verbose) printf("miss ");
      timeMiss++;
      // Find if there exists a line in the set with valid bit not set.
      int spareLine = -1;
      for (int i = 0; i < E; i++) {
	if (!set[i].valid) {
	  spareLine = i;
	  break;
	}
      }
      // If there is, update that line
      if (spareLine != -1) {
	set[spareLine].valid = 1;
	set[spareLine].tag = tag;
	set[spareLine].timeStamp = 0;
      }
      // If all lines are occupied, we need to evict a least recently used line
      else {
	int lru = findLRU(set, E);
	set[lru].tag = tag;
	set[lru].timeStamp = 0;
	if (verbose) printf("eviction ");
	timeEvict++;
      }
    }
    // For operation 'M', there must be a hit after the first store operation
    if (op == 'M') {
      if (verbose) printf("hit ");
      timeHit++;
    }
    if (verbose) printf("\n");
  }
  printSummary(timeHit, timeMiss, timeEvict);

  // Frees the data structure and close file before exiting
  fclose(fptr);
  for (int i = 0; i < S; i++) {
    free(cache[i]);
  }
  free(cache);
  return(EXIT_SUCCESS);
}
