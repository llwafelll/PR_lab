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

int funkcja_watku()
{
  //pid_t pid = getpid();
  //printf("child pid: %d\n", pid);
  char table[4];
  //printf("HELLO %d %d %d %d\n", table[0], table[1], table[2], table[3]);
  //printf("HELLO %d %d %d %d\n", 'a', 'b', 'c', 'd');

  return 0;
}

int main(int argc, char *argv[])
{
  int i; 
  void *stos;
  pid_t pid;
  // char arg[10];
  
  printf("parent pid: %d\n", getpid());
  stos = malloc( ROZMIAR_STOSU );
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 SIGCHLD, 0);

  if (pid == -1)
	  printf("watek nie zostal utworzony.\n");
  else
	  printf("watek zostal utorzony.\n");

  waitpid(pid, NULL, __WCLONE);


  free( stos );
}
