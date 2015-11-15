/*
 * mm.c
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

#define MAX(A, B) ((A) > (B) ? (A) : (B))

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)

#define WSIZE 4             /* Word (header, footer) size in bytes */
#define DSIZE 8             /* Double word (pointer, size_t) size in bytes */
#define CHUNKSIZE (1 << 10) /* Extend heap by at least this amount */

/* Pack a size and allocate bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Given ptr p, read and write a word at this address */
#define GET(p) (*(unsigned long *)(p))
#define PUT(p, val) (*(unsigned long *)(p) = (val))

/* Given ptr p, read size, alloc and predalloc info from this address */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_PRED_ALLOC(p) (GET(p) & 0x2)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(bp) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE((char *)(bp) - WSIZE))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))

/* Global variables */
static char * heap_listp = NULL;   /* Pointer to first block */
static size_t * free_listp = NULL; /* Pointer to first free block */

/* Helper functions */
static void * extend_heap(size_t words);

/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) {
  /* Create an empty heap */
  if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
    return -1;

  PUT(heap_listp, 0);                           /* Alignment padding */
  PUT(heap_listp +     WSIZE, PACK(DSIZE, 1));  /* Prologue header */
  PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));  /* Prologue footer */
  PUT(heap_listp + 3 * WSIZE, PACK(0, 1));      /* Epilogue header */

  heap_listp += 2 * WSIZE;

  /* Extend the heap with CHUNKSIZE bytes as initial preparation */

  return 0;
}

/*
 * malloc
 */
void *malloc (size_t size) {
  return NULL;
}

/*
 * free
 */
void free (void *ptr) {
  if(!ptr) return;
}

/*
 * realloc - you may want to look at mm-naive.c
 */
void *realloc(void *oldptr, size_t size) {
  return NULL;
}

/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *calloc (size_t nmemb, size_t size) {
  return NULL;
}

/* extend_heap - extend the heap with free block and return the block pointer */
static void * extend_heap(size_t words) {
  char * bp;
  size_t size;
  
  /* Allocate an even number of words to maintain alignment */
  size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
  if ((bp = mem_sbrk(size)) == (char *)-1)
    return NULL;

  /* Initiate the header and footer of the new free block and update epilogue */
  PUT(HDRP(bp), PACK(size, 0));        /* New free block header */
  PUT(FTRP(bp), PACK(size, 0));        /* New free block footer */
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0,1)); /* New epilogue header */

  
}

/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static int in_heap(const void *p) {
  return p <= mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static int aligned(const void *p) {
  return (size_t)ALIGN(p) == (size_t)p;
}

/*
 * mm_checkheap
 */
void mm_checkheap(int lineno) {
}
