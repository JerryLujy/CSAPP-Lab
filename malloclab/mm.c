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
//#define DEBUG
//#define VIEW_HEAP
#define VIEW_FREE_LIST
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
# define checkheap(l) mm_checkheap(l)
#else
# define dbg_printf(...)
# define checkheap(l)
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
#define CHUNKSIZE (1 << 8) /* Extend heap by at least this amount */

/* Pack a size and allocate bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Given ptr p, read and write a word at this address */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (GET(p) = (val))

/* Given ptr p, write at this address but keep predalloc info untouched */
#define PUT_SOFT(p, val) (GET(p) = ((GET(p) & 0x2) | (val)))

/* Given ptr p, read size, alloc and predalloc info from this address */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_PRED_ALLOC(p) (GET(p) & 0x2)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of predecessor and successor blocks */
#define SUCC_BLKP(bp) ((char *)(bp) + GET_SIZE((char *)(bp) - WSIZE))
#define PRED_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))

/* Given block ptr bp, set or reset the predalloc info in successor block */
#define SET_SUCC_PREDALLOC(bp) (GET(HDRP(SUCC_BLKP(bp))) |= 0x2)
#define RESET_SUCC_PREDALLOC(bp) (GET(HDRP(SUCC_BLKP(bp))) &= ~0x2)

/* Given (free) block ptr bp, encode prev and next free list block address here */
#define SET_NEXT_FREE_BLKP(bp, ptr) PUT(bp, ptr_to_offst(ptr))
#define SET_PREV_FREE_BLKP(bp, ptr) PUT((bp + WSIZE), ptr_to_offst(ptr))

/* Given (free) block ptr bp, decode prev and next free list block address */
#define GET_NEXT_FREE_BLKP(bp) offst_to_ptr(GET(bp))
#define GET_PREV_FREE_BLKP(bp) offst_to_ptr(GET(bp + WSIZE))

/* Global variables */
static char * heap_listp = NULL;     /* Pointer to first block */
static char * free_list_hp = NULL;   /* Pointer to head of free list */
static char * free_list_tp = NULL;   /* Pointer to tail of free list */

/* Helper functions */
static void * extend_heap(size_t words);
static void * coalesce(void * bp);
static void * find_fit(size_t asize);
static void place(void * bp, size_t asize);
static inline void insert_free_block(char * bp);
static inline void delete_free_block(char * bp);

/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) { 
  dbg_printf("\n***** Init Request *****\n");
  /* Reset global pointers */
  heap_listp = NULL;
  free_list_hp = NULL;
  free_list_tp = NULL;
  
  /* Create an empty heap */
  if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
    return -1;

  PUT(heap_listp, 0);                           /* Alignment padding */
  PUT(heap_listp +     WSIZE, PACK(DSIZE, 1));  /* Prologue header */
  PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));  /* Prologue footer */
  PUT(heap_listp + 3 * WSIZE, PACK(0,     1));  /* Epilogue header */

  heap_listp += 2 * WSIZE;

  /* Set the predalloc info of the prologue block */
  SET_SUCC_PREDALLOC(heap_listp);
  
  /* Extend the heap with CHUNKSIZE bytes as initial preparation */
  if (extend_heap(CHUNKSIZE) == NULL)
    return -1;

  checkheap(__LINE__);

  return 0;
}

/*
 * malloc
 */
void * malloc (size_t size) {
  size_t asize;       /* Adjusted block size */
  size_t esize;       /* Size to extend if no fit could be found */
  char * bp;

  if (heap_listp == NULL)
    if (mm_init() != 0)
      return NULL;
  
  /* Ignore spurious requests */
  if (size == 0)
    return NULL;

  /* Adjust block to include overhead and alignment requirement */
  if (size <= DSIZE)
    asize = 2 * DSIZE;
  else
    /* Overhead for allocated block is WSIZE */
    asize = ALIGN(size + WSIZE);

  dbg_printf("\n***** Malloc Request (size = %zu, round to %zu) *****\n", size, asize);
  /* Search the free list for a fit */
  if ((bp = find_fit(asize)) != NULL) {
    dbg_printf("Found fit at (%p)\n", bp);

    place(bp, asize);

    checkheap(__LINE__);
    return bp;
  }

  /* No fit found in the free list. Extend the heap */
  esize = MAX(asize, CHUNKSIZE);
  if ((bp = extend_heap(esize)) == NULL)
    return NULL;
  dbg_printf("Extended heap by %zu\n", esize);

  place(bp, asize);

  checkheap(__LINE__);

  return bp;
}

/*
 * free
 */
void free (void * bp) {
  if(!bp)
    return;
  
  size_t size = GET_SIZE(HDRP(bp));
  
  dbg_printf("\n***** Free Request (ptr = %p, size = %zu) *****\n", bp, size);

  PUT_SOFT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  RESET_SUCC_PREDALLOC(bp);    /* set the successor block's predalloc info to be 0 */
  insert_free_block(bp);

  coalesce(bp);

  checkheap(__LINE__);
}

/*
 * realloc
 *
 * Change the size of an allocated block pointed to by ptr to size
 */
void * realloc(void * ptr, size_t size) {
  /* Null ptr equivalent to malloc(size) */
  if (ptr == NULL) {
    dbg_printf("\n***** Realloc Request (NULL, %zu) Do malloc instead *****\n", size);
    return malloc(size);
  }
  /* Zero size equivalent to free(ptr) */
  if (size == 0) {
    dbg_printf("\n***** Realloc Request (ptr, 0) Do free instead *****\n");
    free(ptr);
    return NULL;
  }
  
  size_t oldsize = GET_SIZE(HDRP(ptr));
  size_t asize;
  
  if (size <= DSIZE)
    asize = 2 * DSIZE;
  else
    asize = ALIGN(size + WSIZE);
  dbg_printf("\n***** Realloc Request (ptr = %p, oldsize = %zu, "
	     "new size = %zu, round to %zu) *****\n",
	     ptr, oldsize, size, asize);

  if (oldsize >= asize) {
    /* Realloc is shrinking the block size */
    if ((oldsize - asize) > 2 * DSIZE) {
      /* Enough space for another block. Split it */
      if (!GET_ALLOC(HDRP(SUCC_BLKP(ptr)))) {
	/* Successor block is free, "coalesce" */
	oldsize += GET_SIZE(HDRP(SUCC_BLKP(ptr)));
	delete_free_block(SUCC_BLKP(ptr));
      }
      PUT_SOFT(HDRP(ptr), PACK(asize, 1));
      void * freebp = SUCC_BLKP(ptr);
      PUT(HDRP(freebp), PACK(oldsize - asize, 0));
      PUT(FTRP(freebp), PACK(oldsize - asize, 0));
      SET_SUCC_PREDALLOC(ptr);
      insert_free_block(freebp);
    } else {
      /* Not enough space */
      PUT_SOFT(HDRP(ptr), PACK(oldsize, 1));
      SET_SUCC_PREDALLOC(ptr);
    }
    checkheap(__LINE__);
    return ptr;
  }
  else if (!GET_ALLOC(HDRP(SUCC_BLKP(ptr))) &&
	   GET_SIZE(HDRP(SUCC_BLKP(ptr))) + oldsize > asize) {
    /* Realloc is expanding the block size
     * But successor block is free and big enough */
    char * next = SUCC_BLKP(ptr);
    delete_free_block(next);

    size_t freesize = GET_SIZE(HDRP(next)) + oldsize - asize;
    /* Same thing as above */
    if (freesize > 2 * DSIZE) {
      PUT_SOFT(HDRP(ptr), PACK(asize, 1));
      void * freebp = SUCC_BLKP(ptr);
      PUT(HDRP(freebp), PACK(freesize, 0));
      PUT(FTRP(freebp), PACK(freesize, 0));
      SET_SUCC_PREDALLOC(ptr);
      insert_free_block(freebp);
    } else {
      PUT_SOFT(HDRP(ptr), PACK(GET_SIZE(HDRP(next)) + oldsize, 1));
      SET_SUCC_PREDALLOC(ptr);
    }
    checkheap(__LINE__);
    return ptr;
  }
  
  /* If we reach here, need to malloc and free */
  dbg_printf("size is %zu\n", size);
  void * newptr = malloc(size);
  memcpy(newptr, ptr, size);
  free(ptr);

  checkheap(__LINE__);
  
  return newptr;
}

/*
 * calloc
 *
 * Allocates memory for an array of nmemb elements, each of size bytes
 * Memory is set to zero
 */
void * calloc (size_t nmemb, size_t size) {
  size_t total = nmemb * size;
  void * newptr = malloc(total);
  memset(newptr, 0, total);
  return newptr;
}

/* Convert back and forth between 64-bit pointer and 
 * 32-bit offset value w.r.t. heap first block pointer
 * (Given that the size of heap is at most 2^32 bytes)
 */
static inline unsigned int ptr_to_offst(char * ptr) {
  return (ptr == NULL) ? 0 : (unsigned int)(ptr - heap_listp);
}
static inline char * offst_to_ptr(unsigned int offst) {
  return (offst == 0) ? NULL : (char *)(offst + (unsigned long)heap_listp);
}

/* 
 * extend_heap
 *
 * Extend the heap with free block of size s and return the block pointer 
 */
static void * extend_heap(size_t s) {
  char * bp;
  size_t size;
  
  /* Align incoming size to maintain alignment */
  size = ALIGN(s);
  if ((bp = mem_sbrk(size)) == (char *)-1)
    return NULL;

  /* Initiate the header and footer of the new free block and update epilogue */
  PUT_SOFT(HDRP(bp), PACK(size, 0));   /* New free block header */
  PUT(FTRP(bp), PACK(size, 0));        /* New free block footer */
  PUT(HDRP(SUCC_BLKP(bp)), PACK(0,1)); /* New epilogue header */

  insert_free_block(bp);

  return coalesce(bp);
}

/*
 * coalesce
 *
 * Coalesce adjacent free blocks. Return ptr to coalesced block
 */
static void * coalesce(void * bp) {
  void * succ = SUCC_BLKP(bp);
  /* Get alloc info of successor and predecessor blocks */
  size_t pred_allocd = GET_PRED_ALLOC(HDRP(bp));
  size_t succ_allocd = GET_ALLOC(HDRP(succ));
  size_t size = GET_SIZE(HDRP(bp));

  if (pred_allocd) {/* Predecessor block is allocated */
    if (!succ_allocd) {/* Successor block is free */
      size += GET_SIZE(HDRP(succ));
      delete_free_block(succ);
      delete_free_block(bp);
      PUT_SOFT(HDRP(bp), PACK(size, 0));
      PUT(FTRP(bp), PACK(size, 0));
      insert_free_block(bp);
    }
  } else {/* Predecessor block is free */
    char * pred = PRED_BLKP(bp);
    size += GET_SIZE(HDRP(pred));
    if (succ_allocd) {/* Successor block is allocated */
      delete_free_block(pred);
      delete_free_block(bp);
      bp = pred;
      PUT_SOFT(HDRP(bp), PACK(size, 0));
      PUT(FTRP(bp), PACK(size, 0));
      insert_free_block(bp);
    } else {/* Successor block is free */
      size += GET_SIZE(HDRP(succ));
      delete_free_block(pred);
      delete_free_block(succ);
      delete_free_block(bp);
      bp = pred;
      PUT_SOFT(HDRP(bp), PACK(size, 0));
      PUT(FTRP(bp), PACK(size, 0));
      insert_free_block(bp);
    }
  }
  return bp;
}

/*
 * find_fit
 *
 * Find a block with at least asize bytes
 */
static void * find_fit(size_t asize) {
  void * bp;
  for (bp = free_list_hp; bp != NULL; bp = GET_NEXT_FREE_BLKP(bp)) {
    if (!GET_ALLOC(HDRP(bp)) && asize <= GET_SIZE(HDRP(bp)))
      return bp;
  }
  return NULL;
}

/*
 * place
 *
 * Place block of asize bytes at the start of free block bp
 * Split the block if there is enough remaining block size
 */
static void place(void * bp, size_t asize) {
  size_t bsize = GET_SIZE(HDRP(bp));    /* original block size */

  if ((bsize - asize) >= 2 * DSIZE) {
    /* Enough space for another block. Split it */
    PUT_SOFT(HDRP(bp), PACK(asize, 1));
    delete_free_block(bp);
    void * freebp = SUCC_BLKP(bp);
    PUT(HDRP(freebp), PACK(bsize - asize, 0));
    PUT(FTRP(freebp), PACK(bsize - asize, 0));
    SET_SUCC_PREDALLOC(bp);
    insert_free_block(freebp);
  } else {
    /* Not enough space. Mark entire block as allocated */
    PUT_SOFT(HDRP(bp), PACK(bsize, 1));
    SET_SUCC_PREDALLOC(bp);
    /* Update pointers in free list */
    char * prevp = GET_PREV_FREE_BLKP(bp);
    char * nextp = GET_NEXT_FREE_BLKP(bp);
    if (prevp != NULL)
      SET_NEXT_FREE_BLKP(prevp, nextp);
    if (nextp != NULL)
      SET_PREV_FREE_BLKP(nextp, prevp);
    delete_free_block(bp);
  }
}

/* 
 * insert_free_block
 *
 * Insert a new free block pointed to by bp to the free list.
 */
//#define ADDRESS_BASED_LIST
static void insert_free_block(char * bp) {
  if (free_list_hp == NULL) {
    free_list_hp = bp;
    free_list_tp = bp;
    /* In case there is garbage stored in block */
    SET_NEXT_FREE_BLKP(bp, NULL);
    SET_PREV_FREE_BLKP(bp, NULL);
    return;
  }
#ifdef ADDRESS_BASED_LIST
  if (bp < free_list_hp) {/* insert this block at the front of the free list */
#endif
    SET_NEXT_FREE_BLKP(bp, free_list_hp);
    SET_PREV_FREE_BLKP(free_list_hp, bp);
    free_list_hp = bp;
    SET_PREV_FREE_BLKP(bp, NULL);// In case there is garbage
#ifdef ADDRESS_BASED_LIST
  } else if (bp > free_list_tp) {/* insert this block at the end of the free list */
    SET_NEXT_FREE_BLKP(free_list_tp, bp);
    SET_PREV_FREE_BLKP(bp, free_list_tp);
    free_list_tp = bp;
    SET_NEXT_FREE_BLKP(bp, NULL);// In case there is garbage
  } else {/* insert this block somewhere in the free list */
    char * temp;
    if (((long)bp - (long)free_list_hp) < ((long)free_list_tp - (long)bp)) {
      /* bp is closer to free list head. Start searching from head */
      temp = free_list_hp;
      while (temp < bp)
	temp = GET_NEXT_FREE_BLKP(temp);
    } else {
      /* bp is closer to free list tail. Start searching from tail */
      temp = free_list_tp;
      while (temp > bp)
	temp = GET_PREV_FREE_BLKP(temp);
      temp = GET_NEXT_FREE_BLKP(temp);
    }
    SET_NEXT_FREE_BLKP(bp, temp);
    SET_PREV_FREE_BLKP(bp, GET_PREV_FREE_BLKP(temp));
    SET_NEXT_FREE_BLKP(GET_PREV_FREE_BLKP(temp), bp);
    SET_PREV_FREE_BLKP(temp, bp);
  }
#endif
}

/*
 * delete_free_block
 *
 * Delete the free block pointed to by bp from the free list
 */
static void delete_free_block(char * bp) {
  if (free_list_hp == free_list_tp) {/* Only one block in the free list */
    free_list_hp = free_list_tp = NULL;
  } else if (bp == free_list_hp) {/* Removing head of the free list */
    free_list_hp = GET_NEXT_FREE_BLKP(bp);
    SET_PREV_FREE_BLKP(GET_NEXT_FREE_BLKP(bp), NULL);
  } else if (bp == free_list_tp) {/* Removing tail of the free list */
    free_list_tp = GET_PREV_FREE_BLKP(bp);
    SET_NEXT_FREE_BLKP(GET_PREV_FREE_BLKP(bp), NULL);
  } else {
    SET_NEXT_FREE_BLKP(GET_PREV_FREE_BLKP(bp), GET_NEXT_FREE_BLKP(bp));
    SET_PREV_FREE_BLKP(GET_NEXT_FREE_BLKP(bp), GET_PREV_FREE_BLKP(bp));
  }
}

/**********************************
 * Helper functions for debugging *
 **********************************/

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
static int aligned(const void * p) {
  return (size_t)ALIGN(p) == (size_t)p;
}

/*
 * print_block
 *
 * Print the info of the block
 */
static void print_block(void * bp) {
  size_t hsize, halloc, fsize, falloc;
  hsize = GET_SIZE(HDRP(bp));
  halloc = GET_ALLOC(HDRP(bp));
  fsize = GET_SIZE(FTRP(bp));
  falloc = GET_ALLOC(FTRP(bp));
  if (hsize == 0) {
    printf("                (%p): EOL\n", bp);
    return;
  }
  if (halloc)
    printf(" Allocated (%p): header[%5zu|%c|%c]\n", 
	   bp, hsize, (GET_PRED_ALLOC(HDRP(bp)) ? 'a' : 'f'), (halloc ? 'a' : 'f'));
  else
    printf("      Free (%p): header[%5zu|%c|%c] footer[%5zu|%c] next(%p) prev(%p)\n",
	   bp, hsize, (GET_PRED_ALLOC(HDRP(bp)) ? 'a' : 'f'), (halloc ? 'a' : 'f'),
	   fsize, (falloc ? 'a' : 'f'), GET_NEXT_FREE_BLKP(bp), GET_PREV_FREE_BLKP(bp));
}

/*
 * mm_checkheap
 *
 * Check all the invariants in the heap
 */
void mm_checkheap(int lineno) {
  char * p = heap_listp;
  /* Number of free blocks counted by walking through heap and free list */
  int free_block_count_h = 0, free_block_count_fl = 0;

  /* Check epilogue block */
#ifdef VIEW_HEAP
    printf("==heap==");
    print_block(p);
#endif
  if (GET_SIZE(HDRP(p)) != DSIZE || !GET_ALLOC(HDRP(p))) {
    printf("ERROR (line %d): bad epilogue header: %d\n", lineno, GET(HDRP(p)));
  }
  if (GET_SIZE(FTRP(p)) != DSIZE || !GET_ALLOC(FTRP(p))) {
    printf("ERROR (line %d): bad epilogue footer: %d\n", lineno, GET(FTRP(p)));
  }
  
  /* Check heap */
  p = SUCC_BLKP(p);
  for ( ; GET_SIZE(HDRP(p)) > 0; p = SUCC_BLKP(p)) {

#ifdef VIEW_HEAP
    printf("==heap==");
    print_block(p);
#endif
    /* Check block p */
    size_t size = GET_SIZE(HDRP(p));
    size_t allocd = GET_ALLOC(HDRP(p));
    if (!allocd)
      free_block_count_h++;
    
    if (!aligned((void *)size) || size < 2 * DSIZE)
      printf("ERROR (line %d): wrong block size (%zu)\n", lineno, size);
    if (!aligned(p))
      printf("ERROR (line %d): %p is not double word aligned\n", lineno, p);
    /* Header and footer matching if free block */
    if (!allocd && ((GET(HDRP(p)) & ~0x2) != (GET(FTRP(p)) & ~0x2)) )
      printf("ERROR (line %d): header (%u) does not match footer (%u)\n", lineno,
	     GET(HDRP(p)), GET(FTRP(p)));
    /* Alloc bit and next block predalloc bit consistency */
    if (allocd != GET_PRED_ALLOC(HDRP(SUCC_BLKP(p))) >> 1)
      printf("ERROR (line %d): alloc bit does not match successor predalloc bit\n", 
	     lineno);
    /* No consecutive free blocks in the heap */
    if (!allocd && !GET_ALLOC(HDRP(SUCC_BLKP(p))))
      printf("ERROR (line %d): consecutive free blocks afterwards\n", lineno);
  }

  /* Check prologue block */
#ifdef VIEW_HEAP
  printf("==heap==");
  print_block(p);
#endif  
  if (GET_SIZE(HDRP(p)) != 0 || !GET_ALLOC(HDRP(p)))
    printf("ERROR (line %d): bad prologue (%d)\n", lineno, GET(HDRP(p)));

  /* Check free list */
#ifdef VIEW_FREE_LIST
  printf("-----Free list: head (%p) tail (%p)-----\n", free_list_hp, free_list_tp);
#endif
  
  p = free_list_hp;
  while (p) {
    free_block_count_fl++;
    /* Pointer within heap boundary */
    if (!in_heap(p)) {
      printf("ERROR (line %d): free list pointer (%p) out of bound\n", lineno, p);
      return;
    }
#ifdef VIEW_FREE_LIST
    printf("-----");
    print_block(p);
#endif
    
    /* Next/previous pointer consistency */
    char * prevp = GET_PREV_FREE_BLKP(p);
    char * nextp = GET_NEXT_FREE_BLKP(p);

    /* Prev pointer of head should be null */
    if (p == free_list_hp) {
      if (prevp != NULL)
	printf("ERROR (line %d): free list head has non-null prev pointer(%p)\n",
	       lineno, prevp);
    }
    /* Prev block's next pointer should be myself */
    else if (prevp != NULL && GET_NEXT_FREE_BLKP(prevp) != p)
      printf("ERROR (line %d): block (%p) has prev free block with different next pointer (%p)\n",
	     lineno, p, GET_NEXT_FREE_BLKP(prevp));

    /* Next pointer of tail should be null */
    if (p == free_list_tp && nextp != NULL) {
      if (nextp != NULL)
	printf("ERROR (line %d): free list tail has non-null next pointer(%p)\n",
	       lineno, nextp);
    }
    /* Next block's prev pointer should be myself */
    else if (nextp != NULL && GET_PREV_FREE_BLKP(nextp) != p)
      printf("ERROR (line %d): block (%p) has next free block with different prev pointer (%p)\n",
	     lineno, p, GET_PREV_FREE_BLKP(nextp));
    
    p = nextp;
  }

  /* Free blocks counting using two methods should match */
  if (free_block_count_h != free_block_count_fl)
    printf("ERROR (line %d): different free block count by heap (%d) and free list(%d)\n",
	   lineno, free_block_count_h, free_block_count_fl);
}
