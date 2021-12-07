#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

void *funkcja_czytelnika( void *);
void *funkcja_pisarza( void *);


int main(){
  
  int i;
  pthread_t pisarze[5], czytelnicy[10];
  int indeksy[10] = {0,1,2,3,4,5,6,7,8,9}; 
  czytelnia_t czytelnia;
  
  // ustawia liczbe czyt i liczbę pisz na 0
  inicjuj(&czytelnia);
    
  for(i=0; i<5; i++){
    pthread_create( &pisarze[i], NULL, funkcja_pisarza, (void *)&czytelnia );
  }
  for(i=0; i<10; i++){
    pthread_create( &czytelnicy[i], NULL, funkcja_czytelnika, (void *)&czytelnia );
  }
  for(i=0; i<5; i++){
    pthread_join( pisarze[i], NULL); 
  }
  for(i=0; i<10; i++){
    pthread_join( czytelnicy[i], NULL ); 
  }
  
}

void *funkcja_czytelnika( void * arg){
  
  czytelnia_t* czytelnia_p = (czytelnia_t *)arg;
  
  for(;;){
    
    usleep(rand()%1000000);
    printf("czytelnik %lu - przed zamkiem\n", pthread_self());
    printf("\033[0;31mAltualna liczba czytelnikow w czytelni: %d\033[0;1m\n",
           czytelnia_p->liczba_czyt);
    printf("\033[0;31mAltualna liczba czytelnikow w kolejce: %d\033[0;1m\n",
           czytelnia_p->czekajacy_czyt);
    
    my_read_lock_lock(czytelnia_p);
    
    // korzystanie z zasobow czytelni
    printf("czytelnik %lu - wchodze\n", pthread_self());

    // Jesli w czytelni jest co najmniej jeden pisarz
    // po wejsciu czytelnika - przerwij program
    if (czytelnia_p->liczba_pisz > 0) {
        printf("Blad - pisarze nie moga znajdowac sie w czytelni, kiedy wchodzi czytelnik\n");
        printf("Lpisz: %d\n", czytelnia_p->liczba_pisz);
        exit(0);
    }
    
    czytam(czytelnia_p);
    
    printf("czytelnik %lu - wychodze\n", pthread_self());
    
    my_read_lock_unlock(czytelnia_p);
    
    printf("czytelnik %lu - po zamku\n", pthread_self());
    
  }
  
}

void *funkcja_pisarza( void * arg){
  
  czytelnia_t* czytelnia_p = (czytelnia_t *)arg;
  
  for(;;){
    
    usleep(rand()%3000000);
    printf("pisarz %lu - przed zamkiem\n", pthread_self());
    printf("\033[0;34mAltualna liczba pisarzy w czytelni: %d\033[0m\n", 
           czytelnia_p->liczba_pisz);
    printf("\033[0;34mAltualna liczba pisarzy w kolejce: %d\033[0m\n",
           czytelnia_p->czekajacy_pis);
    
    my_write_lock_lock(czytelnia_p);
    
    // korzystanie z zasobow czytelni
    printf("pisarz %lu - wchodze\n", pthread_self());
    
    // Jesli po wejsciu pisarza do czytelni jest w niej
    // wiecej niz jedna osoba przerwij program
    if (czytelnia_p->liczba_pisz + czytelnia_p->liczba_czyt > 1) {
        printf("Blad - czytelnia musi byc pusta, kiedy wchodzi pisarz.\n");
        printf("Lpis: %d, Lczyt: %d\n", czytelnia_p->liczba_pisz,
               czytelnia_p->czekajacy_czyt);
        exit(0);
    }

    pisze(czytelnia_p);
    
    printf("pisarz %lu - wychodze\n", pthread_self());
    
    my_write_lock_unlock(czytelnia_p);
    
    printf("pisarz %lu - po zamku\n", pthread_self());
  }
  
}


