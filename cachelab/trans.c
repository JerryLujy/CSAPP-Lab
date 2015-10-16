/* 
 * Jieyu Lu
 * Andrew ID: jieyul1
 *
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  REQUIRES(M > 0);
  REQUIRES(N > 0);

  int Bi, Bj, i, j;
  int ii, temp;

  if (N == 32) {
    for (Bi = 0; Bi < N; Bi += 8) {
      for (Bj = 0; Bj < M; Bj += 8) {
	for (i = Bi; i < Bi + 8; i++) {
	  for (j = Bj; j < Bj + 8; j++) {
	    if (i == j) {
	      ii = i;
	      temp = A[ii][ii];
	    } else {
	      B[j][i] = A[i][j];
	    }
	  }
	  if (Bi == Bj) B[ii][ii] = temp;
	}
      }
    }
  }
  else if (N == 64) {
    // For 64*64 matrix transpose
    for (Bi = 0; Bi < 64; Bi += 8) {
      for (Bj = 0; Bj < 64; Bj += 8) {
	// Find a 8*8 block at the bottom of B that serves as a temporary buffer for 8*8 blocks in A
	// The starting col of this block should be different from Bi and Bj, because
	// otherwise there will always be a conflict miss due to the memory pattern of this 64*64 matrix
	temp = (Bj + 8) % 64;
	if (temp == Bi) temp += 8;
	// When we are at the second last block, use the last block as buffer instead of wrap back to 0;
	if (Bi == 56 && Bj == 48) temp = 56;
	// When we are at the last block, no need to buffer. Directly transpose it.
	if (Bi == 56 && Bj == 56) continue;
	
	/* We now partition the 8*8 block into four 4*4 sub-blocks
	   If we denote block    A[Bj][Bi] : |a1|a2|    B[Bi][Bj] : |b1|b2|
	                                     |a3|a4|                |b3|b4|
	   Then we know that b1 = a1', b2 = a3', b3 = a2', b4 = a4'
	   The sub-blocks are first transferred into the buffer block in B,
	   then transposed and stored to corresponding position in B*/
	// Transfer a1 and a2 to buffer
	for (j = 0; j < 4; j++) {
	  for (i = 0; i < 8; i++) {
	    B[56 + j][temp + i] = A[Bj + j][Bi + i];
	  }
	}
	// Transfer a1' to b1
	for (j = 0; j < 4; j++) {
	  for (i = 0; i < 4; i++) {
	    B[Bi + i][Bj + j] = B[56 + j][temp + i];
	  }
	}
	// Transfer a3 to buffer
	for (j = 4; j < 8; j++) {
	  for (i = 0; i < 4; i++) {
	    B[52 + j][temp + i] = A[Bj + j][Bi + i];
	  }
	}
	// Transfer a3' to b2
	for (j = 4; j < 8; j++) {
	  for (i = 0; i < 4; i++) {
	    B[Bi + i][Bj + j] = B[52 + j][temp + i];
	  }
	}
	// Transfer a4 to buffer
	for (j = 4; j < 8; j++) {
	  for (i = 4; i < 8; i++) {
	    B[52 + j][temp + i - 4] = A[Bj + j][Bi + i];
	  }
	}
	// Transfer a2' to b3
	for (j = 0; j < 4; j++) {
	  for (i = 4; i < 8; i++) {
	    B[Bi + i][Bj + j] = B[56 + j][temp + i];
	  }
	}
	// Transfer a4' to b4
	for (j = 4; j < 8; j++) {
	  for (i = 4; i < 8; i++) {
	    B[Bi + i][Bj + j] = B[52 + j][temp + i - 4];
	  }
	}
      }
    }
    // Deal with the last block separately
    for (Bi = 56; Bi < 64; Bi += 4) {
      for (Bj = 56; Bj < 64; Bj += 4) {
	for (i = Bi; i < Bi + 4; i++) {
	  for (j = Bj; j < Bj + 4; j++) {
	    if (i == j) {
	      ii = i;
	      temp = A[ii][ii];
	    } else {
	      B[j][i] = A[i][j];
	    }
	  }
	  if (Bi == Bj) B[ii][ii] = temp;
	}
      }
    }
  }
  else {
    for (Bi = 0; Bi < N; Bi += 18) {
      for (Bj = 0; Bj < M; Bj += 18) {
	for (i = Bi; (i < Bi + 18) && (i < N); i++) {
	  for (j = Bj; (j < Bj + 18) && (j < M); j++) {
	    if (i == j) {
	      ii = i;
	      temp = A[ii][ii];
	    } else {
	      B[j][i] = A[i][j];
	    }
	  }
	  if (Bi == Bj) B[ii][ii] = temp;
	}
      }
    }
  }
 
  ENSURES(is_transpose(M, N, A, B));
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

