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
FILE *fd;

#define ROZMIAR_STOSU 1024 * 64

int funkcja_watku(void *arg)
{
	zmienna_globalna = 20;
	fclose(fd);

  return 0;
}

int main(int argc, char *argv[])
{
	unsigned int flags = 0;
	flags |= CLONE_FILES;

  int i, status; 
  void *stos;
  pid_t pid;

  char arg[64];
	strcpy(arg, "Poczatkowa wartosc amiennej arg");

	fd = fopen("clone.c", "r");
  
  printf("parent pid: %d\n", getpid());
  stos = malloc( ROZMIAR_STOSU );
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 flags, 0);


  if (pid == -1) {
	  printf("watek nie zostal utworzony.\n");
	}
  else
	  printf("watek zostal utorzony.\n");

  waitpid(pid, NULL, __WCLONE);

	//naspisanie zmiennej arg nie nastapi jesli fd bedzie zamkniety
	fscanf(fd, "%s", arg);
	printf("read data: %s\n", arg);

	printf("zmienna_globalna po modyfikacji w watku potomnym wypisana z main: %d\n",
			zmienna_globalna);
	printf("arg po modyfikacji w watku potomnym wypisana z main: %s\n", arg);

	fclose(fd);
  free( stos );
}
