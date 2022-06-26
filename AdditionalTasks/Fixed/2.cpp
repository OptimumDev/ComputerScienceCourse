#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


void fill_zero(long int *arr, long int count)
{
    int i;
    for(i=0; i<count; i++)
    {
		arr[i] = 0;
    }
}

void fill_rand(long int *arr, long int count, long int range)
{
    srand(time(NULL));
    int i;
    for(i=0; i<count; i++)
    {
		arr[i] = rand() % (range);	
    }
}

int main()
{
    const long int Nhist = 500;
    const long int Nupd = 10*1000*1000;
    long int *hist1, *upd;
    double time;
    hist1 = (long int*) malloc(Nhist*sizeof(long int));
    upd = (long int*) malloc(Nupd*sizeof(long int));
    int NumThreads = omp_get_num_procs();
    printf("Thread count is: %d\n", NumThreads);

    fill_zero(hist1, Nhist);
    fill_rand(upd, Nupd, Nhist);

    long int i;
    time = omp_get_wtime();
    for(i=0; i<Nupd; i++)
    {
		hist1[upd[i]]++;
    }
    time = omp_get_wtime() - time;
    printf("Serial time: %f\n", time);

    int hist2[Nhist] = {0};
    time = omp_get_wtime();
    #pragma omp parallel for reduction(+:hist2[:])
    for(i=0; i<Nupd; i++)
    {
		hist2[upd[i]]++;	
    }
    time = omp_get_wtime() - time;
    printf("Parallel time: %f\n", time);

    for(i=0; i<Nhist; i++)
    {
		if(hist1[i] != hist2[i])
		{
			printf("Error!\n");
			return 0;
		}
    }
	printf("OK!\n");
    
    free(hist1);
//    free(hist2);
    free(upd);
    return 0;
}
