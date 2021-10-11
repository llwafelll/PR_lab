# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
# OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu


# pliki naglowkowe
INC = -I./pomiar_czasu/

# biblioteki
LIB = -L./pomiar_czasu/ -lm

# to make 'clean' command work
all: moj_program clean

# zaleznosci i komendy
moj_program: moj_program.o pomiar_czasu.o
	$(LOADER) $(OPT) moj_program.o pomiar_czasu.o -o moj_program $(LIB)

# jak uzyskac plik moj_program.o ?
moj_program.o: 
	$(CCOMP) -c moj_program.c $(INC)
	

pomiar_czasu.o: ./pomiar_czasu/pomiar_czasu.c ./pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) ./pomiar_czasu/pomiar_czasu.c

clean:
	rm -f *.o
