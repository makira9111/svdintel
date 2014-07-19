#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include </System/Library/Frameworks/Accelerate.framework/Headers/Accelerate.h>

#define LEN 10000000
/* SGESVD prototype */
extern void sgesvd( char* jobu, char* jobvt, int* m, int* n, float* a,
                   int* lda, float* s, float* u, int* ldu, float* vt, int* ldvt,
                   float* work, int* lwork, int* info );
/* Auxiliary routines prototypes */
extern void print_matrix( char* desc, int m, int n, float* a, int lda );

//int rand();

/*array allocation*/
extern double **alloc_array(int rows, int columns);

double* allocate_mem(double*** arr, int n, int m);

//free memory
void deallocate_mem(int*** arr, int* arr_data);


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
    
    //double* arr = malloc(M*N*LEN);
    float *arr;
    arr = (float *)malloc(sizeof(double)*tot+1);
    //allocate_mem( arr, 10, 10);
    
    int i,j;
    
    //double **mat = alloc_array(M, N);
   /* double mat[M][N];
    
    allocate_mem(mat,  1000,  1000);
    */
    /*
    double **mat;
	mat = malloc(M * sizeof(int *));
	if(mat == NULL)
    {
		fprintf(stderr, "out of memory\n");
		exit;
    }
	for(i = 0; i < M; i++)
    {
		mat[i] = malloc(N * sizeof(int));
		if(mat[i] == NULL)
        {
			fprintf(stderr, "out of memory\n");
			exit;
        }
    }
    */
    
    
    // (2) N mallocs, one for each row, plus one malloc for array of row
    //     arrays.  The bucket locations in individual rows are contiguous,
    //     but rows are not necessarily contiguous in heap space.
    //
    float **mat; // an array of int arrays (a pointer to pointers to ints)
    
    // declaration and allocation:
    
    // allocate an array of N pointers to ints
    // malloc returns the address of this array (a pointer to (int *)'s)
   mat = (float **)malloc(sizeof(int *)*N*M+1);
    
    // for each row, malloc space for its buckets and add it to
    // the array of arrays
    for(i=0; i < N; i++) {
        mat[i] = (float *)malloc(sizeof(int)*M*N+1);
    }
    
    // in memory:
    // 2d_array -----> | *-|-------> [ 0, 0, 0, ... , 0]  row 0
    //                 | *-|-------> [ 0, 0, 0, ... , 0]  row 1
    //                 |...|                               ...
    //                 | *-|-------> [ 0, 0, 0, ... , 0]
    
    // access using [] notation:
    //  2d_array[i] is ith bucket in 2d_array, which is the address of
    // a 1d array, on which you can use indexing to access its bucket value
    for(i=0; i < N; i++) {
        for(j=0; j < M; j++) {
            mat[i][j] = rand1()%10;
        }
    }
    
    /*// access using pointer arithmetic (NOT all N*M buckets are contigous)
    // but each row's buckets are
    int *ptr;
    for(i=0; i < N; i++) {
        *ptr = mat[i];  // set pointer to address of bucket 0 in row i
        for(j=0; j < M; j++) { 
            *ptr = 0; 
            ptr++;
        }
    }
    */
    
  /* for (i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
        {
            int a = rand1()%10+1;
            mat[i][j]=a;
        }
    }
 */

    printf("printing 2-D matrix: \n");
    
    for (int row=0; row<M; row++)
    {
        for(int columns=0; columns<N; columns++)
            printf("%0.2f  ", mat[row][columns]);
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

    for (i = 0; i < M*N; i++) {
        arr[i] = mat[i/(N)][i%N]; // <<-- row-major order
        //arr[i] = mat[i%M][i/M]; // <<-- column-major order
    }

   // mat = (float **)malloc(sizeof(int *)*N);
    
    // for each row, malloc space for its buckets and add it to
    // the array of arrays
    for(i=M; i < 1; i--) {
      free ((void*)mat[i]);
    }
    //free(mat);
    
    //memset(mat, 0, sizeof(int));
    //free(mat);
    
    printf("\nconvert into 1d-matrix:\n");
    
    for(int k=0;k<(tot);k++)
    {
        printf("%0.2f ",arr[k]);
        //free(mat);
    }
    //free(mat);
 
    
    
    
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
    void deallocate_mem(int*** arr, int* arr_data);
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

/*array allocation function*/

double **alloc_array(int rows, int columns)
{
    int i;
    int j;
    double **twoDary = (double**) (malloc(rows * sizeof(int *)));
   // int **twoDaryStart = twoDary;
    double *currentrow;
    
    for ( i = 0; i < rows; i++ ){
        *(twoDary + i) =  (malloc(columns * sizeof(int)));
    }
    
    for (j = 0; j < rows; j++) {
        currentrow = *(twoDary + j);
        for ( i = 0; i < columns; i++ ) {
            *(currentrow + i) = i;
           // printf("%d ", *(currentrow+i));
    }
    }
    
    return twoDary;
}
//memory allocation function

double* allocate_mem(double*** arr, int n, int m)
{
    *arr = (double**)malloc(n * sizeof(int*));
    double *arr_data = malloc( n * m * sizeof(int));
    for(int i=0; i<n; i++)
        (*arr)[i] = arr_data + i * m ;
    return arr_data; //free point
}

//free memory
void deallocate_mem(int*** arr, int* arr_data){
    free(arr_data);
    free(*arr);
}
