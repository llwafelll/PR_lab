#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pomiar_czasu.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

int zmienna_globalna=0;

#define ROZMIAR_STOSU 1024 * 64

int funkcja_watku(void *arg)
{

	char *loc_arg = (char *)arg;

	printf("zmienna_globalna przed modyfikacja w watku potomnym: %d\n", zmienna_globalna);
	printf("loc_arg przed modyfikacja w watku potomnym: %s\n", loc_arg);

	// Nadpisuje dane - globalna i lokalna
	zmienna_globalna = 20;
	strcpy(loc_arg, "nadpisano w watku potomnym");

  return 0;
}

int main(int argc, char *argv[])
{
	unsigned int flags = 0;
	flags |= CLONE_VM;

  int i; 
  void *stos;
  pid_t pid;

  char arg[64];
	strcpy(arg, "Poczatkowa wartosc amiennej arg");
  
  printf("parent pid: %d\n", getpid());
  stos = malloc( ROZMIAR_STOSU );
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 flags, (void *) arg);


  if (pid == -1)
	  printf("watek nie zostal utworzony.\n");
  else
	  printf("watek zostal utorzony.\n");

  waitpid(pid, NULL, __WCLONE);

	printf("zmienna_globalna po modyfikacji w watku potomnym wypisana z main: %d\n",
			zmienna_globalna);
	printf("arg po modyfikacji w watku potomnym wypisana z main: %s\n", arg);


  free( stos );
}
