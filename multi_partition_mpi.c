// Autores: Lucas Néia Torres, Inaiê Moscal Zambonin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "mpi.h"
#include "verifica_particoes.h"
#include "chrono.h"
#define NTIMES 10

int nTotalElements;
int nP;

long long geraAleatorioLL() {
    int a = rand();  // Returns a pseudo-random integer
                //    between 0 and RAND_MAX.
    int b = rand();  // same as above
    long long v = (long long)a * 100 + b;
    return v;
}

void geraNaleatorios(long long v[], int n)
{
    for (int i = 0; i < n; i++)
    {
        v[i] = geraAleatorioLL();
    }
}

int compare(const void *a, const void *b)
{
    long long arg1 = *(const long long *)a;
    long long arg2 = *(const long long *)b;

    if (arg1 < arg2)
        return -1;
    if (arg1 > arg2)
        return 1;
    return 0;
}

void print_array_long_long(long long *arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%lld ", arr[i]);
    }
    printf("\n");
}

int bsearch_lower_bound(long long *input, int left, int right, long long x)
{
    while (left < right)
    {
        int m = left + (right - left) / 2;
        if (input[m]-1 < x)
            left = m + 1;
        else
            right = m;
    }
    return left;
}

void multi_partition_mpi( long long *Input, int n, long long *P, int np, long long *Output, int *nO) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *processPos = (int *)calloc(np, sizeof(int));
    int nTotal = n * np;
    long long *processResult = malloc(sizeof(long long) * nTotal);

    for (int i = 0; i < np; i++) 
        for (int j = 0; j < n; j++) 
            processResult[i * n + j] = -1;

    long long *received = malloc(sizeof(long long) * nTotal);

    for (int i = 0; i < np; i++) 
        processPos[i] = 0;

    for (int i = 0; i < n; i++) {
        int posL = bsearch_lower_bound(P, 0, np, Input[i]);
        processResult[posL * n + processPos[posL]] = Input[i];
        processPos[posL]++;
    }

    MPI_Alltoall(processPos, n, MPI_LONG_LONG, received, n, MPI_LONG_LONG, MPI_COMM_WORLD);

    int tamOutput = 0;
    for (int i = 0; i < nTotal; i++) {
        if (received[i] != -1) {
            Output[tamOutput] = received[i];
            tamOutput++;
        }
    }
    *nO = tamOutput;

    free(processPos);
    free(processResult);
    free(received);
    MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char *argv[]) {
    int verify, debug = 0;
    
    if (argc < 2) {
        printf("mpirun -np <nProcesses> ./multi_partition_mpi <nTotalElements> (-v) (-d)\n");
        return 0;
    } else if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-v") == 0) {
                verify = 1;
            }
            else if (strcmp(argv[i], "-d") == 0) {
                debug = 1;
            } else {
                printf("Usage: mpirun -np <nProcesses> ./multi-partition <nTotalElements> (-v) (-d)\n");
                return 0;
            }
        }
    }
    long long *Input, *P, *Output;
    int n, nO, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nP);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    nTotalElements = atoi(argv[1]);
    srand(2024 * 100 + rank);
    
    n = nTotalElements / nP;

    Input = (long long *) malloc(n * sizeof(long long));
    if (Input == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para Input\n");
        return 1;
    }

    P = malloc(nP * sizeof(long long));
    if (P == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para P\n");
        free(Input);    
        return 1; 
    }

    geraNaleatorios(Input, n);

    if (rank == 0) {
        geraNaleatorios(P, nP);
        qsort(P, nP-1, sizeof(long long), compare);
        P[nP-1] = LLONG_MAX;
    }

    MPI_Bcast(P, nP, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    
    if (debug) {
        printf("Input array: ");
        print_array_long_long(Input, n);

        printf("Partition array: ");
        print_array_long_long(P, nP);
        printf("\n");
    }

    Output = malloc(sizeof(long long) * nP * n);

    chronometer_t ptTime;
    chrono_reset(&ptTime);
    chrono_start(&ptTime);
    for (int i = 0; i < NTIMES; i++) {
        multi_partition_mpi(Input, n, P, nP, Output, &nO); 
    }

    if (debug) {
        printf("Processo %d output array: ", rank);
        print_array_long_long(Output, nO);
        printf("\n");
    }

    chrono_stop(&ptTime);

    double total_time_in_seconds = ((double) chrono_gettotal(&ptTime)) / (1000 * 1000 * 1000);
    printf("Total time: %lf s\n", total_time_in_seconds);
    double average_time = total_time_in_seconds / (NTIMES);
    printf("Average time: %lf s\n", average_time);
                                  
    double eps = n * NTIMES / total_time_in_seconds;
    double megaeps = eps/1000000;
    printf("Throughput: %lf MEPS/s\n", megaeps);

    if (verify) {
        verifica_particoes(Input, n, P, nP, Output, &nO);
    }

    free(Input);
    free(P);
    free(Output);

    MPI_Finalize();
    return 0;
}