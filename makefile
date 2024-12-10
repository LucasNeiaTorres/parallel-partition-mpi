parametrosCompilacao=-lpthread
arquivos=multi_partition_mpi.o verifica_particoes.o
all: multi_partition_mpi verifica_particoes

multi_partition_mpi: multi_partition_mpi.o verifica_particoes.o chrono.o
	gcc -o multi_partition_mpi multi_partition_mpi.o verifica_particoes.o chrono.o $(parametrosCompilacao)

verifica_particoes.o: verifica_particoes.h verifica_particoes.c
	gcc -c verifica_particoes.c $(parametrosCompilacao)

chrono.o: chrono.h chrono.c
	gcc -c chrono.c $(parametrosCompilacao)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch multi_partition_mpi verifica_particoes chrono

purge: clean
	-rm -f multi_partition_mpi verifica_particoes chrono