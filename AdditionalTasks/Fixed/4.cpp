#include <stdio.h>
#include <omp.h>

int main()
{
	const int N = 100000;
	int num_prime;
	double time;
	int NumThreads;
#pragma omp parallel
	{
#pragma omp single
		{
			NumThreads = omp_get_num_threads();
			printf("Threads count is: %d\n", NumThreads);
		}
	}

	num_prime = 0;
	time = omp_get_wtime();
	for(int i=2; i<=N; i++)
	{
		int div = 0;
		for(int j=2; j<=i; j++)
		{
			if(i % j == 0)
			{
				div++;
			}
		}
		if(div == 1)
		{
			num_prime++;
		}
	}
	time = omp_get_wtime() - time;
	
	printf("Serial prime numbers: %d\n", num_prime);
	printf("Serial time: %f\n", time);

    num_prime = 0;
    time = omp_get_wtime();
#pragma omp parallel for reduction(+:num_prime)
    for(int i=2; i<=N; i++)
    {
        int div = 0;
        for(int j=2; j<=i; j++)
        {
            if(i % j == 0)
            {
                div++;
            }
        }
        if(div == 1)
        {
            num_prime++;
        }
    }
    time = omp_get_wtime() - time;

    printf("Parallel prime numbers: %d\n", num_prime);
    printf("Parallel time: %f\n", time);
	return 0;
}