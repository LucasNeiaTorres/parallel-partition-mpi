// Autores: Lucas Néia Torres, Inaiê Moscal Zambonin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "mpi.h"
#include "verifica_particoes.h"
#include "chrono.h"

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
    int *processPos = (int *)calloc(nP, sizeof(int));

    for (int i = 0; i < n; i++) {
        int posL = bsearch_lower_bound(P, 0, nP, Input[i]);
        
    }
    free(processPos);
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
    int n, nO, processId;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    srand(2024 * 100 + processId);
    
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

    if (processId == 0) {
        geraNaleatorios(P, nP);
        qsort(P, nP-1, sizeof(long long), compare);
        P[nP-1] = LLONG_MAX;
    }

    MPI_Bcast(P, nP, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    
    Output = malloc(sizeof(long long) * nP * n);

    multi_partition_mpi(Input, n, P, nP, Output, &nO); 
    
    free(Input);
    free(P);
    free(Output);
    return 0;
}