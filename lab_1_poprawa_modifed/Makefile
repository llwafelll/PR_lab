# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania (jest wybierana domyslnie)
ifndef $(mode)
OPT = -g -DDEBUG
endif

# make mode=debug
ifeq ($(mode), debug)
OPT = -g -DDEBUG
endif

# wersja zoptymalizowana do mierzenia czasu (make mode=optimized)
ifeq ($(mode), optimized)
OPT = -O3
endif


# pliki naglowkowe
INC = -I./pomiar_czasu/

# biblioteki
LIB = -L./pomiar_czasu/ -lpomiar_czasu -lm

# ustawienie automatycznego czyszczenia plikow
run: moj_program clean

# zaleznosci i komendy
moj_program: moj_program.o pomiar_czasu.o
	$(LOADER) $(OPT) moj_program.o ./pomiar_czasu/pomiar_czasu.o -o moj_program

# jak uzyskac plik moj_program.o ?
moj_program.o: moj_program.c libmoj_program.a
	$(CCOMP) -c $(OPT) moj_program.c -o moj_program.o $(INC) $(LIB)

libmoj_program.a: pomiar_czasu.o
	ar -rs ./pomiar_czasu/libpomiar_czasu.a ./pomiar_czasu/pomiar_czasu.o
	
pomiar_czasu.o: ./pomiar_czasu/pomiar_czasu.c ./pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) ./pomiar_czasu/pomiar_czasu.c -o ./pomiar_czasu/pomiar_czasu.o

clean:
	rm -f *.o ./pomiar_czasu/*.o ./pomiar_czasu/*.a
