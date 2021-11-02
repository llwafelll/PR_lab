#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pomiar_czasu.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

int zmienna_globalna=0;

#define ROZMIAR_STOSU 1024*64

int funkcja_watku( void* argument )
{

	static int i;
	int zmienna_lokalna_watku = (*(int*)argument);

	for (i = 0; i < 100000; ++i) {
		++zmienna_globalna;
		++zmienna_lokalna_watku;
	}

	printf("Wartosci zmiennych wypisane z wewnatrz funkcji funkcja_watku:\n");
	printf("zmienna globalna: %d\n", zmienna_globalna++);
	printf("zmienna lokalna watku: %d\n\n", zmienna_lokalna_watku++);

  /* int wynik; */
  /* wynik=execv("./program",NULL); */
  /* if(wynik==-1) */
  /*   printf("Proces potomny nie wykonal programu\n"); */

  return 0;
}

int main()
{

  int zmienna_lokalna = 0;
  void *stos1;
  void *stos2;
  pid_t pid1, pid2;
  int i; 
  
  stos1 = malloc( ROZMIAR_STOSU );
  stos2 = malloc( ROZMIAR_STOSU );

  if (stos1 == 0 || stos2 == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

	inicjuj_czas();
	printf("\n");

	pid1 = clone( &funkcja_watku, (void *) stos1+ROZMIAR_STOSU, 
	 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &zmienna_lokalna );

	pid2 = clone( &funkcja_watku, (void *) stos2+ROZMIAR_STOSU, 
	 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &zmienna_lokalna );

	waitpid(pid1, NULL, __WCLONE);
	waitpid(pid2, NULL, __WCLONE);
	
	printf("Wartosci zmiennych po zakonczeniu wykonywania sie watkow (z poziomu main):\n");
	printf("zmienna globalna: %d\n", zmienna_globalna);
	printf("zmienna lokalna : %d\n\n", zmienna_lokalna);

	printf("Czas wykonywania sie obu watkow:\n");
	drukuj_czas();
	printf("\n");

  free( stos1 );
  free( stos2 );
}
