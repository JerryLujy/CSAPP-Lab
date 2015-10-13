#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

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
  if (s == 0 || E == 0 || b == 0 || fileStr == NULL) {
    // In case user forgets any of the options
    printUsage(argv[0]);
    return(EXIT_FAILURE);
  }
  
  return(EXIT_SUCCESS);
}
