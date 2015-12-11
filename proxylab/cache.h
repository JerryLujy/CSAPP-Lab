/*
 * cache.h - declaration of the cache managing functions for
 *           multithreaded caching proxy
 *
 * Author: Jieyu Lu      Andrew ID: jieyul1
 */
#ifndef __CACHE_H__
#define __CACHE_H__

#include "csapp.h"

/* The maximum size of cache and each cached object */
#define MAX_CACHE_SIZE  1049000
#define MAX_OBJECT_SIZE 102400

/* Information stored in each cache line */
typedef struct line_st {
  int valid;
  char * url;
  size_t len;
  char * obj;
} Line;

/* The cache object. Field "usage" defines the order of recently 
   used cache lines. Each time a cache line is read/write, its 
   number is put at the front of this array. */
typedef struct cache_st {
  int * usage;
  Line * line;
} Cache;

/* Initiatializes the cache. Allocates memory for cache and cache lines */
void    cache_init();
/* Frees up the allocated memory */
void    cache_destroy();
/* Loads the content from cache by url into response
   Cache hit returns size of the content, miss returns zero */
size_t  cache_load(char * url, char * response);
/* Saves the server response with url, and length len into cache */
void    cache_save(char * url, char * response, size_t len);

#endif



