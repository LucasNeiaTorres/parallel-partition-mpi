#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "verifica_particoes.h"
#include "chrono.h"

void multi_partition_mpi( long long *Input, int n, long long *P, int np, long long *Output, int *nO) {
    
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("usage: %s <numElemInput> <numElemP> <nThreads>\n", argv[0]);
        return 0;
    // } else if(atoi(argv[3]) > MAX_THREADS) {
    //     printf("Número de threads maior que o máximo permitido\n");
    //     return 0;
    } else {
        // numThreads = atoi(argv[3]);
        // nP = atoi(argv[2]);
        // n = atoi(argv[1]);
    }

    return 0;
}