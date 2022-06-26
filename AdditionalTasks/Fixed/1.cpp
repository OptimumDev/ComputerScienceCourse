#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


int main()
{
    const long int N = 300*1000*1000;
    double time;
    int NumThreads = omp_get_num_procs();
    
    double *a1, *a2;
    a1 = (double*) malloc(N*sizeof(double));
    a2 = (double*) malloc(N*sizeof(double));

    long int i;
    for(i=0; i<N; i++)
    {
		a1[i] = (double) i;
		a2[i] = (double) i;
    }
    
    printf("Threads count is: %d\n", NumThreads);
    
    time = omp_get_wtime();
    for(i=0; i<N; i++)
    {
		a1[i] = sqrt(a1[i]);
    }
    time = omp_get_wtime() - time;
    printf("Serial time: %f\n", time);

    time = omp_get_wtime();
#pragma omp parallel for
    for(i=0; i<N; i++)
    {
		a2[i] = sqrt(a2[i]);
    }
    time = omp_get_wtime() - time;
    printf("Parallel time: %f\n", time);

    bool isOk = true;
    for(i=0; i<N; i++)
    {
        if(a1[i] != a2[i])
        {
            isOk = false;
            break;
        }
    }

    if (isOk)
    {
        printf("OK!\n");
    }
    else
    {
        printf("Error!\n");
    }

    free(a1);
    free(a2);
    return 0;
}