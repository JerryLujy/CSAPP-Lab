/*
 * cache.c - definition of the cache managing functions for
 *           multithreaded caching proxy.
 *
 * Author: Jieyu Lu      Andrew ID: jieyul1
 */
#include "cache.h"

static Cache cache;       // cache object
static int num_line;      // number of lines in the cache
static int load_cnt;      // number of threads loading content from cache
static sem_t load, save;  // semaphores in first readers-writers problem
/* Helper function that updates the usage history array */
static void update_usage(int);

void cache_init()
{
  num_line = MAX_CACHE_SIZE / MAX_OBJECT_SIZE;
  load_cnt = 0;// record how many threads are currently loading from cache
  Sem_init(&load, 0, 1);// initialize semaphores
  Sem_init(&save, 0, 1);
  /* Allocate memory for the cache */
  cache.usage = malloc(sizeof(int) * num_line);
  cache.line = malloc(sizeof(Line) * num_line);
  int i;
  for (i = 0; i < num_line; i++) {
    cache.usage[i] = i;
    cache.line[i].valid = 0;
    cache.line[i].url = malloc(sizeof(char) * MAXLINE);
    cache.line[i].obj = malloc(sizeof(char) * MAX_OBJECT_SIZE);
  }
}

void cache_destroy()
{
  int i;
  for (i = 0; i < num_line; i++) {
    free(cache.line[i].url);
    free(cache.line[i].obj);
  }
  free(cache.usage);
  free(cache.line);
}

/* Note that the usage array might not strictly reflect LRU, due to 
   the uncertainty of thread execution order. Nevertheless, line number
   to the front of the array must be recently used. */
size_t cache_load(char * url, char * response)
{
  P(&load);// protects load_cnt
  load_cnt++;
  if (load_cnt == 1)// first load action locks save action
    P(&save);
  V(&load);

  int i, hit = -1;
  for (i = 0; i < num_line; i++) {
    if (cache.line[i].valid &&
	strcmp(cache.line[i].url, url) == 0) {
      hit = i;
      memcpy(response, cache.line[i].obj, cache.line[i].len);
      break;
    }
  }

  P(&load);
  load_cnt--;
  if (load_cnt == 0)// last load action unlocks save action
    V(&save);
  V(&load);

  if (hit >= 0) {
    P(&save);// update usage is a write action, needs lock
    update_usage(hit);
    V(&save);
    return cache.line[hit].len;
  }
  return 0;
}

void cache_save(char * url, char * response, size_t len)
{
  P(&save);// locks save action

  /* least recently used line must be at the end of the array */
  int evict = cache.usage[num_line - 1];
  
  strcpy(cache.line[evict].url, url);
  memcpy(cache.line[evict].obj, response, len);
  if (cache.line[evict].valid == 0)
    cache.line[evict].valid = 1;
  cache.line[evict].len = len;
  update_usage(evict);

  V(&save);// unlocks save action
}

/*
 * update_usage
 *
 * Updates the usage history by placing the recently used line number
 * at the beginning of the usage array
 */
static void update_usage(int recent)
{
  int i, j;
  for (i = 0; i < num_line; i++) {
    if (cache.usage[i] == recent)
      break;
  }
  for (j = i; j > 0; j--) {
    cache.usage[j] = cache.usage[j - 1];
  }
  cache.usage[0] = recent;
}
