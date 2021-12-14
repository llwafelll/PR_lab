#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

// Odpowiednik chce_czytac
int my_read_lock_lock(czytelnia_t* czytelnia_p){
    ++(czytelnia_p->czekajacy_czyt);
    pthread_rwlock_rdlock(&(czytelnia_p->rwlock));
    ++(czytelnia_p->liczba_czyt);
    --(czytelnia_p->czekajacy_czyt);
}


// Odpowiednik koniec_czytania
int my_read_lock_unlock(czytelnia_t* czytelnia_p){
    --(czytelnia_p->liczba_czyt);
    pthread_rwlock_unlock(&(czytelnia_p->rwlock));
}


// Odpowiednik chce_pisac
int my_write_lock_lock(czytelnia_t* czytelnia_p){
    ++(czytelnia_p->czekajacy_pis);
    pthread_rwlock_wrlock(&(czytelnia_p->rwlock));
    ++(czytelnia_p->liczba_pisz);
    --(czytelnia_p->czekajacy_pis);
}


// Odpowiednik koniec_pisania
int my_write_lock_unlock(czytelnia_t* czytelnia_p){
    --(czytelnia_p->liczba_pisz);
    pthread_rwlock_unlock(&(czytelnia_p->rwlock));
}

void inicjuj(czytelnia_t* czytelnia_p){
    czytelnia_p->liczba_czyt = 0;
    czytelnia_p->liczba_pisz = 0;
    czytelnia_p->czekajacy_czyt = 0;
    czytelnia_p->czekajacy_pis = 0;
    pthread_rwlock_init(&(czytelnia_p->rwlock), NULL);
}

void czytam(czytelnia_t* czytelnia_p){
    usleep(rand()%300000);
}

void pisze(czytelnia_t* czytelnia_p){
    usleep(rand()%300000);
}


