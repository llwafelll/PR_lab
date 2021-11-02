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

#define ROZMIAR_STOSU 1024*64

int funkcja_watku( void* argument )
{
  pid_t pid = getpid();
  char pids[10];
  sprintf(pids, "%d", pid);
  char *arg = (char*)argument;

  int wynik;

  char *args[] = {"myinfo", pids, arg, NULL};
  wynik=execv("./myinfo", args);
  if(wynik==-1)
    printf("Proces potomny nie wykonal programu\n");

  return 0;
}

int main(int argc, char *argv[])
{
  int i; 
  void *stos;
  pid_t pid;
  char arg[10];
  
  stos = malloc( ROZMIAR_STOSU );
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  if (argc <= 1) strcpy(arg, "");
  else if (argc == 2) strcpy(arg, argv[1]);

  pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*) arg);

  waitpid(pid, NULL, __WCLONE);


  free( stos );
}
