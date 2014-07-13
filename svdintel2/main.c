/*******************************************************************************
 *  Copyright (C) 2009-2012 Intel Corporation. All Rights Reserved.
 *  The information and material ("Material") provided below is owned by Intel
 *  Corporation or its suppliers or licensors, and title to such Material remains
 *  with Intel Corporation or its suppliers or licensors. The Material contains
 *  proprietary information of Intel or its suppliers and licensors. The Material
 *  is protected by worldwide copyright laws and treaty provisions. No part of
 *  the Material may be copied, reproduced, published, uploaded, posted,
 *  transmitted, or distributed in any way without Intel's prior express written
 *  permission. No license under any patent, copyright or other intellectual
 *  property rights in the Material is granted to or conferred upon you, either
 *  expressly, by implication, inducement, estoppel or otherwise. Any license
 *  under such intellectual property rights must be express and approved by Intel
 *  in writing.
 *
 ********************************************************************************
 */
/*
 SGESVD Example.
 ==============
 
 Program computes the singular value decomposition of a general
 rectangular matrix A:
 
 8.79   9.93   9.83   5.45   3.16
 6.11   6.91   5.04  -0.27   7.98
 -9.15  -7.93   4.86   4.85   3.01
 9.57   1.64   8.83   0.74   5.80
 -3.49   4.02   9.80  10.00   4.27
 9.84   0.15  -8.99  -6.02  -5.31
 
 Description.
 ============
 
 The routine computes the singular value decomposition (SVD) of a real
 m-by-n matrix A, optionally computing the left and/or right singular
 vectors. The SVD is written as
 
 A = U*SIGMA*VT
 
 where SIGMA is an m-by-n matrix which is zero except for its min(m,n)
 diagonal elements, U is an m-by-m orthogonal matrix and VT (V transposed)
 is an n-by-n orthogonal matrix. The diagonal elements of SIGMA
 are the singular values of A; they are real and non-negative, and are
 returned in descending order. The first min(m, n) columns of U and V are
 the left and right singular vectors of A.
 
 Note that the routine returns VT, not V.
 
 Example Program Results.
 ========================
 
 SGESVD Example Program Results
 
 Singular values
 27.47  22.64   8.56   5.99   2.01
 
 Left singular vectors (stored columnwise)
 -0.59   0.26   0.36   0.31   0.23
 -0.40   0.24  -0.22  -0.75  -0.36
 -0.03  -0.60  -0.45   0.23  -0.31
 -0.43   0.24  -0.69   0.33   0.16
 -0.47  -0.35   0.39   0.16  -0.52
 0.29   0.58  -0.02   0.38  -0.65
 
 Right singular vectors (stored rowwise)
 -0.25  -0.40  -0.69  -0.37  -0.41
 0.81   0.36  -0.25  -0.37  -0.10
 -0.26   0.70  -0.22   0.39  -0.49
 0.40  -0.45   0.25   0.43  -0.62
 -0.22   0.14   0.59  -0.63  -0.44
 */
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include </System/Library/Frameworks/Accelerate.framework/Headers/Accelerate.h>

/* SGESVD prototype */
extern void sgesvd( char* jobu, char* jobvt, int* m, int* n, float* a,
                   int* lda, float* s, float* u, int* ldu, float* vt, int* ldvt,
                   float* work, int* lwork, int* info );
/* Auxiliary routines prototypes */
extern void print_matrix( char* desc, int m, int n, float* a, int lda );

int rand();

/*random number generator*/

int rand_seed=10;
int rand()
{
    rand_seed = rand_seed * 1103515245 +12345;
    return (unsigned int)(rand_seed / 65536) % 32768;
}

/* Main program */
int main() {
    
    /* Locals */
    int M;
    int N;
    int tot;
    
    printf("insert the column number\n");
    scanf("%d", &M);
    printf("insert the row number\n");
    scanf("%d", &N);
    tot=M*N;
    printf("total element is %d \n" , tot) ;
    
    float* arr = malloc(M * N * sizeof(int));

    int i,j,k;
    float mat[M][N];
    
    
    for (i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
        {
            mat[i][j]=rand()%100;
        }
    }
    
    
    
    
    printf("\nprinting 2-D matrix: \n");
    
    //int row, columns;
    for (int row=0; row<M; row++)
    {
        for(int columns=0; columns<N; columns++)
            printf("%0.4f  ", mat[row][columns]);
        printf("\n");
    }
    
    //int LDA M, LDU M, LDVT N;
    int LDA = M;
    int LDU = M;
    int LDVT = N;
    
    int m = M, n = N, lda = LDA, ldu = LDU, ldvt = LDVT, info, lwork;
    float wkopt;
    float* work;
    
    
    /* Local arrays */
    float s[N], u[LDU*M], vt[LDVT*N];

    for(i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
            arr[k++]=mat[i][j];
    }
    
    printf("\nconvert into 1d-matrix:\n");
    
    for(k=0;k<(tot);k++)
        
        printf("%0.4f \n",arr[k]);
    
    free(arr);
    
    
    /* Executable statements */
    printf( "\nSGESVD Example Program Results\n" );
    /* Query and allocate the optimal workspace */
    lwork = -1;
    sgesvd( "All", "All", &m, &n, arr, &lda, s, u, &ldu, vt, &ldvt, &wkopt, &lwork,
           &info );
    lwork = (int)wkopt;
    work = (float*)malloc( lwork*sizeof(float) );
    /* Compute SVD */
    sgesvd( "All", "All", &m, &n, arr, &lda, s, u, &ldu, vt, &ldvt, work, &lwork,
           &info );
    /* Check for convergence */
    if( info > 0 ) {
        printf( "The algorithm computing SVD failed to converge.\n" );
        exit( 1 );
    }
    /* Print singular values */
    print_matrix( "Singular values", 1, n, s, 1 );
    /* Print left singular vectors */
    print_matrix( "Left singular vectors (stored columnwise)", m, n, u, ldu );
    /* Print right singular vectors */
    print_matrix( "Right singular vectors (stored rowwise)", n, n, vt, ldvt );
    /* Free workspace */
    
    free( (void*)work );
    exit( 0 );
} /* End of SGESVD Example */

/* Auxiliary routine: printing a matrix */
void print_matrix( char* desc, int m, int n, float* a, int lda ) {
    int i, j;
    printf( "\n %s\n", desc );
    for( i = 0; i < m; i++ ) {
        for( j = 0; j < n; j++ ) printf( " %6.4f", a[i+j*lda] );
        printf( "\n" );
    }
}


