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
int rand1()
{
    srand((int)time(0));
    
    rand_seed = rand_seed * 1103515245 +12345*rand();
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
    
    double* arr = malloc(M * N * sizeof(double));

    int i,j,k;
    double mat[M][N];
    
    for (i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
        {
            mat[i][j]=rand1()%10 +1;
        }
    }
    
    printf("\nprinting 2-D matrix: \n");
    
    for (int row=0; row<M; row++)
    {
        for(int columns=0; columns<N; columns++)
            printf("%0.4f  ", mat[row][columns]);
        printf("\n");
    }

    int LDA = M;
    int LDU = M;
    int LDVT = N;
    
    int m = M, n = N, lda = LDA, ldu = LDU, ldvt = LDVT, info, lwork;
    float wkopt;
    float* work;
    
    
    /* Local arrays */
    float s[N], u[LDU*M], vt[LDVT*N];
/*
    for(i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
            arr[k++]=mat[i][j];
    }
  */
   // double array[M * N];
    
    for (i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
        {
            arr[N * i + j] = mat[i][j];
        }
    }
    
    
    free(arr);
    
    printf("\nconvert into 1d-matrix:\n");
    
    for(k=0;k<(tot);k++)
        
        printf("%0.4f ",arr[k]);
  
    
   // free(arr);
    
    
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


