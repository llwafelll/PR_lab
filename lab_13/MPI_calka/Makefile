# kompilator c
CCOMP = /home/kali/opt/openmpi/bin/mpicc

# konsolidator
LINK = /home/kali/opt/openmpi/bin/mpicc

# program uruchomieniowy MPI
MPI_run = /home/kali/opt/openmpi/bin/mpiexec

# opcje optymalizacji:
# wersja do debugowania
# OPT = -g -DDEBUG -p
# wersja zoptymalizowana do mierzenia czasu
OPT = -O3 -fopenmp 

# pliki naglowkowe
#INC = -I../pomiar_czasu

# biblioteki
#LIB = -L../pomiar_czasu -lm
LIB = -lm

# zaleznosci i komendy
#calka: calka.o pomiar_czasu.o
#	$(LINK) $(OPT) calka.o pomiar_czasu.o -o calka $(LIB)
#
#calka.o: calka.c pomiar_czasu.h
#	$(CCOMP) -c $(OPT) calka.c $(INC)
#
#pomiar_czasu.o: pomiar_czasu.c pomiar_czasu.h
#	$(CCOMP) -c $(OPT) pomiar_czasu.c

all: MPI_calka run 

MPI_calka: MPI_calka.o
	$(LINK) $(OPT) MPI_calka.o -o MPI_calka $(LIB)

MPI_calka.o: MPI_calka.c
	$(CCOMP) -c $(OPT) MPI_calka.c $(INC)

run: MPI_calka
	$(MPI_run) -np 4 MPI_calka

clean:
	rm -f *.o
