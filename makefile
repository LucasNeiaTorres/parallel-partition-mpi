# flags = -Wall
# name = multi_partition_mpi

# all: $(name)

# $(name): multi_partition_mpi.o chrono.o verifica_particoes.o
# 	mpicc -o $(name) multi_partition_mpi.o chrono.o verifica_particoes.o $(flags)

# multi_partition_mpi.o: multi_partition_mpi.c
# 	mpicc -c multi_partition_mpi.c $(flags)

# chrono.o: chrono.c
# 	mpicc -c chrono.c $(flags)

# verifica_particoes.o: verifica_particoes.c
# 	mpicc -c verifica_particoes.c $(flags)

# clean:
# 	rm -f *~ *.o

# purge: clean
# 	rm -f $(name)

arquivos=multi_partition_sem.o verifica_particoes.o
all: multi_partition_sem verifica_particoes

multi_partition_sem: multi_partition_sem.o verifica_particoes.o chrono.o
	gcc -o multi_partition_sem multi_partition_sem.o verifica_particoes.o chrono.o $(parametrosCompilacao)

verifica_particoes.o: verifica_particoes.h verifica_particoes.c
	gcc -c verifica_particoes.c $(parametrosCompilacao)

chrono.o: chrono.h chrono.c
	gcc -c chrono.c $(parametrosCompilacao)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch multi_partition_sem verifica_particoes chrono

purge: clean
	-rm -f multi_partition_sem verifica_particoes chrono