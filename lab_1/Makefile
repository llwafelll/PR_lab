# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -g -O2


# pliki naglowkowe
INC = -I./pomiar_czasu

# sciezka do bibliotek statycznych
LIBDIR = -L./pomiar_czasu -lm

# biblioteki statyczne
LIB = -lpomiar_czasu

# to make 'clean' command work
all: moj_program clean

# zaleznosci i komendy
moj_program: moj_program.o
	$(LOADER) $(OPT) moj_program.o $(LIB) $(LIBDIR) -o moj_program

moj_program.o: moj_program.c
	$(LOADER) $(OPT) -c moj_program.c $(INC) -o moj_program.o

clean:
	rm -f *.o
