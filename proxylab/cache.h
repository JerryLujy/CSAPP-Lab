#ifndef __CACHE_H__
#define __CACHE_H__

#include "csapp.h"

#define MAX_CACHE_SIZE  1049000
#define MAX_OBJECT_SIZE 102400

typedef struct line_st {
  int valid;
  char * url;
  size_t len;
  char * obj;
} Line;

typedef struct cache_st {
  int * usage;
  Line * line;
} Cache;

static Cache cache;
static int num_line;
static sem_t load, save;
static int load_cnt;

void    cache_init();
void    cache_destroy();
size_t  cache_load(char * url, char * response);
void    cache_save(char * url, char * response, size_t len);

#endif



