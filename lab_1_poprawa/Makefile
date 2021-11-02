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
INC = -I./.

# biblioteki
#LIB = -L../pomiar_czasu -lm

# ustawienie automatycznego czyszczenia plikow
run: moj_program clean

# zaleznosci i komendy
moj_program: moj_program.o pomiar_czasu.o
	$(LOADER) $(OPT) moj_program.o pomiar_czasu.o -o moj_program

# jak uzyskac plik moj_program.o ?
moj_program.o: moj_program.c
	$(CCOMP) -c $(OPT) moj_program.c -o moj_program.o $(INC)
	
pomiar_czasu.o: pomiar_czasu.c pomiar_czasu.h
	$(CCOMP) -c $(OPT) pomiar_czasu.c

clean:
	rm -f *.o
