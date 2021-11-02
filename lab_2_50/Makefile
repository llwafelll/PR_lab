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
OPT = -g -DDEBUG -O0
endif

# wersja zoptymalizowana do mierzenia czasu (make mode=optimized)
ifeq ($(mode), optimized)
OPT = -O3
endif

# pliki naglowkowe
INC = -I./pomiar_czasu

# biblioteki
LIB = -L./pomiar_czasu -lpomiar_czasu -lm

# in command line >sudo make prog=fork|clone
run: $(prog) clean

# zaleznosci i komendy
fork: fork.o 
	$(LOADER) $(OPT) fork.o -o fork $(LIB)

# jak uzyskac plik fork.o ?
fork.o: fork.c ./pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) fork.c $(INC) 

# zaleznosci i komendy
clone: clone.o 
	$(LOADER) $(OPT) clone.o -o clone $(LIB)

# jak uzyskac plik clone.o ?
clone.o: clone.c ./pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone.c $(INC) 

# zaleznosci i komendy
pzadanie: pzadanie.o 
	$(LOADER) $(OPT) pzadanie.o -o pzadanie $(LIB)

# jak uzyskac plik clone.o ?
pzadanie.o: pzadanie.c ./pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) pzadanie.c $(INC) 

# zaleznosci i komendy
clone_with_vm: clone_with_vm.o 
	$(LOADER) $(OPT) clone_with_vm.o -o clone_with_vm $(LIB)

# jak uzyskac plik clone.o ?
clone_with_vm.o: clone_with_vm.c ./pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone_with_vm.c $(INC) 

# zaleznosci i komendy
clone_with_file: clone_with_file.o 
	$(LOADER) $(OPT) clone_with_file.o -o clone_with_file $(LIB)

# jak uzyskac plik clone.o ?
clone_with_file.o: clone_with_file.c ./pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone_with_file.c $(INC) 

clean:
	rm -f *.o
	rm -f ./pomiar_czasu/*.o
