/*
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

/*
ANALYZE:
The relative position of elements in matrix A storing in the memory
is 32 * i + j, while 32 * j + i for elements in matric B. It means 
the diagonal elements of matrix A and matrix B have the same address,
so they'll be mapped to the same set in direct-mapped cache, which 
causes 32 conflict misses.
To solve the conflict miss, I use 11 local variables. I'll read the
8 consecutive elements in A's row into CPU's registers. Then write the
data into matrix B. Otherwise when writing data into B, it will evict 
the A element simultaneously.
*/
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    REQUIRES(M > 0);
    REQUIRES(N > 0);

    // 32 * 32
    if (M == 32 && N == 32) {
        // instead of B[j][i] = A[i][j];
        int i, j, k;
        int v0, v1, v2, v3, v4, v5, v6, v7;
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (k = i; k < i + 8; k++) {
                    v0 = A[k][j + 0];
                    v1 = A[k][j + 1];
                    v2 = A[k][j + 2];
                    v3 = A[k][j + 3];
                    v4 = A[k][j + 4];
                    v5 = A[k][j + 5];
                    v6 = A[k][j + 6];
                    v7 = A[k][j + 7];

                    B[j + 0][k] = v0;
                    B[j + 1][k] = v1;
                    B[j + 2][k] = v2;
                    B[j + 3][k] = v3;
                    B[j + 4][k] = v4;
                    B[j + 5][k] = v5;
                    B[j + 6][k] = v6;
                    B[j + 7][k] = v7;
                }
            }
        }
    }

    /*
    If we still use 8 * 8, we'll find that in the 64 * 64 matrix, there
    are 64 * 4(for int type data)bytes in one row, so for a 2 ^ 10 bytes
    cache, the conflict miss will occur every four rows. Thus, we use 4 * 4
    block in this case.
    */
    else if (M == 64 && N == 64) {
        int i, j, k;
        int v0, v1, v2, v3;
        for (i = 0; i < N; i += 4) {
            for (j = 0; j < M; j += 4) {
                for (k = i; k < i + 4; ++k) {
                    v0 = A[k][j + 0];
                    v1 = A[k][j + 1];
                    v2 = A[k][j + 2];
                    v3 = A[k][j + 3];

                    B[j + 0][k] = v0;
                    B[j + 1][k] = v1;
                    B[j + 2][k] = v2;
                    B[j + 3][k] = v3;
                }
            }
        }
    }
    // 60 * 68
    else {
        int i, j, k, l;
        for (i = 0; i < N; i += 16) {
            for(j = 0; j < M; j += 16) {
                for (k = i; k < i + 16 && k < N; k++) {
                    for (l = j; l < j + 16 && l < M; l++) {
                        B[l][k] = A[k][l];
                    }
                }
            }
        }
    }
    /*
    This is merely my assignment implementation and obviously it isn't
    the full score answer. By learning more optimized algorithms, misses
    can be further reduced. The main approach is to tackle those on the
    diagonal by copying temporary block, or use greedy algorithm to divide
    the 60 * 68 matrix into several 8 * 8 matrixes and 4 * 4 ones.
    */

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

