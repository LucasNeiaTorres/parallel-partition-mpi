flags = -Wall
name = multi_partition_mpi

all: $(name)

$(name): multi_partition_mpi.o chrono.o verifica_particoes.o
	mpicc -o $(name) multi_partition_mpi.o chrono.o verifica_particoes.o $(flags)

multi_partition_mpi.o: multi_partition_mpi.c
	mpicc -c multi_partition_mpi.c $(flags)

chrono.o: chrono.c
	mpicc -c chrono.c $(flags)

verifica_particoes.o: verifica_particoes.c
	mpicc -c verifica_particoes.c $(flags)

clean:
	rm -f *~ *.o

purge: clean
	rm -f $(name)